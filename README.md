# SSD ESP Skaner

This repository contains an ESP32 based firmware for scanning FPV video signals. It was developed for TTGO style ESP32 boards equipped with an OLED display. The code can be built either in the Arduino IDE or with PlatformIO.

## Required Arduino/PlatformIO Libraries

Install the following libraries using the Arduino Library Manager or PlatformIO:

- **ESP32 Arduino core** (provides `WiFi.h`, `WebServer.h`, `ESPmDNS.h`, `DNSServer.h`, `Update.h` and `Ticker.h`)
- **Adafruit GFX** and **Adafruit SSD1306** (or **Adafruit SH110X** if using a SH1106 based display)
- `Wire` and `EEPROM` which are part of the ESP32 core

## Default Pin Mapping

The default pin layout for a TTGO board is defined in `config.h` and reproduced below:

| Function       | Pin |
| -------------- | --- |
| Tuner video    | `35` |
| Tuner RSSI     | `34` |
| RX5808 video   | `36` |
| RX5808 RSSI    | `39` |
| RX5808 MOSI    | `4`  |
| RX5808 CLK     | `19` |
| RX5808 CS      | `18` |
| CD4066 A       | `33` |
| CD4066 B       | `32` |
| Buzzer         | `25` |
| Battery monitor| `27` |
| Charger detect | `26` |
| Key input      | `14` |

The firmware starts a Wi‑Fi access point using the default credentials `SSD_FPV_DETECTOR` and `12345678`.

## Selecting the TTGO Configuration

Compile the project with the `BOARD_TTGO` macro defined to use the above pin assignments. In PlatformIO this can be done with:

```ini
build_flags = -D BOARD_TTGO
```

or pass `-D BOARD_TTGO` via the Arduino IDE's build options.

## Calibrating Buttons and RSSI

### Button Calibration

Hold the key input for more than five seconds to enter the button calibration routine. Follow the on‑screen instructions to press each button. The measured ADC values are saved to EEPROM so they persist after reboot.

### RSSI Calibration

From the main screen hold the **up** button for around five seconds. The tuner measures the noise floor on both receivers and stores the minimum RSSI thresholds to EEPROM.

## OTA Update Page

When Wi‑Fi mode is enabled, the device starts a simple web server. After connecting to the `SSD_FPV_DETECTOR` network open:

```
http://<device ip>/update
```

Upload a compiled binary via this page to update the firmware over the air.

