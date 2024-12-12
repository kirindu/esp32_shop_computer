// Incluimos e importamos las librerias necesarias
#include <Arduino.h>
#include <ArduinoJson.h>
#include <DHT.h>


// Declaracion de variables
const byte redLED = 14;
const byte greenLED = 14;
const byte internalLED = 2;
const byte sensorDoor = 25;

const byte pinRelay = 27;

bool doorState; // Estadp de la puerta

const byte DHTPIN = 16;
float humedad, tempC, tempF;
DHT dht(DHTPIN, DHT11);


unsigned long t;

void setup() {

Serial.begin(115200);
Serial.println("<ESP32 Loading>");

 pinMode(redLED, OUTPUT);
 pinMode(greenLED, OUTPUT);
 pinMode(internalLED, OUTPUT);
 pinMode(sensorDoor, INPUT_PULLUP);
 pinMode(pinRelay, OUTPUT);

 dht.begin();

  t = millis();

}

void loop() {

doorState = digitalRead(sensorDoor); // Leemos el estado del sensor de la puerta

if(!doorState) { // Si la puerta esta cerrada se enciende el led rojo
digitalWrite(redLED, HIGH);
digitalWrite(internalLED, LOW);


 digitalWrite(pinRelay, HIGH);

}else{
  digitalWrite(redLED, LOW);
  digitalWrite(internalLED, HIGH);

  digitalWrite(pinRelay, LOW);
}


// Cada 2 Segundos leemos el sensor de temperatura y lo mandamos
 if ( millis() - t >= 2000 ) {

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

    t = millis();
  }

}




