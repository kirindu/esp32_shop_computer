#include <Arduino.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

// Declaracion de variables y Pines
const byte DHTPIN = 16;
//float humedad, tempC, tempF;


extern float humedad;
extern float tempC;
extern float tempF;

// Tipo de Sensor de temperatur/humedad
DHT dht(DHTPIN, DHT11);

void initDHT() {
    dht.begin();
}

void readDHT11() {
    humedad = dht.readHumidity();
    tempC = dht.readTemperature();
    tempF = dht.readTemperature(true);

    if(isnan(humedad) | isnan(tempC) | isnan(tempF)) {
        Serial.println("Error en DHT11");
    }else {
        Serial.print("Humedad: "); Serial.print(humedad); Serial.print(" %");
        Serial.println(" ");
        // Serial.print("Temp C : "); Serial.print(tempC); Serial.print(" °C");
        // Serial.println(" ");
        Serial.print("Temp F: "); Serial.print(tempF); Serial.print(" °F");
        Serial.println(" ");

    }
}
