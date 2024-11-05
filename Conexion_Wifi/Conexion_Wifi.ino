#include <WiFi.h>

#include "red_config.h"

void setup() {
   Serial.begin(115200);
   Serial.println("");
   WiFi.begin(SSID, PASS);
   while (WiFi.status() != WL_CONNECTED) {
       delay(500);
       Serial.print(".");
   }
   Serial.println("");
   Serial.println("WiFi conectado");
   Serial.print("IP: ");
   Serial.println(WiFi.localIP());
}

void loop() {

}