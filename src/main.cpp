// Incluimos e importamos las librerias necesarias
#include <Arduino.h>
#include <ArduinoJson.h>


// Declaracion de variables
const byte redLED = 14;
const byte greenLED = 14;
const byte internalLED = 2;

const byte sensorDoor = 25;

bool doorState;


void setup() {

Serial.begin(115200);
Serial.println("<ESP32 Loading>");

 pinMode(redLED, OUTPUT);
 pinMode(greenLED, OUTPUT);
 pinMode(internalLED, OUTPUT);

 pinMode(sensorDoor, INPUT_PULLUP);

}

void loop() {

doorState = digitalRead(sensorDoor); // Leemos el estado del sensor de la puerta

if(!doorState) { // Si la puerta esta cerrada se enciende el led rojo
digitalWrite(redLED, HIGH);
digitalWrite(internalLED, LOW);
}else{
  digitalWrite(redLED, LOW);
  digitalWrite(internalLED, HIGH);

}

}
