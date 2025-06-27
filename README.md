# SSD ESP Skaner

This project contains a firmware sketch for ESP32 based FPV signal scanner. It supports TTGO style boards with an OLED display and allows Wi‑Fi OTA firmware updates.

## Required Libraries

The sketch depends on several Arduino libraries. Make sure the following packages are installed:

- **ESP32 core** for Arduino providing `<WiFi.h>`, `<WebServer.h>`, `<ESPmDNS.h>`, `<DNSServer.h>`, `<Update.h>` and `<Ticker.h>` used in the OTA implementation【F:include/wifiupdate.h†L1-L10】.
- **Adafruit GFX** and **Adafruit SSD1306** or **Adafruit SH110X** for the OLED display driver as referenced in `display_1306.h`【F:include/display_1306.h†L1-L9】.
- `<Wire.h>` and `<EEPROM.h>` which are part of the ESP32 Arduino core.

Install these libraries through the Arduino Library Manager or from the ESP32 board package.

## TTGO Wiring

Pin mappings for the TTGO board are defined in `board_ttgo.h` (mirrored below from `config.h` for reference):

| Function | Pin |
| -------- | --- |
| Tuner video | `35` |
| Tuner RSSI | `34` |
| RX5808 video | `36` |
| RX5808 RSSI | `39` |
| RX5808 MOSI | `4` |
| RX5808 CLK | `19` |
| RX5808 CS | `18` |
| CD4066 A | `33` |
| CD4066 B | `32` |
| Buzzer | `25` |
| Battery monitor | `27` |
| Charger detect | `26` |
| Key input | `14` |

The Wi‑Fi SSID and password default to `SSD_FPV_DETECTOR` / `12345678`【F:include/config.h†L44-L45】.

## Selecting the Board

The project expects a board header such as `board_ttgo.h` to set the pin mapping macros. Compile with `-D BOARD_TTGO` (or adjust the include) to use the TTGO definition. Other boards can be supported by providing similar header files with their own mappings.

## Building with PlatformIO

The firmware sources now follow the typical PlatformIO layout with implementation
files in `src/` and headers in `include/`. A ready to use
`platformio.ini` configuration is provided. After installing
[PlatformIO](https://platformio.org/) simply run:

```bash
pio run              # build the firmware
pio run --target upload  # build and upload to the board
```


## Wi‑Fi OTA Update

OTA updates are handled when Wi‑Fi mode is enabled. The device starts a soft AP and web server:

```cpp
WiFi.mode(WIFI_AP);
WiFi.softAP(CONFIG_WIFI_SSID, CONFIG_WIFI_PASSWORD);
```
【F:include/wifiupdate.h†L24-L31】

Once started the console prints the URL of the update page【F:include/wifiupdate.h†L100-L105】. Connect to the device Wi‑Fi network and open `http://<device ip>/update` to upload a new firmware binary.

## Calibration

### Button calibration

Holding the key input high for more than five seconds enters button calibration. The firmware then asks to press each button in turn and stores the ADC values to EEPROM【F:src/SSD_ESP_SKANER.ino†L256-L307】.

### RSSI thresholds

When on the main screen press and hold the **up** button for five seconds to start RSSI calibration. Minimum RSSI levels for both receivers are measured and written to EEPROM【F:src/SSD_ESP_SKANER.ino†L226-L255】. These thresholds are saved at addresses `CONFIG_RSSI_ADDR_CLIBR_12` and `CONFIG_RSSI_ADDR_CLIBR_58`【F:include/config.h†L68-L69】.

---
