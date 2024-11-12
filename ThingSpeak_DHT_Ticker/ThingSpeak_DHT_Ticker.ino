#include <ThingSpeak.h>
#include <WiFi.h>
#include <DHT.h>
#include <Ticker.h>

#define pinDHT 33    //Pin del sensor

const char* ssid = "";
const char* password = "";
unsigned long channelID = 2510209;
const char* WriteAPIKey = "FD2HYTOTLFFV1FQE";

WiFiClient cliente;
DHT dht(pinDHT, DHT22);
Ticker subirDatos;

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
  subirDatos.attach(16, leerySubir);
}

void loop() {

}

void leerySubir() {
  float t2 = dht.readTemperature();
  float h2 = dht.readHumidity();

  while (isnan(t2) || isnan(h2)){
    Serial.println("Lectura fallida en el sensor DHT22, repitiendo lectura...");
    delay(2000);
    t2 = dht.readTemperature();
    h2 = dht.readHumidity();
  }

  Serial.println("------------------------------");
  Serial.printf("Temperatura DHT22: ");
  Serial.print(t2);
  Serial.println(" ºC.");
  Serial.print("Humedad DHT22: ");
  Serial.print(h2);
  Serial.println(" %."); 
  int rand = random(1,100);
  ThingSpeak.setField (1,rand);
  ThingSpeak.setField (2,t2);
  ThingSpeak.setField (3,h2);
  int statusCode = ThingSpeak.writeFields(channelID,WriteAPIKey);
  Serial.print("Datos enviados a ThingSpeak!   ");
  if(statusCode == 200){
    Serial.println("Canal escrito correctamente.");
  }
  else{
    Serial.println("Problemas al actualizar el canal. HTTP error code " + String(statusCode));
  }
}


/*
int contador = 0
int maximo = 10

contador++

if (contador >= maximo) {
  subirDatos.detach;
}

*/