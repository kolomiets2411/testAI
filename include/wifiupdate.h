#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <DNSServer.h>
#include <Update.h>
#include <Ticker.h>
#include "html.h"
WebServer server(80);
Ticker tkSecond;
uint8_t otaDone = 0;
DNSServer dnsServer;

class wifiupdate {
  public:  
    wifiupdate(int s){
//      apMode();
//      webServerInit();
    }


  void loops(int state){
    _state = state;
    if(_state == 1){
      if(inits == 0){
          WiFi.disconnect(false);
          WiFi.mode(WIFI_AP);
          WiFi.softAP(CONFIG_WIFI_SSID, CONFIG_WIFI_PASSWORD);  
          MDNS.begin("SSD_FPV_SCANER"); 
          dnsServer.start(53, "*", WiFi.softAPIP());
          inits = 1;
          webServerInit();
      }
      dnsServer.processNextRequest();
      MDNS.update();
      server.handleClient();
    }else{
      inits = 0; 
      dnsServer.stop();
      WiFi.disconnect(true);
      WiFi.mode(WIFI_OFF);
    }
  }


  
  private:
   static void handleUpdateEnd() {
      server.sendHeader("Connection", "close");
      if (Update.hasError()) {
        server.send(502, "text/plain", Update.errorString());
      } else {
        server.sendHeader("Refresh", "10");
        server.sendHeader("Location", "/");
        server.send(307);
        ESP.restart();
      }
    }
    
    static void handleUpdate() {
      size_t fsize = UPDATE_SIZE_UNKNOWN;
      if (server.hasArg("size")) {
        fsize = server.arg("size").toInt();
      }
      HTTPUpload &upload = server.upload();
      if (upload.status == UPLOAD_FILE_START) {
        Serial.printf("Receiving Update: %s, Size: %d\n", upload.filename.c_str(), fsize);
        if (!Update.begin(fsize)) {
          otaDone = 0;
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_WRITE) {
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
          Update.printError(Serial);
        } else {
          otaDone = 100 * Update.progress() / Update.size();
        }
      } else if (upload.status == UPLOAD_FILE_END) {
        if (Update.end(true)) {
          Serial.printf("Update Success: %u bytes\nRebooting...\n", upload.totalSize);
        } else {
          Serial.printf("%s\n", Update.errorString());
          otaDone = 0;
        }
      }
    }
    
    void webServerInit() {
      server.on(
        "/update", HTTP_POST,
        []() {
          handleUpdateEnd();
        },
        []() {
          handleUpdate();
        }
      );
      server.on("/favicon.ico", HTTP_GET, []() {
        server.sendHeader("Content-Encoding", "gzip");
        server.send_P(200, "image/x-icon", favicon_ico_gz, favicon_ico_gz_len);
      });
      server.onNotFound([]() {
        server.send(200, "text/html", indexHtml);
      });
      server.begin();
      Serial.printf("Web Server ready at http://esp32.local or http://%s\n", WiFi.softAPIP().toString().c_str());
    }
    
    void everySecond() {
      if (otaDone > 1) {
        Serial.printf("ota: %d%%\n", otaDone);
      }
    }
  int inits = 0;
  int _state = 0;
};
