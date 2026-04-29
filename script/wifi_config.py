# Check if the wifi_config.h file is valid
# Create a default wifi_config.h file if it doesn't exist

import os
import re

ROOT_DIR = os.path.abspath(os.curdir)
TARGET_FILE = os.path.join(ROOT_DIR, 'include/wifi_config.h')
UID_FILE = os.path.join(ROOT_DIR, 'include/device_id.h')

RED     = '\033[0;31m'
GREEN   = '\033[0;32m'
YELLOW  = '\033[0;33m'
END     = '\033[0m'

def VerfiyConfigFile():
    if(not os.path.exists(TARGET_FILE)):
        return False
    try:
        success = True
        with open(TARGET_FILE, 'r') as file:
            content = file.read()
            if(not re.search(r'^(?!\s*//).*#define\s+WIFI_MODE\s+_\w+', content, re.MULTILINE)):
                print(f"WIFI_MODE\t\t{RED}undefined{END}")
                success = False
            if(not re.search(r'^(?!\s*//).*#define\s+SSID\s+"\w+"', content, re.MULTILINE)):
                print(f"SSID\t\t\t{RED}undefined{END}")
                success = False
            if(not re.search(r'^(?!\s*//).*#define\s+PASSWORD\s+"\w+"', content, re.MULTILINE)):
                print(f"PASSWORD\t\t{RED}undefined{END}")
                success = False
            if(not re.search(r'^(?!\s*//).*#define\s+DNS_NAME\s+"\w+"', content, re.MULTILINE)):
                print(f"DNS_NAME\t\t{RED}undefined{END}")
                success = False
    except Exception as e:
        print(f"{RED}Error reading wifi_config.h: {e}{END}")
        return False
    return success

def CreateDefaultConfigFile():
    print(f"\n{YELLOW}Creating default wifi_config.h file{END}")

    with open(TARGET_FILE, 'w') as file:
        file.write('/* DEFAULT CONFIG FILE */\n')
        file.write('#pragma once\n')
        file.write('#include "device_id.h"\n\n')
        file.write('#define _AP  0\n')
        file.write('#define _STA 1\n\n')

        file.write('#define WIFI_MODE _AP\n')
        file.write('#if WIFI_MODE == _AP\n')
        file.write('    #ifdef UNIQUE_ID\n')
        file.write('        #define SSID        "UnifiedFeeder" UNIQUE_ID\n')
        file.write('    #else\n')
        file.write('        #define SSID        "UnifiedFeeder"\n')
        file.write('    #endif\n')
        file.write('    #define PASSWORD        "Nowy149300" // Min 8 chars\n')
        file.write('#elif WIFI_MODE == _STA\n')
        file.write('    #define SSID            ""\n')
        file.write('    #define PASSWORD        ""\n')
        file.write('#else\n')
        file.write('    #error "WiFi mode is not defined!"\n')
        file.write('#endif\n\n')

        file.write('#define DNS_NAME            "UnifiedFeeder"\n\n')

        file.write('#define OTA_PASSWORD_ENABLE 0\n')
        file.write('#if OTA_PASSWORD_ENABLE == 1\n')
        file.write('    #define OTA_SSID        "UnifiedFeeder"\n')
        file.write('    #define OTA_PASSWORD    "Nowy149300"\n')
        file.write('#else\n')
        file.write('    #define OTA_SSID        ""\n')
        file.write('    #define OTA_PASSWORD    ""\n')
        file.write('#endif')


def OutputConfigFile(color):
    with open(TARGET_FILE, 'r') as file:
        content = file.read()
        lines = content.split('\n')

        # Extract WiFi mode first
        wifi_mode_match = re.search(r'#define WIFI_MODE\s+_(\w+)', content)
        if wifi_mode_match:
            wifi_mode = wifi_mode_match.group(1)
            print(f"WIFI_MODE:\t\t{color}{wifi_mode}{END}")
        else:
            print(f"WIFI_MODE:\t\t{color}UNKNOWN{END}")
            return

        # Try to read device_id.h to resolve UNIQUE_ID
        unique_id_value = None
        if os.path.exists(UID_FILE):
            try:
                with open(UID_FILE, 'r') as f:
                    device_id_content = f.read()
                    unique_id_match = re.search(r'^(?!\s*//).*#define\s+UNIQUE_ID\s+"([^"]+)"', device_id_content, re.MULTILINE)
                    if unique_id_match:
                        unique_id_value = unique_id_match.group(1)
            except:
                pass

        # Extract SSID and PASSWORD based on the active mode
        if wifi_mode == "AP":
            ssid_match = re.search(r'#if WIFI_MODE == _AP\s*\n\s*#ifdef UNIQUE_ID\s*\n\s*#define SSID\s+"([^"]+)"\s+(\w+)', content, re.MULTILINE)
            if not ssid_match:
                ssid_match = re.search(r'#if WIFI_MODE == _AP\s*\n\s*(?:#ifdef UNIQUE_ID\s*\n\s*#define SSID\s+"[^"]+"\s+\w+\s*\n\s*#else\s*\n\s*)?#define SSID\s+"([^"]+)"', content, re.MULTILINE)
            password_match = re.search(r'#if WIFI_MODE == _AP.*?#define PASSWORD\s+"([^"]+)"', content, re.MULTILINE | re.DOTALL)
        elif wifi_mode == "STA":
            ssid_match = re.search(r'#elif WIFI_MODE == _STA\s*\n\s*#define SSID\s+"([^"]+)"', content, re.MULTILINE)
            password_match = re.search(r'#elif WIFI_MODE == _STA\s*\n\s*#define SSID\s+"[^"]+"\s*\n\s*#define PASSWORD\s+"([^"]+)"', content, re.MULTILINE)
        else:
            print(f"Unknown WiFi mode: {wifi_mode}")
            return

        if ssid_match:
            ssid_base = ssid_match.group(1)
            if ssid_match.lastindex >= 2 and ssid_match.group(2) == "UNIQUE_ID" and unique_id_value:
                print(f"SSID:\t\t\t{color}{ssid_base}{unique_id_value}{END}")
            else:
                print(f"SSID:\t\t\t{color}{ssid_base}{END}")
        else:
            print(f"SSID:\t\t\t{color}NOT FOUND{END}")

        if password_match:
            print(f"PASSWORD:\t\t{color}{password_match.group(1)}{END}")
        else:
            print(f"PASSWORD:\t\t{color}NOT FOUND{END}")

        dns_match = re.search(r'#define DNS_NAME\s+"([^"]+)"', content)
        if dns_match:
            print(f"DNS_NAME:\t\t{color}{dns_match.group(1)}.local/{END}")

        ota_enable_match = re.search(r'#define OTA_PASSWORD_ENABLE\s+(\d+)', content)
        if ota_enable_match and ota_enable_match.group(1) == '1':
            ota_ssid_match = re.search(r'#define OTA_SSID\s+"([^"]+)"', content)
            if ota_ssid_match:
                print(f"OTA_SSID:\t\t{color}{ota_ssid_match.group(1)}{END}")

            ota_password_match = re.search(r'#define OTA_PASSWORD\s+"([^"]+)"', content)
            if ota_password_match:
                print(f"OTA_PASSWORD:\t\t{color}{ota_password_match.group(1)}{END}")

def RunScript():
    if(not VerfiyConfigFile()):
        CreateDefaultConfigFile()
        OutputConfigFile(YELLOW)
    else:
        OutputConfigFile(GREEN)

RunScript()