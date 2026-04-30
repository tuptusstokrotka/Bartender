#!/usr/bin/env python3
"""
Local Bart host: serves index.html and the same /esp/* API the ESP32 firmware should expose.

  Set-Location <folder>; python esp_sim.py

Then open http://127.0.0.1:9527/ (default port; tries others if busy). The UI uses same-origin
GET /esp/state and EventSource /esp/events — no cross-origin ?esp= parameter.

On Windows PowerShell 5.x use a semicolon between commands, not &&.

HTTP (CORS * on API): GET / and /index.html, GET /esp/state, GET /esp/events (SSE),
  POST /esp/calibration-begin (show live weight on device), POST /esp/idle (return to idle, no body),
  POST /esp/apply-factor {"factor":n}, POST /esp/pour-ml {"pour_ml":n},
  POST /esp/serve {"glass":i}, POST /esp/serve-all (no body), POST /esp/config (dev bulk JSON).

Stdin commands (1-based slot index), one per line:
  help
  pour <ml>              set pour setpoint (ml)
  factor <f>             set calibration factor
  slots <n>              resize number of glass pads (1..64)
  empty <slot>           clear glass on slot
  place <slot> <tare_g> [net_g]   glass on pad with tare (g) and optional net liquid (g)
  net <slot> <g>         set net mass (g); use +5 or -5 to adjust
  state                  print JSON snapshot to stderr
  quit
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

_SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
_INDEX_PATH = os.path.join(_SCRIPT_DIR, "index.html")

_LOCK = threading.Lock()
_STATE: dict[str, Any] = {}


def _default_state(n: int, factor: float, pour_ml: float) -> dict[str, Any]:
    return {
        "glassCount": n,
        "calibrationFactor": float(factor),
        "pourMl": float(pour_ml),
        "bartenderState": "idle",
        "softwareVersion": "bart-bridge",
        "commitHash": "",
        "author": "",
        "liveWeightDisplay": False,
        "glasses": [{"tareG": None, "netG": None} for _ in range(n)],
    }


def _ensure_glasses() -> None:
    n = int(_STATE.get("glassCount") or 0)
    if n < 1:
        n = 1
    _STATE["glassCount"] = n
    glasses: list[dict[str, Any]] = list(_STATE.get("glasses") or [])
    while len(glasses) < n:
        glasses.append({"tareG": None, "netG": None})
    _STATE["glasses"] = glasses[:n]


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
    with _LOCK:
        return json.dumps(dict(_STATE), separators=(",", ":"))


def _bridge_log_post(path: str, body: dict[str, Any] | None) -> None:
    if not body:
        sys.stdout.write(f"[bridge] POST {path} (empty body)\n")
    else:
        sys.stdout.write(f"[bridge] POST {path} {json.dumps(body, separators=(',', ':'))}\n")
    sys.stdout.flush()


def _print_help() -> None:
    sys.stderr.write(
        "\nCommands (slot = 1..N):  pour <ml>  factor <f>  slots <n>  "
        "empty <slot>  place <slot> <tare_g> [net_g]  net <slot> <g>|+d|-d  state  quit\n\n",
    )
    sys.stderr.flush()


def _parse_slot(tok: str, n: int) -> int | None:
    try:
        s = int(tok, 10)
    except ValueError:
        return None
    if s < 1 or s > n:
        return None
    return s - 1


def _handle_line(line: str) -> None:
    parts = line.strip().split()
    if not parts:
        return
    cmd = parts[0].lower()
    if cmd in ("help", "h", "?"):
        _print_help()
        return
    if cmd in ("quit", "q", "exit"):
        sys.stderr.write("Stop the bridge with Ctrl+C in this terminal.\n")
        sys.stderr.flush()
        return
    with _LOCK:
        _ensure_glasses()
        n = int(_STATE["glassCount"])

    if cmd == "state":
        sys.stderr.write(_wire_json() + "\n")
        sys.stderr.flush()
        return

    if cmd == "pour" and len(parts) >= 2:
        ml = _as_float(parts[1])
        if ml is not None and ml > 0:
            with _LOCK:
                _ensure_glasses()
                _STATE["pourMl"] = float(ml)
        return

    if cmd == "factor" and len(parts) >= 2:
        f = _as_float(parts[1])
        if f is not None and f > 0:
            with _LOCK:
                _ensure_glasses()
                _STATE["calibrationFactor"] = float(f)
        return

    if cmd == "slots" and len(parts) >= 2:
        try:
            nn = int(parts[1], 10)
        except ValueError:
            return
        nn = max(1, min(64, nn))
        with _LOCK:
            _ensure_glasses()
            cur = list(_STATE["glasses"])
            if nn > len(cur):
                cur.extend({"tareG": None, "netG": None} for _ in range(nn - len(cur)))
            else:
                cur = cur[:nn]
            _STATE["glassCount"] = nn
            _STATE["glasses"] = cur
        return

    if cmd == "empty" and len(parts) >= 2:
        idx = _parse_slot(parts[1], n)
        if idx is None:
            return
        with _LOCK:
            _ensure_glasses()
            g = _STATE["glasses"][idx]
            g["tareG"] = None
            g["netG"] = None
            g.pop("occupied", None)
        return

    if cmd == "place" and len(parts) >= 3:
        idx = _parse_slot(parts[1], n)
        if idx is None:
            return
        t = _as_float(parts[2])
        net = _as_float(parts[3]) if len(parts) >= 4 else 0.0
        if t is None:
            return
        with _LOCK:
            _ensure_glasses()
            g = _STATE["glasses"][idx]
            g["tareG"] = float(t)
            g["netG"] = float(net or 0.0)
            g["occupied"] = True
        return

    if cmd == "net" and len(parts) >= 3:
        idx = _parse_slot(parts[1], n)
        if idx is None:
            return
        raw = parts[2]
        with _LOCK:
            _ensure_glasses()
            g = _STATE["glasses"][idx]
            if not _occ(g):
                return
            cur = float(g.get("netG") or 0.0)
            if raw.startswith("+") or raw.startswith("-"):
                delta = _as_float(raw)
                if delta is None:
                    return
                g["netG"] = max(0.0, cur + delta)
            else:
                v = _as_float(raw)
                if v is None:
                    return
                g["netG"] = max(0.0, float(v))
        return

    sys.stderr.write(f"Unknown or incomplete command: {line!r}\n")
    sys.stderr.flush()


def _stdin_loop() -> None:
    _print_help()
    try:
        for line in iter(sys.stdin.readline, ""):
            _handle_line(line)
    except Exception as e:  # noqa: BLE001
        sys.stderr.write(f"stdin loop: {e}\n")
        sys.stderr.flush()


class EspHandler(BaseHTTPRequestHandler):
    server_version = "bart-bridge/1.0"

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
        if path in ("/", "/index.html"):
            try:
                with open(_INDEX_PATH, "rb") as f:
                    data = f.read()
            except OSError:
                msg = b"index.html not found next to esp_sim.py"
                self.send_response(500)
                self.send_header("Content-Type", "text/plain; charset=utf-8")
                self.send_header("Content-Length", str(len(msg)))
                self._cors()
                self.end_headers()
                self.wfile.write(msg)
                return
            self.send_response(200)
            self.send_header("Content-Type", "text/html; charset=utf-8")
            self.send_header("Content-Length", str(len(data)))
            self._cors()
            self.send_header("Cache-Control", "no-store")
            self.end_headers()
            self.wfile.write(data)
            return
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

    def _read_json_body(self) -> dict[str, Any]:
        length = int(self.headers.get("Content-Length", "0") or 0)
        if length > 65536:
            length = 65536
        raw = self.rfile.read(length) if length > 0 else b"{}"
        try:
            out = json.loads(raw.decode("utf-8", errors="replace") or "{}")
        except (json.JSONDecodeError, UnicodeError):
            return {}
        return out if isinstance(out, dict) else {}

    def do_POST(self) -> None:  # noqa: N802
        path = urlparse(self.path).path
        if path == "/esp/calibration-begin":
            body = self._read_json_body()
            _bridge_log_post(path, body if body else None)
            with _LOCK:
                _ensure_glasses()
                _STATE["liveWeightDisplay"] = True
            self.send_response(204)
            self._cors()
            self.end_headers()
            return
        if path == "/esp/idle":
            length = int(self.headers.get("Content-Length", "0") or 0)
            if length > 0 and length <= 65536:
                raw = self.rfile.read(length)
                try:
                    body = json.loads(raw.decode("utf-8", errors="replace") or "{}")
                except (json.JSONDecodeError, UnicodeError):
                    body = {}
                _bridge_log_post(path, body if isinstance(body, dict) and body else None)
            else:
                _bridge_log_post(path, None)
            with _LOCK:
                _ensure_glasses()
                _STATE["liveWeightDisplay"] = False
                _STATE["bartenderState"] = "idle"
            self.send_response(204)
            self._cors()
            self.end_headers()
            return
        if path == "/esp/pour-ml":
            body = self._read_json_body()
            _bridge_log_post(path, body if body else None)
            ml = _as_float(body.get("pour_ml"))
            if ml is None:
                ml = _as_float(body.get("pourMl"))
            if ml is not None and 0 < ml <= 9999:
                with _LOCK:
                    _ensure_glasses()
                    _STATE["pourMl"] = float(ml)
            self.send_response(204)
            self._cors()
            self.end_headers()
            return
        if path == "/esp/apply-factor":
            body = self._read_json_body()
            f = _as_float(body.get("factor"))
            if f is None:
                f = _as_float(body.get("calibrationFactor"))
            if f is not None and f > 0:
                f = round(float(f), 6)
                body_out = {"factor": f}
                _bridge_log_post(path, body_out)
                with _LOCK:
                    _ensure_glasses()
                    _STATE["calibrationFactor"] = float(f)
            else:
                _bridge_log_post(path, body if body else None)
            self.send_response(204)
            self._cors()
            self.end_headers()
            return
        if path == "/esp/serve":
            body = self._read_json_body()
            _bridge_log_post(path, body if body else None)
            self.send_response(204)
            self._cors()
            self.end_headers()
            return
        if path == "/esp/serve-all":
            length = int(self.headers.get("Content-Length", "0") or 0)
            if length > 0 and length <= 65536:
                raw = self.rfile.read(length)
                try:
                    body = json.loads(raw.decode("utf-8", errors="replace") or "{}")
                except (json.JSONDecodeError, UnicodeError):
                    body = {}
                _bridge_log_post(path, body if isinstance(body, dict) and body else None)
            else:
                _bridge_log_post(path, None)
            self.send_response(204)
            self._cors()
            self.end_headers()
            return
        if path == "/esp/config":
            body = self._read_json_body()
            _bridge_log_post(path, body if body else None)
            with _LOCK:
                if "glassCount" in body and body["glassCount"] is not None:
                    nn = max(1, min(64, int(body["glassCount"])))
                    cur = list(_STATE.get("glasses") or [])
                    if nn > len(cur):
                        cur.extend({"tareG": None, "netG": None} for _ in range(nn - len(cur)))
                    else:
                        cur = cur[:nn]
                    _STATE["glassCount"] = nn
                    _STATE["glasses"] = cur
                _ensure_glasses()
                if "calibrationFactor" in body:
                    cf = _as_float(body.get("calibrationFactor"))
                    if cf is not None and cf > 0:
                        _STATE["calibrationFactor"] = float(cf)
                if "pourMl" in body:
                    pm = _as_float(body.get("pourMl"))
                    if pm is not None and pm > 0:
                        _STATE["pourMl"] = float(pm)
                for key in ("bartenderState", "softwareVersion", "commitHash", "author"):
                    if key in body:
                        _STATE[key] = body[key]
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
    raw = os.environ.get("BART_BRIDGE_PORT")
    if raw:
        try:
            ports.append(int(raw))
        except ValueError:
            pass
    ports.extend([9527, 9000, 9001, 9010, 8765, 8888])
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
    global _STATE  # noqa: PLW0603
    p = argparse.ArgumentParser(description="Bart device HTTP bridge for index.html (SSE + REST)")
    p.add_argument("-g", "--glasses", type=int, default=4, metavar="N", help="Number of glass slots (default 4)")
    p.add_argument("-f", "--factor", type=float, default=1.25, help="Calibration factor (default 1.25)")
    p.add_argument("--pour", type=float, default=150.0, help="Pour setpoint ml (default 150)")
    p.add_argument("--bind", default=os.environ.get("BART_BRIDGE_BIND", "127.0.0.1"))
    p.add_argument("-p", "--port", type=int, default=None, help="Listen port (default: first free among 9527, 9000, …)")
    args = p.parse_args()

    n = max(1, min(64, args.glasses))
    _STATE = _default_state(n, float(args.factor), float(args.pour))

    t = threading.Thread(target=_stdin_loop, name="bart-host-stdin", daemon=True)
    t.start()

    httpd, port = create_server(args.bind, args.port)
    with httpd:
        base = f"http://{args.bind}:{port}"
        print(f"Bart host: UI and API at {base}/")
        print(f"  Page {base}/   state {base}/esp/state   SSE {base}/esp/events")
        print("Stdin: place/net/pour commands update data pushed to the browser.")
        print("Ctrl+C to stop.\n")
        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            print()


if __name__ == "__main__":
    main()
