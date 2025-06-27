#pragma once

#ifdef BOARD_TTGO
#include "board_ttgo.h"
#endif

//TA8804
#define CONFIG_TUNER_FREQUENCY_START 1000
#define CONFIG_TUNER_FREQUENCY_STOP 1820
#define CONFIG_TUNER_FREQUENCY_STEP 10
#ifndef CONFIG_TUNER_PIN_VIDEO
#define CONFIG_TUNER_PIN_VIDEO 35
#endif
#ifndef CONFIG_TUNER_PIN_RSSI
#define CONFIG_TUNER_PIN_RSSI 34
#endif
#define RSSI_TUNER_ADDR_CLIBR 1
//RX5808
#ifndef CONFIG_VRX58_PIN_VIDEO
#define CONFIG_VRX58_PIN_VIDEO 36
#endif
#ifndef CONFIG_VRX58_RSSI
#define CONFIG_VRX58_RSSI 39
#endif
#define RSSI_VRX58_ADDR_CLIBR 0
//RX5808 SPI CONFIG
#ifndef CONFIG_VRX58_PIN_MOSI
#define CONFIG_VRX58_PIN_MOSI 4
#endif
#ifndef CONFIG_VRX58_PIN_CLK
#define CONFIG_VRX58_PIN_CLK 19
#endif
#ifndef CONFIG_VRX58_PIN_CS
#define CONFIG_VRX58_PIN_CS 18
#endif
//CONFIG


//REB
#define CONFIG_REB_ENABLE 0
#ifndef CONFIG_REB_PIN
#define CONFIG_REB_PIN -1
#endif
#define CONFIG_REB_TIME 30
#define CONFIG_REB_MIN_RSSI 30
//BUZZ
#ifndef CONFIG_BUZZER_PIN
#define CONFIG_BUZZER_PIN 25
#endif
#define CONFIG_BUZZER_ENABLE 1
#define CONFIG_BUZZER_ADDR 14

#ifndef CONFIG_BATT_PIN_CARGER
#define CONFIG_BATT_PIN_CARGER 26
#endif
#ifndef CONFIG_BATT_PIN
#define CONFIG_BATT_PIN 27
#endif
#ifndef CONFIG_BATT_PIN_R1
#define CONFIG_BATT_PIN_R1 6800
#endif
#ifndef CONFIG_BATT_PIN_R2
#define CONFIG_BATT_PIN_R2 2200
#endif
#ifndef CONFIG_BATT_PIN_VREF
#define CONFIG_BATT_PIN_VREF 3.64
#endif
//BATT 

//*************CD4066*START**************//
#ifndef CONFIG_CD4066_PIN_A
#define CONFIG_CD4066_PIN_A 33
#endif
#ifndef CONFIG_CD4066_PIN_B
#define CONFIG_CD4066_PIN_B 32
#endif
//*************CD4066*END**************//



//*************WIFI*START**************//
#define CONFIG_WIFI_SSID "SSD_FPV_DETECTOR"
#define CONFIG_WIFI_PASSWORD "12345678"
//*************WIFI*END**************//



//*************OLED*START**************//
#define CONFIG_OLED_1106 0
#define CONFIG_OLED_1306 1
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
//*************OLED*END**************//


// pin KEY
#ifndef CONFIG_KEY_PIN
#define CONFIG_KEY_PIN 14
#endif
#define CONFIG_KEY_ADDR_CLIBR_UP 10
#define CONFIG_KEY_ADDR_CLIBR_DOWN 11
#define CONFIG_KEY_ADDR_CLIBR_NEXT 12
#define CONFIG_KEY_ADDR_CLIBR_PREV 13
#define CONFIG_KEY_PLUS 20
//rssi 
#define CONFIG_RSSI_ADDR_CLIBR_12 20
#define CONFIG_RSSI_ADDR_CLIBR_58 21
