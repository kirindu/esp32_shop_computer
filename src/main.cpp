// Incluimos e importamos las librerias necesarias
#include <Arduino.h>
#include <ArduinoJson.h>
// #include <DHT.h>
#include "sensor_dht11.h"
#include "wifimqtt.h"
#include "mqtt.h"

// Declaracion de variables
const byte redLED = 14;
// const byte greenLED = 14;
const byte internalLED = 2;
const byte sensorDoor = 25;

byte pinRelay = 27; // No se le agrega const si pienzas extraerla como extern de otra libreria
bool doorState;     // Estado de la puerta

float humedad, tempC, tempF; // Declaramos aqui estas variables para poder llamarlas desde sensor_dht11.h com extern y poder alimentarlas desde alli

unsigned long t;

void setup()
{

  Serial.begin(115200);

  Serial.println("<ESP32 Loading>");

  connectWiFi(); // Nos conectamos con esta funcion que esta en wifimqtt.h

  // MQTT
  client.setServer(mqtt_server, mqtt_port); // Le pasamos las variables que estan en env.h
  client.setCallback(callback);             // Empezamos a escuchar nuestras subscripciones

  //

  initDHT(); // Inicializamos el sensor de temperatura

  pinMode(redLED, OUTPUT);
  // pinMode(greenLED, OUTPUT);
  pinMode(internalLED, OUTPUT);
  pinMode(sensorDoor, INPUT_PULLUP);
  pinMode(pinRelay, OUTPUT);

  t = millis();
}

void loop()
{

  if (!client.connected())
  {
    reconnectToBroker();
  }
  if (!client.loop())
  {
    client.connect("ESP32-SHOP-COMPUTER");
  }

  doorState = digitalRead(sensorDoor); // Leemos el estado del sensor de la puerta

  if (!doorState)
  { // Si la puerta esta cerrada se enciende el led rojo
    digitalWrite(redLED, HIGH);
    digitalWrite(internalLED, LOW);

    //   digitalWrite(pinRelay, HIGH);
  }
  else
  {
    digitalWrite(redLED, LOW);
    digitalWrite(internalLED, HIGH);

    //  digitalWrite(pinRelay, LOW);
  }

  // Cada 10 Segundos leemos el sensor de temperatura y lo mandamos
  if (millis() - t >= 2000)
  {
    readDHT11();  // Leemos los datos del sensor de temperatura

    // Publicamos aqui las medidas del DHT11
    String tempStrF = String(tempF, 2);
    String humStrF = String(humedad, 2);
    client.publish("ace_disposal/shop_computer/sensor/temperature", tempStrF.c_str()); // Publicamos aqui
    client.publish("ace_disposal/shop_computer/sensor/temperature", humStrF.c_str()); // Publicamos aqui
    
    
    // Y para probar lo que se esta enviando podemos hacerlo con:  mosquitto_sub -h 5.78.130.1 -p 1883 -t ace_disposal/shop_computer/sensor/temperature -u "userShop" -P "qwerty123"
    t = millis(); // inicializamos de nuevo el contador interno
  }
}
