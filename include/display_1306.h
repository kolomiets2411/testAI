#include <Adafruit_GFX.h>
#if (CONFIG_OLED_1306 == 1)
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#endif
#if (CONFIG_OLED_1106 == 1)
#include <Adafruit_SH110X.h>
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#endif

#ifdef SSD1306_SWITCHCAPVCC

#else
#define  SSD1306_SWITCHCAPVCC false
#endif

#ifdef SSD1306_WHITE
  
#else
#define  SSD1306_WHITE SH110X_WHITE
#endif

class display_1306 {
  public:
    int _delays = 50;
    int min_rssi12 = 10;
    int min_rssi58 = 10;
    int display_ls = 0;
    int display_av = 1;
    String tuner = "Error";
    String VRX = "Error";
    int RSSI_58 = 0;
    int isVideo = 0;
    int s = 0;
    int wifi = 0;
    int buzz = 1;
    int isVideo58 = 0;
    uint16_t _rssi_array_58[56] = {
             0,0,0,0,0,0,0,0,
             0,0,0,0,0,0,0,0,
             0,0,0,0,0,0,0,0,
             0,0,0,0,0,0,0,0,
             0,0,0,0,0,0,0,0
      };
    uint16_t _rssi_array_12[68] = {
             0  
      };
    int RSSI_12 = 0;
    int mhz_58 = 0;
    int mhz_12 = 0;
    explicit display_1306(int delays) : _delays(delays), _timing(0), _timing3(0) {

    }
    bool begin(){
      bool str = false;
      if(CONFIG_OLED_1106 == 1){
        str = display.begin(SCREEN_ADDRESS, true);
      }
      if(CONFIG_OLED_1306 == 1 || SSD1306_SWITCHCAPVCC){
        str = display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
      }
      display.setRotation(CONFIG_DISPLAY_ROTATION);
      pinMode(CONFIG_BATT_PIN,INPUT_PULLDOWN);
      if(!str) {
        return false;
      }else{
            display.clearDisplay();
            viewText("SLAVA UKRAINE",20,20,1);
            viewText("Powered by Fizik",15,30,1);
            delay(3000);
            display.display();
            return true;
      }
      _timing = millis();
    }
    void viewsErrors(String text,int starts,int stops, int sizes){
      display.clearDisplay();
      viewText(text,15,30,1);
      display.display();
    }
    void loops(int menu1, int menu2){
      if (millis() - _timing > _delays){
        display.clearDisplay();
        _timing = millis();
         for (int i=-1; i <= 122; i++){
            viewText(".",i,15,1);
         }
         switch (menu1) {
            case 0:
                d0(menu2);
              break;
            case 1:
              d1(menu2);
              break;
            case 2:
              d2(menu2);
              break;
            case 3:
              d3(menu2);
              break;
            case 4:
              d4(menu2);
              break;
            case 5:
              d5(menu2);
              break;
            case 6:
              d6(menu2);
              break;
            case 7:
              d7(menu2);
              break;
         }
         display.display();
      }
    }
    void d0_1(int menu2){
      viewText("Calibration min RSSI",0,37,1);
    }
    void d0(int menu2){
        if(millis() - _timing3 > 2000){
            if(s){
              s = 0;
            }else{
              s = 1;
            }
          _timing3 = millis();
        }
        if(s){
            viewText("1.2G",5,0,2);
            viewText("5.8G",70,0,2); 
        }else{
          
          viewText("BATT:"+String(getBattProcent())+"%",0,0,2);
        }
        if(menu2 == 1){
          digitalWrite(CONFIG_CD4066_PIN_B,HIGH);
          digitalWrite(CONFIG_CD4066_PIN_A,LOW);
          viewText(String(mhz_12)+"mhz",10,25,1);
          viewText(String(mhz_58)+"mhz*",70,25,1);
        }else{
          digitalWrite(CONFIG_CD4066_PIN_B,LOW);
          digitalWrite(CONFIG_CD4066_PIN_A,HIGH);
          viewText(String(mhz_12)+"mhz*",10,25,1);
          viewText(String(mhz_58)+"mhz",70,25,1);
        }
        viewText("RSSI:"+String(RSSI_58)+"%",70,37,1);
        viewText("RSSI:"+String(RSSI_12)+"%",5,37,1);
        for (int i=17; i <= 64; i++){
            viewText(".",60,i,1);
        }
        if(isVideo){
          viewText("FPV",10,50,2); 
        }
        if(isVideo58){
          viewText("FPV",70,50,2); 
        }
    }
    void d1(int menu2){
      //header
      viewText("BAND",0,0,2);
      viewText("SCANER",56,0,2);
      for (int i=-1; i <= 128; i++){
            viewText(".",i,47,1);
      }



      
      for (int ii=0; ii <= 55; ii++){
        if(_rssi_array_58[ii] > 0){
            for (int i=0; i <= map(_rssi_array_58[ii], 0, 100, 0, 30); i++){
                viewText(".",(ii+5),(45-i),1);
            }
        }else{
              viewText(".",(ii),45,1);
        }
      }
      



      //Footer
      for (int i=47; i <= 64; i++){
            viewText(".",91,i,1);
      }
      for (int i=47; i <= 64; i++){
            viewText(".",25,i,1);
      }
     
      viewText("4990",0,57,1);
      viewText("5917",100,57,1);
    }
    void d2(int menu2){
            viewText("BAND",0,0,2);
      viewText("SCANER",56,0,2);
      for (int i=-1; i <= 128; i++){
            viewText(".",i,47,1);
      }
      
      for (int ii=0; ii <= 67; ii++){  
        if(_rssi_array_12[ii] > 0){
            for (int i=0; i <= map(_rssi_array_12[ii], 0, 100, 0, 30); i++){
                viewText(".",(ii+5),(45-i),1);
            }
        }else{
              viewText(".",(ii),45,1);
        }
      }
      



      //Footer
      for (int i=47; i <= 64; i++){
            viewText(".",91,i,1);
      }
      for (int i=47; i <= 64; i++){
            viewText(".",25,i,1);
      }
     
      viewText(String(CONFIG_TUNER_FREQUENCY_START),0,57,1);
      viewText(String(CONFIG_TUNER_FREQUENCY_STOP ),100,57,1);
    }
    void d3(int menu2){
      viewText("BUZZER",25,0,2);

      if(buzz){
        viewText("BUZZ: ON",0,37,2);
      }else{
     
        viewText("BUZZ: OFF",0,37,2); 
      }
    }
    void d4(int menu2){
      viewText("Battery",20,0,2);
      if(menu2 == 0){
        viewText("Battery:"+String(getBattProcent())+"%",0,26,1);
        if(analogRead(CONFIG_BATT_PIN_CARGER) > 1000){
         viewText("Battery: Charger",0,37,1); 
        }else{
          viewText("Battery: discharge",0,37,1); 
        }
      }else{
        viewText("Voltage bat:"+String(getVoltagerBat())+"V",0,26,1);
        viewText("Voltage Charger:"+String((((analogRead(CONFIG_BATT_PIN_CARGER))*3.3)/4095)*2)+"V",0,36,1);
      }
    }
    void d5(int menu2){
      viewText("WIFI",35,0,2);
      
      if(wifi){
        viewText("Wifi: ON",0,25,1); 
      }else{
        viewText("Wifi: OFF",0,25,1); 
      }
      viewText("SSID:"+String(CONFIG_WIFI_SSID),0,35,1);
      viewText("PASS:"+String(CONFIG_WIFI_PASSWORD),0,45,1);
      viewText("IP:"+String("192.168.4.1"),0,55,1);
    }
    void d6(int menu2){
      for (int i=17; i <= 64; i++){
        viewText(".",60,i,1);
      }
      viewText("MIN RSSI",15,0,2);
      if(menu2 == 0){
        viewText("1.2*",5,25,2);
        viewText("5.8",70,25,2); 
      }else{
        viewText("1.2",5,25,2);
        viewText("5.8*",70,25,2);  
      }
      viewText(String(min_rssi12)+"%",7,43,2);
      viewText(String(min_rssi58)+"%",75,43,2);
    }
    void d7(int menu2){
      viewText("ABOUT",35,0,2);
      viewText("Version:"+String(1.361),0,26,1);
      if(menu2 == 0){
        viewText("RX1.2G:"+tuner,0,36,1);
        viewText("RX5.8G:"+VRX,0,46,1);
      }
    }
    void calib_start(){
      display.clearDisplay();
      viewText("Calibration",10,32,1); 
      display.display();
    }
     void calib_finish(){
      display.clearDisplay();
      viewText("Calibration finish",10,32,1); 
      display.display();
      delay(3000);
    }
    int getBattProcent(){
      float vbat = (getVoltagerBat()-5.0)/0.034;
      if(vbat > 100){
        return 100;
      }
      if(vbat < 0){
        return 0;
      }
      return int(vbat);
    }
    float getVoltagerBat(){
      int r = 0;
      int interation = 0;
      for (int i = 0; i < 10; i++) {
        r += analogRead(CONFIG_BATT_PIN);
        delayMicroseconds(10);
        interation++;
      }
      return  (float)(r/interation) * CONFIG_BATT_PIN_VREF * ((CONFIG_BATT_PIN_R1 + CONFIG_BATT_PIN_R2) / CONFIG_BATT_PIN_R2) / 4096;
    }
    void button_view(String s){
      display.clearDisplay();
      viewText(s,10,32,1); 
      display.display();
      delay(3000);
    }
    private:
      void viewText(String str,int starts,int stops, int sizes){
          display.setTextSize(sizes);             // Normal 1:1 pixel scale
          display.setTextColor(SSD1306_WHITE);        // Draw white text
          display.setCursor(starts,stops);             // Start at top-left corner
          display.println(str);
      } 
      int status_disp = 0;
      unsigned long _timing;
      unsigned long _timing3;
};
