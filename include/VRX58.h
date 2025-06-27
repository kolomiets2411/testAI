#include <SPI.h>
#define BIT_BANG_FREQ                               10000
#define SYNTHESIZER_REG_A                           0x00
#define SYNTHESIZER_REG_B                           0x01
#define SYNTHESIZER_REG_C                           0x02
#define SYNTHESIZER_REG_D                           0x03
#define VCO_SWITCH_CAP_CONTROL_REGISTER             0x04
#define DFC_CONTROL_REGISTER                        0x05
#define SIXM_AUDIO_DEMODULATOR_CONTROL_REGISTER     0x06
#define SIXM5_AUDIO_DEMODULATOR_CONTROL_REGISTER    0x07
#define RECEIVER_CONTROL_REGISTER_1                 0x08
#define RECEIVER_CONTROL_REGISTER_2                 0x09
#define POWER_DOWN_CONTROL_REGISTER                 0x0A
#define STATE_REGISTER                              0x0F
#define RX5808_READ_CTRL_BIT                        0x00
#define RX5808_WRITE_CTRL_BIT                       0x01
#define RX5808_ADDRESS_R_W_LENGTH                   5
#define RX5808_DATA_LENGTH                          20
#define RX5808_PACKET_LENGTH                        25
class VRX58 {
  public:
    bool _isVideo58 = false;
    int _rssi = 0;
    int _rssi_min_1 = 0;
    int debug = 0;
    int _stops = 0;
    int _index_chanel;
    uint16_t _frequencyTable[136] = {
            4900,4921,4940,4958,4990,5020,5050,5060,
            5070,5080,5090,5100,5110,5120,5130,5140,
            5150,5160,5170,5180,5190,5200,5210,5220,
            5230,5240,5250,5260,5270,5280,5290,5300,
            5313,5325,5333,5348,5362,5366,5373,5384,
            5399,5402,5413,5420,5436,5438,5453,5456,
            5473,5474,5492,5493,5510,5510,5528,5533,
            5546,5547,5564,5573,5582,5584,5600,5613,
            5621,5633,5645,5653,5658,5665,5675,5685,
            5693,5695,5705,5715,5725,5732,5733,5733,
            5740,5745,5752,5760,5765,5769,5771,5773,
            5780,5785,5790,5800,5805,5806,5809,5813,
            5820,5825,5828,5840,5843,5845,5847,5853,
            5860,5865,5866,5880,5885,5893,5905,5917,
            5925,5933,5945,5955,5960,5965,5975,5980,
            5990,6000,6002,6020,6028,6030,6040,6050,
            6054,6060,6080,6106,6132,6158,6184
            };
    uint16_t _rssi_array[136] = {0};
    explicit VRX58(int PIN_MOSI,int PIN_CLK, int PIN_CS, int PIN_RSSI, int PIN_VIDEO)
        : _SPIModeEnabled(false), times2(0), _PIN_MOSI(PIN_MOSI), _PIN_CLK(PIN_CLK),
          _PIN_CS(PIN_CS), max_calc(0), min_calc(0), _pin(PIN_VIDEO),
          _rssi_min(0), _PIN_RSSI(PIN_RSSI) {
      _index_chanel = 0;
    }
    bool begin(int rssi_min){
      pinMode(_PIN_MOSI, INPUT);
      pinMode(_PIN_CLK, INPUT);
      pinMode(_PIN_CS, INPUT);
      pinMode (_PIN_RSSI, INPUT);
      pinMode(_pin, INPUT);
      EnableSPIMode();
      setChanel(0);
      _rssi_min = rssi_min;
      if(rtc6705readRegister(SYNTHESIZER_REG_B) == 0){
        return false;
      }else{
        begin_s = true;
        return true;
      }
    }

    int calibration(){
       int rs = 0;
       for (int i=0; i <= 512; i++){
          rs += analogRead(_PIN_RSSI);
          _index_chanel++;
          if(_index_chanel > 55){
            _index_chanel = 0;
          }
          setChanel(_index_chanel);
          delay(20);
       }
       _rssi_min = rs/512;
       return rs/512;
    }
    void loops(int REB){
      if(rtc6705readRegister(SYNTHESIZER_REG_B) == 0){
        _index_chanel = 0;
        return;
      }
      if(REB == 0){
        if(begin_s){
          if(!isVideo(70)){
            if(_stops == 0){
              _index_chanel++;
            }
            if(_index_chanel > 135){
              _index_chanel = 0;
            }
            setChanel(_index_chanel); 
          }
        }else{
//          Serial.println("Error set chenel");
        }
      }
    }
    bool isVideo(int delays){
      int rss = getRSSI();
      _rssi_array[_index_chanel] = rss;
      _rssi = rss;
      if(rss > _rssi_min_1){
        float dat = calc(_pin, delays);
        //Serial.println(dat);
        if(dat > 14000 && dat < 18000){
           times2 = millis();
          _isVideo58 = true;
          return true;
        }else{
          if(millis() - times2  > 3000){
           _isVideo58 = false;
           return false; 
          }else{
            _isVideo58 = true;
            return true;
          }
        } 
      }else{
        _isVideo58 = false;
        return false;
      }
    }
    float calc(int pin,int delays){
      unsigned long times = millis();
      int d = 1;
      int newVal = 0;
      float dat = 0;
      int oldVal = 0;
      int i = 0;
      while (d){
        oldVal = pulseIn(_pin,HIGH,80) + pulseIn(_pin,LOW,80);
        if(oldVal > 0){
          if((1000000/oldVal) > 15000 && (1000000/oldVal) < 18000){
                dat += 1000000/oldVal;
                //Serial.println(1000000/oldVal);
                i++;
          }
        }
       if(millis() - times  >= delays){
          d = 0;
        }
      }
      if(i <= 0){
        i = 1;
      }
       return dat/i;
    }
    int getRSSI(){
       int r = 0;
       for (int i = 0; i < 10; i++) {
        r += analogRead(_PIN_RSSI);
        delayMicroseconds(70);
      }
      int rssi = map(r/10, _rssi_min, 1400, 0, 100);
      if(rssi > 1){
        if(rssi <= 100){
          return rssi;  
        }else{
          return 100; 
        }
      }else{
        return 0;
      }
    }
    void setChanel(uint8_t index){
        _index_chanel = index;
        uint16_t f = _frequencyTable[index];
        uint32_t data = ((((f - 479) / 2) / 32) << 7) | (((f - 479) / 2) % 32);
        uint32_t newRegisterData = SYNTHESIZER_REG_B  | (RX5808_WRITE_CTRL_BIT << 4) | (data << 5);
        uint32_t currentRegisterData = SYNTHESIZER_REG_B | (RX5808_WRITE_CTRL_BIT << 4) | rtc6705readRegister(SYNTHESIZER_REG_B);
        if (newRegisterData != currentRegisterData)
        {
            rtc6705WriteRegister(newRegisterData);
        }
    }
    uint32_t getChanel(){
   
      return _frequencyTable[_index_chanel];
    }
    void nextChanel(){
      _isVideo58 = false;
      _index_chanel++;
      if(_index_chanel > 135){
        _index_chanel = 0;
      }
      setChanel(_index_chanel);
    }
    void prevChanel(){
      _isVideo58 = false;
      _index_chanel--;
      if(_index_chanel < 0){
        _index_chanel = 135;
      }
      setChanel(_index_chanel);
    }
  // список членов, доступных в программе
  private:
      uint32_t rtc6705readRegister(uint8_t readRegister){
        if (!_SPIModeEnabled) 
        {
            EnableSPIMode();
        }
    
        uint32_t buf = readRegister | (RX5808_READ_CTRL_BIT << 4);
        uint32_t registerData = 0;
    
        uint32_t periodMicroSec = 1000000 / BIT_BANG_FREQ;
    
        digitalWrite(_PIN_CS, LOW);
        delayMicroseconds(periodMicroSec);
    
        // Write register address and read bit
        for (uint8_t i = 0; i < RX5808_ADDRESS_R_W_LENGTH; ++i)
        {
            digitalWrite(_PIN_CLK, LOW);
            delayMicroseconds(periodMicroSec / 4);
            digitalWrite(_PIN_MOSI, buf & 0x01);
            delayMicroseconds(periodMicroSec / 4);
            digitalWrite(_PIN_CLK, HIGH);
            delayMicroseconds(periodMicroSec / 2);
    
            buf >>= 1; 
        }
    
        // Change pin from output to input
        pinMode(_PIN_MOSI, INPUT);
    
        // Read data 20 bits
        for (uint8_t i = 0; i < RX5808_DATA_LENGTH; i++)
        {
            digitalWrite(_PIN_CLK, LOW);
            delayMicroseconds(periodMicroSec / 4);
    
            if (digitalRead(_PIN_MOSI))
            {
                registerData = registerData | (1 << (5 + i));
            }
    
            delayMicroseconds(periodMicroSec / 4);
            digitalWrite(_PIN_CLK, HIGH);
            delayMicroseconds(periodMicroSec / 2);
        }
    
        // Change pin back to output
        pinMode(_PIN_MOSI, OUTPUT);
    
        digitalWrite(_PIN_MOSI, LOW);
        digitalWrite(_PIN_CLK, LOW);
        digitalWrite(_PIN_CS, HIGH);
    
        return registerData;
    }
      void rtc6705WriteRegister(uint32_t buf){
          if (!_SPIModeEnabled) 
          {
              EnableSPIMode();
          }      
          uint32_t periodMicroSec = 1000000 / BIT_BANG_FREQ;
      
          digitalWrite(_PIN_CS, LOW);
          delayMicroseconds(periodMicroSec);
      
          for (uint8_t i = 0; i < RX5808_PACKET_LENGTH; ++i)
          {
              digitalWrite(_PIN_CLK, LOW);
              delayMicroseconds(periodMicroSec / 4);
              digitalWrite(_PIN_MOSI, buf & 0x01);
              delayMicroseconds(periodMicroSec / 4);
              digitalWrite(_PIN_CLK, HIGH);
              delayMicroseconds(periodMicroSec / 2);
      
              buf >>= 1; 
          }
      
          digitalWrite(_PIN_CLK, LOW);
          delayMicroseconds(periodMicroSec);
          digitalWrite(_PIN_MOSI, LOW);
          digitalWrite(_PIN_CLK, LOW);
          digitalWrite(_PIN_CS, HIGH);
      }
      void EnableSPIMode(){
          pinMode(_PIN_MOSI, OUTPUT);
          pinMode(_PIN_CLK, OUTPUT);
          pinMode(_PIN_CS, OUTPUT);
      
          digitalWrite(_PIN_MOSI, LOW);
          digitalWrite(_PIN_CLK, LOW);
          digitalWrite(_PIN_CS, HIGH);
      
          _SPIModeEnabled = true;
        }
        bool _SPIModeEnabled = false;
        unsigned long times2 = 0;
        int _PIN_MOSI = 23;
        int _PIN_CLK = 18;
        int _PIN_CS = 32;
        int max_calc;
        int min_calc;
        int _pin = 26;
        int _rssi_min = 0;
        int _PIN_RSSI = 0;
        bool begin_s = false;     
  // список членов для использования внутри класса
};
