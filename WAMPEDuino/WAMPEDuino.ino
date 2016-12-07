#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include "WAMPEDuino.h"

#include <Hash.h>

ESP8266WiFiMulti WiFiMulti;
WebSocketsClient client;
WAMPEDuino wamp;


#define USE_SERIAL Serial

void connected() {

}

void setup() {
    // USE_SERIAL.begin(921600);
    USE_SERIAL.begin(115200);

    //Serial.setDebugOutput(true);
    USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

      for(uint8_t t = 4; t > 0; t--) {
          USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
          USE_SERIAL.flush();
          delay(1000);
      }
    USE_SERIAL.println();
    WiFiMulti.addAP("ESPap", "thereisnospoon");

    //WiFi.disconnect();
    while(WiFiMulti.run() != WL_CONNECTED) {
        delay(100);
    }

    wamp.begin(&client,"192.168.4.2", 8080,connected);
    //webSocket.setAuthorization("user", "Password"); // HTTP Basic Authorization


}

void loop() {
    wamp.loop();
}
