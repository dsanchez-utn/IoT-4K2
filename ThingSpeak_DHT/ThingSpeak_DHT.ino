#include <ThingSpeak.h>
#include <WiFi.h>
#include <DHT.h>

#define pinDHT 33    //Pin del sensor

const char* ssid = "";
const char* password = "";

unsigned long channelID = 2274848;
const char* WriteAPIKey = "FXPPV47A789VL2A7";

// Timer
unsigned long lastTime = 0;
unsigned long timerDelay = 15000;

WiFiClient cliente;

DHT dht(pinDHT, DHT22);


void setup() {
  Serial.begin(115200);
  Serial.println("Test de sensores:");

  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Wifi conectado!");

  ThingSpeak.begin(cliente);
  dht.begin();

}

void loop() {

  if ((millis() - lastTime) > timerDelay) {
    leerdht();

    int code = ThingSpeak.writeFields(channelID,WriteAPIKey);
    if(code==200)
      Serial.println("Canal de ThingSpeak actualizado correctamente.");
    else
      Serial.println("No se pudo actualizar el canal. Error HTTP: " + String(code));
    
    lastTime = millis();
  }
}

void leerdht() {
  
  float t2 = dht.readTemperature();
  float h2 = dht.readHumidity();

  while (isnan(t2) || isnan(h2)){
    Serial.println("Lectura fallida en el sensor DHT22, repitiendo lectura...");
    delay(2000);
    t2 = dht.readTemperature();
    h2 = dht.readHumidity();
  }
  Serial.println("------------------------------");
  Serial.print("Temperatura DHT22: ");
  Serial.print(t2);
  Serial.println(" ºC.");

  Serial.print("Humedad DHT22: ");
  Serial.print(h2);
  Serial.println(" %."); 

  int rand = random(1,100);

  String status;
  if(t2 > 30) 
    status = "Temperatura Alta";
  else if (t2 > 20)
    status = "Temperatura Media";
  else 
    status = "Temperatura Baja";

  ThingSpeak.setField (1,rand);
  ThingSpeak.setField (2,t2);
  ThingSpeak.setField (3,h2);
  ThingSpeak.setStatus(status);
}