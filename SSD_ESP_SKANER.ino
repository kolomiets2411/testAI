#include <Wire.h>
#include <EEPROM.h>
#include "key.h"
#include "config.h"
#include "display_1306.h"
#include "TA8804.h"
#include "VRX58.h"
#include "wifiupdate.h"
display_1306 D1306(50);
key key(CONFIG_KEY_PIN);
wifiupdate wifiupdate(1);
TA8804 tuner(CONFIG_TUNER_PIN_VIDEO,CONFIG_TUNER_PIN_RSSI);
VRX58 vrx5808(CONFIG_VRX58_PIN_MOSI,CONFIG_VRX58_PIN_CLK,CONFIG_VRX58_PIN_CS,CONFIG_VRX58_RSSI,CONFIG_VRX58_PIN_VIDEO);
unsigned long timer2 = 0;
unsigned long timer3 = 0;

unsigned long timer4 = 0;
unsigned long timer5 = 0;
unsigned long timer6 = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(CONFIG_CD4066_PIN_A,OUTPUT);
  pinMode(CONFIG_CD4066_PIN_B,OUTPUT);
  digitalWrite(CONFIG_CD4066_PIN_A,LOW);
  digitalWrite(CONFIG_CD4066_PIN_B,LOW);
  analogWrite(CONFIG_BUZZER_PIN, 0);
  EEPROM.begin(512);

  key.key_up = EEPROM.read(CONFIG_KEY_ADDR_CLIBR_UP)*16;
  key.key_down = EEPROM.read(CONFIG_KEY_ADDR_CLIBR_DOWN)*16;
  
  key.key_next = EEPROM.read(CONFIG_KEY_ADDR_CLIBR_NEXT)*16;
  key.key_prev = EEPROM.read(CONFIG_KEY_ADDR_CLIBR_PREV)*16;
  
  if(D1306.begin()){
    //если ошибка Дисплея
      int _rssi_min_tuner = EEPROM.read(RSSI_TUNER_ADDR_CLIBR)*3;
      if(!tuner.begin(_rssi_min_tuner)){
        //если ошибка тюнера
        D1306.viewsErrors("Error RX 8804",15,30,1);
        delay(5000);
      }else{
        if(tuner.is8804()){
          D1306.tuner = "TA8804F(0x61)";
        }
        if(tuner.is8012()){
          D1306.tuner = "DA8012AM(0x60)";
        }
      }
      int _rssi_min = EEPROM.read(RSSI_VRX58_ADDR_CLIBR)*3;
      if(!vrx5808.begin(_rssi_min)){
        D1306.viewsErrors("Error RX 5808",15,30,1);
        delay(5000);
      }else{
        D1306.VRX = "RX5808";
      }
      D1306.buzz = EEPROM.read(CONFIG_BUZZER_ADDR);
  }

  D1306.min_rssi12 = EEPROM.read(CONFIG_RSSI_ADDR_CLIBR_12);
  if(D1306.min_rssi12 == 255){
    D1306.min_rssi12 = 10;
  }
  D1306.min_rssi58 = EEPROM.read(CONFIG_RSSI_ADDR_CLIBR_58);
  if(D1306.min_rssi58 == 255){
    D1306.min_rssi58 = 10;
  }
}

void loop() {
  
  key.loops();
  tuner._rssi_min_1 = D1306.min_rssi12;
  vrx5808._rssi_min_1 = D1306.min_rssi58;
  tuner.loops(0);
  vrx5808.loops(0);
  D1306.mhz_58 = vrx5808.getChanel();
  D1306.RSSI_58 = vrx5808._rssi;
  D1306._rssi_array_12[int((tuner._fs - CONFIG_TUNER_FREQUENCY_START)/CONFIG_TUNER_FREQUENCY_STEP)] = tuner._rssi_c;
  D1306.mhz_12 = tuner._fs;
  D1306.RSSI_12 = tuner._rssi_c;
  D1306._rssi_array_58[vrx5808._index_chanel] = vrx5808._rssi;
  D1306.loops(key._menu1,key._menu2);
  
   if(vrx5808._isVideo58 || tuner._isVideo){
    //если сигнал от Дрона есть более 1сек Включаем БУЗЗЕР или реб 
    if(millis() - timer2 > 1000){
      D1306.isVideo = tuner._isVideo;
      D1306.isVideo58 = vrx5808._isVideo58;
      if(D1306.buzz == 1){
        analogWrite(CONFIG_BUZZER_PIN, 255); 
      }else{
        analogWrite(CONFIG_BUZZER_PIN, 0);
      }
    }
   }else{
    timer2 = millis();
    D1306.isVideo = tuner._isVideo;
    D1306.isVideo58 = vrx5808._isVideo58;
    analogWrite(CONFIG_BUZZER_PIN, 0);
   }
   if(vrx5808._isVideo58 == 1 && tuner._isVideo == 1){
    
   }else{
      if(millis() - timer2 > 2000){
       if(vrx5808._isVideo58){
          digitalWrite(CONFIG_CD4066_PIN_B,HIGH);
          digitalWrite(CONFIG_CD4066_PIN_A,LOW);
          if(key._menu1 == 0){
            key._menu2 = 1;
          }
       }else if(tuner._isVideo){
          digitalWrite(CONFIG_CD4066_PIN_B,LOW);
          digitalWrite(CONFIG_CD4066_PIN_A,HIGH);
          if(key._menu1 == 0){
            key._menu2 = 0;
          }
       }
      }
   }
   if(key._menu1 == 3 && key._menu2 == 1){
      if(D1306.buzz == 1){
          D1306.buzz = 0;
          EEPROM.write(CONFIG_BUZZER_ADDR, 0);
          EEPROM.commit();
      }else{
        EEPROM.write(CONFIG_BUZZER_ADDR, 1);
        EEPROM.commit();
        D1306.buzz = 1;
      }
      key._menu2 = 0;
  }
  if(key._menu1 == 5 && key._menu2 == 1){
      if(D1306.wifi == 1){
          D1306.wifi = 0;
      }else{
        D1306.wifi = 1;
      }
      key._menu2 = 0;
   }

   if(key._menu1 == 0 && key._menu2 == 0 && key._next == 1){
    tuner.nextChanel();
    key._next = 0;
   }
   if(key._menu1 == 0 && key._menu2 == 0 && key._old == 1){
    tuner.prevChanel();
    key._old = 0;
   }

   if(key._menu1 == 6 && key._menu2 == 0){
    if(analogRead(CONFIG_KEY_PIN) > key.key_prev - CONFIG_KEY_PLUS && analogRead(CONFIG_KEY_PIN) < key.key_prev + CONFIG_KEY_PLUS){
        
        D1306.min_rssi12--;
        if(D1306.min_rssi12 < 1){
          D1306.min_rssi12 = 1;
        }
    }
    if(analogRead(CONFIG_KEY_PIN) > key.key_next - CONFIG_KEY_PLUS && analogRead(CONFIG_KEY_PIN) < key.key_next + CONFIG_KEY_PLUS){
        D1306.min_rssi12++;
        if(D1306.min_rssi12 > 100){
          D1306.min_rssi12 = 100;
        }
    }
   }else if(key._menu1 == 6 && key._menu2 == 1){
    if(analogRead(CONFIG_KEY_PIN) > key.key_prev - CONFIG_KEY_PLUS && analogRead(CONFIG_KEY_PIN) < key.key_prev + CONFIG_KEY_PLUS){
      D1306.min_rssi58--;
      if(D1306.min_rssi58 < 1){
          D1306.min_rssi58 = 1;
        }
    }
    if(analogRead(CONFIG_KEY_PIN) > key.key_next - CONFIG_KEY_PLUS && analogRead(CONFIG_KEY_PIN) < key.key_next + CONFIG_KEY_PLUS){
      D1306.min_rssi58++;
      if(D1306.min_rssi58 > 100){
          D1306.min_rssi58 = 100;
        }
    }
   }
   if(key._menu1 == 6){
    if(EEPROM.read(CONFIG_RSSI_ADDR_CLIBR_12) != D1306.min_rssi12){
         EEPROM.write(CONFIG_RSSI_ADDR_CLIBR_12, D1306.min_rssi12);
         EEPROM.commit();
         delay(200);
    }
    if(EEPROM.read(CONFIG_RSSI_ADDR_CLIBR_58) != D1306.min_rssi58){
         EEPROM.write(CONFIG_RSSI_ADDR_CLIBR_58, D1306.min_rssi58);
         EEPROM.commit();
         delay(200);      
    }
   }
   if(key._menu1 == 0 && key._menu2 == 1 && key._next == 1){
      vrx5808.nextChanel();
      key._next = 0;
   }
   if(key._menu1 == 0 && key._menu2 == 1 && key._old == 1){
      vrx5808.prevChanel();
      key._old = 0;
   }
   button_calibr();
   if(analogRead(CONFIG_KEY_PIN) > key.key_prev - CONFIG_KEY_PLUS && analogRead(CONFIG_KEY_PIN) < key.key_prev + CONFIG_KEY_PLUS && key._menu1 == 0){
      if(millis() - timer4 > 800){
          if(key._menu2 == 0){
            tuner._stops = 1;
          }
           if(key._menu2 == 1){
            vrx5808._stops = 1;
          }

      }
   }else{
      timer4 = millis();
   }
   if(analogRead(CONFIG_KEY_PIN) > key.key_next - CONFIG_KEY_PLUS && analogRead(CONFIG_KEY_PIN) < key.key_next + CONFIG_KEY_PLUS && key._menu1 == 0){
      if(millis() - timer5 > 800){
          if(key._menu2 == 0){
            tuner._stops = 0;
          }
           if(key._menu2 == 1){
            vrx5808._stops = 0;
          }
      }
   }else{
      timer5 = millis();
   }
   if(analogRead(CONFIG_KEY_PIN) > key.key_next - CONFIG_KEY_PLUS && analogRead(CONFIG_KEY_PIN) < key.key_next + CONFIG_KEY_PLUS && key._menu1 == 1){
       Wire.beginTransmission(0x58);   // стучимся к синтезатору и передаем байты параметров
       Wire.write(0x01);
       Wire.write(0x00);
       bool error = Wire.endTransmission();
       if(error){
          Serial.println("Error 12");
       }
   }
   if(analogRead(CONFIG_KEY_PIN) > key.key_up - CONFIG_KEY_PLUS && analogRead(CONFIG_KEY_PIN) < key.key_up + CONFIG_KEY_PLUS && key._menu1 == 0){
    if(millis() - timer6 > 5000){
            D1306.calib_start();
            int newVal = tuner.calibration();
            EEPROM.write(RSSI_TUNER_ADDR_CLIBR, newVal/3);
            newVal = vrx5808.calibration();
            EEPROM.write(RSSI_VRX58_ADDR_CLIBR, newVal/3);
            key._menu2 = 0;
            bool bRc = EEPROM.commit();
            if(bRc){
                D1306.calib_finish(); 
            }
    }else{
      Serial.println("ELSE 5000 time: "+String(millis() - timer6));
    }
   }else{
      timer6 = millis();
   }
   wifiupdate.loops(D1306.wifi);
}



void button_calibr(){
  if(analogRead(CONFIG_KEY_PIN) > 4094){
      if(millis() - timer3 > 5000){
        int bt_up = 0;
        int bt_down = 0;
        int bt_prev = 0;
        int bt_next = 0;
        D1306.button_view("Press button up!");
        int d = 1;
        while (d){
          if(analogRead(CONFIG_KEY_PIN) > 300 && analogRead(CONFIG_KEY_PIN) < 4000){
            
            for (int i=0; i <= 30; i++){
              bt_up += analogRead(CONFIG_KEY_PIN);
            }
            bt_up = bt_up/31;
            D1306.button_view(String(bt_up));
            delay(3000);
            d = 0;
          }
        }
        d = 1;
        D1306.button_view("Press button down!");
        while (d){
          if(analogRead(CONFIG_KEY_PIN) > 300 && analogRead(CONFIG_KEY_PIN) < 4000){
            
            for (int i=0; i <= 30; i++){
              bt_down += analogRead(CONFIG_KEY_PIN);
            }
            bt_down = bt_down/31;
            D1306.button_view(String(bt_down));
            delay(3000);
            d = 0;
          }
        }
        d = 1;
        D1306.button_view("Press button next!");
        while (d){
          if(analogRead(CONFIG_KEY_PIN) > 300 && analogRead(CONFIG_KEY_PIN) < 4000){
            
            for (int i=0; i <= 30; i++){
              bt_next += analogRead(CONFIG_KEY_PIN);
            }
            bt_next = bt_next/31;
            D1306.button_view(String(bt_next));
            delay(3000);
            d = 0;
          }
        }
        d = 1;
        D1306.button_view("Press button prev!");
        while (d){
          if(analogRead(CONFIG_KEY_PIN) > 300 && analogRead(CONFIG_KEY_PIN) < 4000){
            
            for (int i=0; i <= 30; i++){
              bt_prev += analogRead(CONFIG_KEY_PIN);
            }
            bt_prev = bt_prev/31;
            D1306.button_view(String(bt_prev));
            delay(3000);
            d = 0;
          }
        }
         EEPROM.write(CONFIG_KEY_ADDR_CLIBR_DOWN, bt_down/16);
         EEPROM.write(CONFIG_KEY_ADDR_CLIBR_UP, bt_up/16);
         EEPROM.write(CONFIG_KEY_ADDR_CLIBR_NEXT, bt_next/16);
         EEPROM.write(CONFIG_KEY_ADDR_CLIBR_PREV, bt_prev/16);
         bool bRc = EEPROM.commit();
         if(bRc){
            D1306.button_view("CALC FINISH!");
         }else{
            D1306.button_view("CALC ERROR SAVE !");
         }
         delay(3000);
         ESP.restart();
      }
   }else{
      timer3 = millis();
   }
}
