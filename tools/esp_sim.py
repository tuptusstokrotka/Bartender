#!/usr/bin/env python3
"""
ESP-style data bridge for the Bart UI (separate from the static file server).

  python esp_sim.py -g 6 -f 1.25 --bind 127.0.0.1 -p 9000

Then open the page from your static server with:
  index.html?esp=http://127.0.0.1:9000

HTTP: POST /esp/display-weight (JSON), POST /esp/normal-mode (return device UI to normal; clears liveWeightDisplay in sim),
  POST /esp/setpoint (JSON {"pourMl": number}) — keeps SSE/state in sync with the UI setpoint.

Keys (this terminal, Windows = single key):
  qwertyuiop  select glass 1..10 (clamped to -g count)
  a           place glass / cycle tare: empty -> 20 g; then 20 g <-> 0 g
  s / d       add / subtract net (5 g)
"""
from __future__ import annotations

import argparse
import json
import os
import sys
import threading
import time
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from typing import Any
from urllib.parse import urlparse

SIM_LOCK = threading.Lock()
SIM_STATE: dict[str, Any] = {}
SIM_SELECTED = 0
SIM_FACTOR = 1.25
_KEY_ROW = "qwertyuiop"


def _default_state(n: int) -> dict[str, Any]:
    return {
        "glassCount": n,
        "calibrationFactor": SIM_FACTOR,
        "pourMl": 150.0,
        "bartenderState": "idle",
        "softwareVersion": "esp-sim",
        "commitHash": "",
        "author": "",
        "liveWeightDisplay": False,
        "glasses": [{"tareG": None, "netG": None} for _ in range(n)],
    }


def _ensure_glasses() -> None:
    n = int(SIM_STATE.get("glassCount") or 0)
    if n < 1:
        n = 1
    SIM_STATE["glassCount"] = n
    glasses: list[dict[str, Any]] = list(SIM_STATE.get("glasses") or [])
    while len(glasses) < n:
        glasses.append({"tareG": None, "netG": None})
    SIM_STATE["glasses"] = glasses[:n]


def _as_float(v: Any) -> float | None:
    if v is None:
        return None
    try:
        return float(v)
    except (TypeError, ValueError):
        return None


def _occ(g: dict[str, Any]) -> bool:
    if "occupied" in g and isinstance(g["occupied"], bool):
        return g["occupied"]
    return g.get("tareG") is not None or g.get("netG") is not None


def _wire_json() -> str:
    with SIM_LOCK:
        snap = dict(SIM_STATE)
        snap["calibrationFactor"] = SIM_FACTOR
        return json.dumps(snap, separators=(",", ":"))


def print_keyboard_help() -> None:
    n = SIM_STATE.get("glassCount", "?")
    sys.stdout.write(
        "\n--- esp_sim keys ---\n"
        f"  {_KEY_ROW}  select glass (1..10, max {n})\n"
        "  a           place glass or cycle tare 20 g <-> 0 g\n"
        "  s / d       add / subtract net (5 g)\n"
        f"  factor fixed at {SIM_FACTOR}  (-f)\n"
        "  h           help\n\n",
    )
    sys.stdout.flush()


def handle_key(ch: bytes) -> None:
    global SIM_SELECTED
    if not ch:
        return
    b0 = ch[0]
    if b0 in (13, 10, 32):
        return
    c = chr(b0) if b0 < 128 else "?"
    lc = c.lower()

    with SIM_LOCK:
        _ensure_glasses()
        n = int(SIM_STATE["glassCount"])
        SIM_SELECTED = max(0, min(n - 1, SIM_SELECTED))
        g = SIM_STATE["glasses"][SIM_SELECTED]

        if lc in _KEY_ROW:
            idx = _KEY_ROW.index(lc)
            SIM_SELECTED = min(idx, max(0, n - 1))
        elif lc == "a":
            if not _occ(g):
                g["occupied"] = True
                g["tareG"] = 20.0
                g["netG"] = 0.0
            else:
                t = _as_float(g.get("tareG"))
                if t is None or abs(t) < 0.01:
                    g["tareG"] = 20.0
                elif abs(t - 20.0) < 0.01:
                    g["tareG"] = 0.0
                else:
                    g["tareG"] = 20.0
        elif lc == "s":
            if _occ(g):
                g["netG"] = float(g.get("netG") or 0.0) + 5.0
        elif lc == "d":
            if _occ(g):
                g["netG"] = max(0.0, float(g.get("netG") or 0.0) - 5.0)

        SIM_STATE["calibrationFactor"] = SIM_FACTOR

    if lc in "h?":
        print_keyboard_help()


def _keyboard_win() -> None:
    import msvcrt

    while True:
        while msvcrt.kbhit():
            ch = msvcrt.getch()
            if ch in (b"\x00", b"\xe0") and msvcrt.kbhit():
                msvcrt.getch()
                continue
            handle_key(ch)
        time.sleep(0.04)


def _keyboard_line() -> None:
    for line in iter(sys.stdin.readline, ""):
        for ch in line.strip():
            handle_key(ch.encode("utf-8", errors="ignore")[:1] or b" ")


def keyboard_thread_main() -> None:
    print_keyboard_help()
    try:
        if sys.platform == "win32":
            _keyboard_win()
        else:
            _keyboard_line()
    except Exception as e:  # noqa: BLE001
        sys.stderr.write(f"Keyboard thread: {e}\n")


class EspHandler(BaseHTTPRequestHandler):
    server_version = "esp_sim/1.0"

    def log_message(self, fmt: str, *args: object) -> None:
        if "/esp/events" in str(args):
            return
        super().log_message(fmt, *args)

    def _cors(self) -> None:
        self.send_header("Access-Control-Allow-Origin", "*")
        self.send_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS")
        self.send_header("Access-Control-Allow-Headers", "Content-Type")

    def do_OPTIONS(self) -> None:  # noqa: N802
        self.send_response(204)
        self._cors()
        self.end_headers()

    def do_GET(self) -> None:  # noqa: N802
        path = urlparse(self.path).path
        if path == "/esp/state":
            body = _wire_json()
            data = body.encode("utf-8")
            self.send_response(200)
            self.send_header("Content-Type", "application/json; charset=utf-8")
            self.send_header("Content-Length", str(len(data)))
            self._cors()
            self.send_header("Cache-Control", "no-store")
            self.end_headers()
            self.wfile.write(data)
            return
        if path == "/esp/events":
            self.send_response(200)
            self.send_header("Content-Type", "text/event-stream; charset=utf-8")
            self.send_header("Cache-Control", "no-store")
            self._cors()
            self.end_headers()
            try:
                while True:
                    payload = _wire_json()
                    self.wfile.write(f"data: {payload}\n\n".encode("utf-8"))
                    self.wfile.flush()
                    time.sleep(0.28)
            except (BrokenPipeError, ConnectionResetError, TimeoutError, ValueError):
                return
        self.send_response(404)
        self._cors()
        self.end_headers()

    def do_POST(self) -> None:  # noqa: N802
        path = urlparse(self.path).path
        if path == "/esp/display-weight":
            length = int(self.headers.get("Content-Length", "0") or 0)
            if length > 65536:
                length = 65536
            raw = self.rfile.read(length) if length > 0 else b"{}"
            try:
                json.loads(raw.decode("utf-8", errors="replace") or "{}")
            except (json.JSONDecodeError, UnicodeError):
                pass
            with SIM_LOCK:
                _ensure_glasses()
                SIM_STATE["liveWeightDisplay"] = True
            self.send_response(204)
            self._cors()
            self.end_headers()
            sys.stdout.write("[esp_sim] live weight display requested (POST /esp/display-weight)\n")
            sys.stdout.flush()
            return
        if path == "/esp/normal-mode":
            length = int(self.headers.get("Content-Length", "0") or 0)
            if length > 65536:
                length = 65536
            if length > 0:
                self.rfile.read(length)
            with SIM_LOCK:
                _ensure_glasses()
                SIM_STATE["liveWeightDisplay"] = False
            self.send_response(204)
            self._cors()
            self.end_headers()
            sys.stdout.write("[esp_sim] normal mode (POST /esp/normal-mode)\n")
            sys.stdout.flush()
            return
        if path == "/esp/setpoint":
            length = int(self.headers.get("Content-Length", "0") or 0)
            if length > 65536:
                length = 65536
            raw = self.rfile.read(length) if length > 0 else b"{}"
            try:
                body = json.loads(raw.decode("utf-8", errors="replace") or "{}")
            except (json.JSONDecodeError, UnicodeError):
                body = {}
            ml = _as_float(body.get("pourMl"))
            if ml is not None and ml > 0 and ml <= 9999:
                with SIM_LOCK:
                    _ensure_glasses()
                    SIM_STATE["pourMl"] = float(ml)
            self.send_response(204)
            self._cors()
            self.end_headers()
            return
        self.send_response(404)
        self._cors()
        self.end_headers()


class Server(ThreadingHTTPServer):
    allow_reuse_address = True


def _port_candidates(cli_port: int | None) -> list[int]:
    ports: list[int] = []
    if cli_port is not None:
        ports.append(int(cli_port))
    raw = os.environ.get("ESP_SIM_PORT")
    if raw:
        try:
            ports.append(int(raw))
        except ValueError:
            pass
    ports.extend([9000, 9001, 9010, 8765, 8888])
    seen: set[int] = set()
    out: list[int] = []
    for p in ports:
        if p not in seen and 1 <= p <= 65535:
            seen.add(p)
            out.append(p)
    return out


def create_server(host: str, cli_port: int | None) -> tuple[Server, int]:
    last_err: OSError | None = None
    for port in _port_candidates(cli_port):
        try:
            return Server((host, port), EspHandler), port
        except OSError as e:
            last_err = e
            win = getattr(e, "winerror", None)
            if win in (10013, 10048) or e.errno in (98, 48, 13):
                continue
            raise
    assert last_err is not None
    raise last_err


def main() -> None:
    global SIM_STATE, SIM_FACTOR  # noqa: PLW0603
    p = argparse.ArgumentParser(description="Bart ESP bridge (SSE + keyboard) for static-served UI")
    p.add_argument("-g", "--glasses", type=int, default=4, metavar="N", help="Number of glass slots (default 4)")
    p.add_argument("-f", "--factor", type=float, default=1.25, help="Calibration factor pushed to UI (default 1.25)")
    p.add_argument("--bind", default=os.environ.get("ESP_SIM_BIND", "127.0.0.1"))
    p.add_argument("-p", "--port", type=int, default=None)
    args = p.parse_args()

    n = max(1, min(64, args.glasses))
    SIM_FACTOR = float(args.factor)
    SIM_STATE = _default_state(n)

    t = threading.Thread(target=keyboard_thread_main, name="esp-sim-keys", daemon=True)
    t.start()

    httpd, port = create_server(args.bind, args.port)
    with httpd:
        print(f"esp_sim listening: http://{args.bind}:{port}/")
        print(f"  State: http://{args.bind}:{port}/esp/state")
        print(f"  SSE:   http://{args.bind}:{port}/esp/events")
        print(f"Open static index.html with:  ?esp=http://{args.bind}:{port}")
        print("Ctrl+C to stop.\n")
        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            print()


if __name__ == "__main__":
    main()
