#include <Arduino.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

const byte DHTPIN = 16;
float humedad, tempC, tempF;

DHT dht(DHTPIN, DHT11);


void initDHT() {
    dht.begin();
}

void readDHT11() {
    humedad = dht.readHumidity();
    tempC = dht.readTemperature();
    tempC = dht.readTemperature(true);

    Serial.print(tempC);

    if(isnan(humedad) | isnan(tempC) | isnan(tempF)) {
        Serial.println("Error en DHT11");
    }else {
        Serial.print("Humedad: "); Serial.print(humedad); Serial.print(" %");
        Serial.print("Temp : "); Serial.print(tempC); Serial.print(" °C");
        Serial.print("Temp : "); Serial.print(tempF); Serial.print(" °F");
    }
}
