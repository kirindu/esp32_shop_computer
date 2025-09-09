// Incluimos e importamos las librerias necesarias
#include <Arduino.h>
#include <ArduinoJson.h>
// #include <DHT.h>
#include "sensor_dht22.h"
#include "wifimqtt.h"
#include "mqtt.h"

// Declaracion de variables
const byte redLED = 14; // Este led rojo se encendera cuando la puerta este enllavada
// const byte greenLED = 14;
const byte internalLED = 2;
const byte sensorDoor = 25;

byte pinRelay = 27; // No se le agrega const si pienzas extraerla como extern de otra libreria
bool doorState;     // Estado de la puerta

float humedad, tempC, tempF; // Declaramos aqui estas variables para poder llamarlas desde sensor_dht22.h com extern y poder alimentarlas desde alli

unsigned long t;
unsigned long t2;
unsigned long lastWiFiCheck = 0;  // Nueva: Para verificar WiFi periódicamente

void setup() {
    Serial.begin(115200);
    Serial.println("<ESP32 Loading>");

    connectWiFi(); // Nos conectamos con esta funcion que esta en wifimqtt.h

    // MQTT
    client.setServer(mqtt_server, mqtt_port); // Le pasamos las variables que estan en env.h
    client.setCallback(callback);             // Empezamos a escuchar nuestras subscripciones

    initDHT(); // Inicializamos el sensor de temperatura

    pinMode(redLED, OUTPUT);
    // pinMode(greenLED, OUTPUT);
    pinMode(internalLED, OUTPUT);
    pinMode(sensorDoor, INPUT_PULLUP);
    pinMode(pinRelay, OUTPUT);

    t = millis();
    t2 = millis();  // Inicializa t2 también
    lastWiFiCheck = millis();  // Inicializa el checker de WiFi
}

void loop()
{

  // Nueva: Verifica WiFi periódicamente (cada 5s para no sobrecargar)
    if (millis() - lastWiFiCheck >= 5000) {
        if (WiFi.status() != WL_CONNECTED) {
            Serial.println("WiFi desconectado. Intentando reconectar...");
            connectWiFi();  // Reconecta WiFi
        }
        lastWiFiCheck = millis();
    }


// MQTT: Reconecta si es necesario (ahora maneja WiFi internamente)
    if (!client.connected()) {
        reconnectToBroker();   // ✅ reconecta SIEMPRE con user/pass
    }
    client.loop();            // ✅ solo procesa

  if (millis() - t2 >= 1000) // Cada 1000ms leemos el estado de la puerta
  {
    doorState = digitalRead(sensorDoor); // Leemos el estado del actual de la puerta

    if (!doorState)
    { // Si la puerta esta cerrada se enciende el led rojo
     // digitalWrite(redLED, HIGH);
      digitalWrite(internalLED, LOW);
      client.publish("ace_disposal/shop_computer/sensor/door", "on"); // Publicamos aqui
    }
    else
    {
    //  digitalWrite(redLED, LOW);
      digitalWrite(internalLED, HIGH);
      client.publish("ace_disposal/shop_computer/sensor/door", "off"); // Publicamos aqui
    }

    // Publicamos el estado del Relay

    if(digitalRead(pinRelay)) { // Leemos el estado de la salida, y publicamos
      digitalWrite(redLED, HIGH);
      client.publish("ace_disposal/shop_computer/sensor/relay_status", "on");
    }else {
      digitalWrite(redLED, LOW);
      client.publish("ace_disposal/shop_computer/sensor/relay_status", "off");
    }
    //

    t2 = millis(); // inicializamos de nuevo el contador interno
  }

  if (millis() - t >= 2000)
  {
    readDHT22(); // Leemos los datos del sensor de temperatura

    // Publicamos aqui las medidas del DHT22
    String tempStrF = String(tempF, 2);
    String humStrF = String(humedad, 2);
  
    client.publish("ace_disposal/shop_computer/sensor/humidity", humStrF.c_str());  // Publicamos aqui
    client.publish("ace_disposal/shop_computer/sensor/temperature", tempStrF.c_str()); // Publicamos aqui

    // Y para probar lo que se esta enviando podemos hacerlo con:  mosquitto_sub -h 5.78.130.1 -p 1883 -t ace_disposal/shop_computer/sensor/temperature -u "userShop" -P "qwerty123"
    t = millis(); // inicializamos de nuevo el contador interno
  }
}