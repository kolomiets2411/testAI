class TA8804 {
  public:
    bool _isVideo = false;
    int _rssi_c = 0;
    int _rssi_min = 0;
    int _rssi_min_1 = 0;
    int _stops = 0;
    int debug = 0;
    int _fs = CONFIG_TUNER_FREQUENCY_START;
    int _index_chanel;
    uint16_t _frequencyTable[35] = {
      1010,1040,1060,1080,1100,1120,1140,1160,
      1180,1200,1220,1240,1258,1280,1300,1320,
      1340,1360,1380,1400,1430,1455,1480,1505,
      1530,1555,1580,1605,1630,1655,1680,1705,
      1730,1755,1780
    };
    explicit TA8804(int pin, int rssi)
        : _index_chanel(0), _Delitel(0), _DelitelH(0), _DelitelL(0),
          times2(0), _pin(pin), _rssi(rssi), _status(0), _addr(0x61) {

    }
    bool begin(int rssi_min){
        _rssi_min = rssi_min;
        pinMode (_pin, INPUT);
        pinMode (_rssi, INPUT);
        Wire.beginTransmission(_addr);
        int error = Wire.endTransmission();
        if(error == 0){
          return SetFrequency(_frequencyTable[0]);  
        }else{
          _addr = 0x60;
          return SetFrequency(_frequencyTable[0]); 
        }
    }
    int calibration(){
       int rs = 0;
       for (int i=0; i <= 512; i++){
          rs += analogRead(_rssi);
          
          if(_fs >= CONFIG_TUNER_FREQUENCY_STOP){
             _fs = CONFIG_TUNER_FREQUENCY_START; 
          }
          _fs += CONFIG_TUNER_FREQUENCY_STEP;
          SetFrequency(_frequencyTable[0]);
          delay(20);
       }
       _rssi_min = rs/512;
       return rs/512;
    }
    int getRSSI(){
      int r = 0;
      int i_n = 0;
      for (int i = 0; i < 10; i++) {
        r += analogRead(_rssi);
        delayMicroseconds(1);
        i_n++;
      }
      _rssi_c = map(r/i_n, _rssi_min, 1200, 0, 100);
      if(_rssi_c >= 12){
        if(_rssi_c >= 100){
          _rssi_c = 100;
          return 100;
        }else{
          return _rssi_c; 
        }
      }else{
       _rssi_c = 0;
       return 0; 
      }
    }
    
    bool isVideo(int delays){ 
      int rssi = getRSSI();
      if(rssi >= _rssi_min_1){
           float dat = calc(_pin, delays);
           
           if(dat > 14000 && dat < 47000){
            times2 = millis();
            _isVideo = true;
            return true;
          }else{
            if(millis() - times2  > 5000){
              return false;
            }else{
              _isVideo = true;
              return true;
            }
          } 
      }else{
           _isVideo = false;
           return false; 
      }
    }
    void setChanel(int fsw){
      if(!SetFrequency(fsw)){
        
      }
    }

    void nextChanel(){
       _index_chanel++;
       times2 += 5000;
       if(_index_chanel > 34 ){
            _index_chanel = 0;
           }
       if(!SetFrequency(_frequencyTable[_index_chanel])){
              _fs = _frequencyTable[_index_chanel];
       }
    }
     void prevChanel(){
       times2 += 5000;
       _index_chanel  = _index_chanel - 1;
       if(_index_chanel < 0){
        _index_chanel = 34;
       }
       if(!SetFrequency(_frequencyTable[_index_chanel])){
              _fs = _frequencyTable[_index_chanel];
       }
    }
    bool is8804(){
      if(_addr == 0x61){
        return true;
      }else{
        return false;
      }
    }
    bool is8012(){
      if(_addr == 0x60){
        return true;
      }else{
        return false;
      }
    }
    int GetFrequency(){
      return _fs;
    }
    void loops(int REB){
      if(_fs >= CONFIG_TUNER_FREQUENCY_STOP){
         _fs = CONFIG_TUNER_FREQUENCY_START; 
      }
      if(REB == 0){
        if(!isVideo(80)){
          if(_stops == 0){
           _index_chanel++; 
          }
           if(_index_chanel > 34 ){
            _index_chanel = 0;
           }
           if(!SetFrequency(_frequencyTable[_index_chanel])){
              _fs = _frequencyTable[_index_chanel];
           }
           _isVideo = false;
        }else{
          _isVideo = true;
        }
      }else{
        getRSSI(); 
      }
    }
  private:
     bool SetFrequency(uint16_t f){
        _fs = f;
        _Delitel=f*8+3836; //считаем делитель
        _DelitelH=_Delitel>>8;
        _DelitelL=_Delitel&0XFF;
       Wire.beginTransmission(_addr);   // стучимся к синтезатору и передаем байты параметров
       Wire.write(_DelitelH);
       Wire.write(_DelitelL);
       Wire.write(0xCE);
       Wire.write(0x00);
       bool error = Wire.endTransmission();
       if(error == 0){
          return true;
       }else{
          return false;
       }
    }
    float calc(int pin,int delays){
      unsigned long times = millis();
      int d = 1;
      int newVal = 0;
      int i = 0;
      int oldVal = 0;
      float dat = 0;
      while (d){
        oldVal = pulseIn(_pin,HIGH,80) + pulseIn(_pin,LOW,80);
        if(oldVal > 0){
          if((1000000/oldVal) > 15000 && (1000000/oldVal) < 18000){
                dat += 1000000/oldVal;
                i++;
          }
        }
        if(millis() - times  >= delays){
          d = 0;
        }
      }

      Serial.println(i);
      if((dat) > 0){
        return (dat/i);
      }else{
        return 0;
      }
    }
     unsigned int _Delitel;
     byte _DelitelH;
     byte _DelitelL;
     unsigned long times2 = 0;
     int _pin;
     int _rssi;
     int _status = 0;
     byte _addr = 0x61;
};
