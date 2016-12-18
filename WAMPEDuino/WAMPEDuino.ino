#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include "WAMPEDuino.h"
#include "IntegerTopic.h"

#include <Hash.h>

ESP8266WiFiMulti WiFiMulti;
WebSocketsClient client;
WAMPEDuino wamp;


#define USE_SERIAL Serial
IntegerTopic top1("de.lichtundlaessig.top1");
IntegerTopic top2("de.lichtundlaessig.top1");
long time_old;
void connected() {
	USE_SERIAL.println("Registering Variables");
	//wamp.publish(&top1);
	wamp.subscribe(&top2);
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
    USE_SERIAL.println("Starting AP");
    WiFiMulti.addAP("ESPNET", "testtest");
    USE_SERIAL.println("Running AP");
    //WiFi.disconnect();
    while(WiFiMulti.run() != WL_CONNECTED) {
        delay(100);
        USE_SERIAL.print(".");
    }
    USE_SERIAL.println();
    USE_SERIAL.println("Starting WAMP");
    wamp.begin(&client,"192.168.1.2", 8080,"realm1", connected);
    //webSocket.setAuthorization("user", "Password"); // HTTP Basic Authorization
    time_old = millis();

}

void loop() {
	/*if(millis()-time_old > 5000){
		USE_SERIAL.println("Should Publish");
		//top1.setData(time_old);
		//top1.setFresh();
		time_old = millis();
	}*/
    wamp.loop();

}
