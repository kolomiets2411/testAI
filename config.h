//TA8804
#define CONFIG_TUNER_FREQUENCY_START 1000
#define CONFIG_TUNER_FREQUENCY_STOP 1820
#define CONFIG_TUNER_FREQUENCY_STEP 10
#ifdef BOARD_TTGO
#include "board_ttgo.h"
#else
#define CONFIG_TUNER_PIN_VIDEO 35
#define CONFIG_TUNER_PIN_RSSI 34
#define CONFIG_VRX58_PIN_VIDEO 36
#define CONFIG_VRX58_RSSI 39
#define CONFIG_VRX58_PIN_MOSI 4
#define CONFIG_VRX58_PIN_CLK 19
#define CONFIG_VRX58_PIN_CS 18
#define CONFIG_BUZZER_PIN 25
#define CONFIG_BATT_PIN_CARGER 26
#define CONFIG_BATT_PIN 27
#define CONFIG_CD4066_PIN_A 33
#define CONFIG_CD4066_PIN_B 32
#define CONFIG_KEY_PIN 14
#endif
#define RSSI_TUNER_ADDR_CLIBR 1
//RX5808
#define RSSI_VRX58_ADDR_CLIBR 0
//CONFIG


//REB
#define CONFIG_REB_ENABLE 0
#define CONFIG_REB_PIN -1
#define CONFIG_REB_TIME 30
#define CONFIG_REB_MIN_RSSI 30
//BUZZ
#define CONFIG_BUZZER_ENABLE 1
#define CONFIG_BUZZER_ADDR 14

#define CONFIG_BATT_PIN_R1 6800
#define CONFIG_BATT_PIN_R2 2200
#define CONFIG_BATT_PIN_VREF 3.64
//BATT 

//*************CD4066*START**************//
// pins defined in board configuration
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
#define CONFIG_DISPLAY_ROTATION 0 // 0-3 for Adafruit displays
//*************OLED*END**************//


// pin KEY
#define CONFIG_KEY_ADDR_CLIBR_UP 10
#define CONFIG_KEY_ADDR_CLIBR_DOWN 11
#define CONFIG_KEY_ADDR_CLIBR_NEXT 12
#define CONFIG_KEY_ADDR_CLIBR_PREV 13
#define CONFIG_KEY_PLUS 20
//rssi 
#define CONFIG_RSSI_ADDR_CLIBR_12 20
#define CONFIG_RSSI_ADDR_CLIBR_58 21
