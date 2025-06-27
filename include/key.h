#include "config.h"
class key {
  public:
    int _menu1 = 0;
    int _menu2 = 0;
    int _next = 0;
    int _old = 0;
    int _calibr = 0;

    int key_up = 0;
    int key_down = 0;
    int key_next = 0;
    int key_prev = 0;
    int _input = 0;
    
    
    explicit key(int key1) : _key1(key1), _timing(0) {
      pinMode(_key1, INPUT);
    }
    int getKey_f(){
      int r = 0;
      int i_n = 0;
      for (int i = 0; i < 20; i++) {
        r += analogRead(_key1);
        delayMicroseconds(50);
        i_n++;
      }
      return r/i_n;
    }
    int getKeyDataRaw(){
      return 0;
    }
    void loops(){
      int input = getKey_f();
      if(input > 300){
        if(!isDubul){
          
          // up
          if(input > key_up - CONFIG_KEY_PLUS && input < key_up + CONFIG_KEY_PLUS){
              _menu1 +=1;
              _menu2 = 0;
          }

          // OK
          if(input > 4000 && input < 4096){
              _menu2 += 1;
          }
          //calibr
          if(input > 1000 && input < 1050){
              _calibr += 1;
          }
          // Next
          if(input > key_next - CONFIG_KEY_PLUS && input < key_next + CONFIG_KEY_PLUS){
              _next = 1;
          }
          //prev
          if(input > key_prev - CONFIG_KEY_PLUS && input < key_prev + CONFIG_KEY_PLUS){
             _old = 1;
          }
          //down
          if(input > key_down - CONFIG_KEY_PLUS && input < key_down + CONFIG_KEY_PLUS){
              _menu1 -=1;
              _menu2 = 0;
          } 
        }
        isDubul = true;
      }else{
        isDubul = false;
      }
      if(_menu1 > 7){
          _menu1 = 0;
       }
       if(_menu1 < 0){
          _menu1 = 7 ;
       }
       if(_menu2 > 1){
          _menu2 = 0;
       }

    }
    private:
      int button3_set = 0;
      int button1_set = 0;
      int _key1 = 0;
      bool isDubul = false;
      unsigned long _timing;
};
