#include <Arduino.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include "env_mqtt.h"
#include "wifimqtt.h"  // Agrega esto para acceder a connectWiFi()

String topic = "sensor/temp";
String clientID;

WiFiClient espClient;
PubSubClient client(espClient);

extern byte pinRelay; // Con extern podemos traernos la referencia de las variables declaradas en el main.cpp

void reconnectToBroker() {  // Nos conectamos al Broker y nos suscribimos
    if (WiFi.status() != WL_CONNECTED) {  // Nueva: Verifica WiFi primero
        Serial.println("WiFi no conectado. Reconectando WiFi antes de MQTT...");
        if (!connectWiFi()) {
            return;  // Si falla, sale y lo intentará en el próximo loop
        }
    }

    while (!client.connected()) {
        Serial.println("Attempting MQTT connection...");
        clientID = "ESP32-";
        clientID += String((uint32_t)ESP.getEfuseMac(), HEX);

        if (client.connect(clientID.c_str(), mqtt_user, mqtt_password)) {
            // Nos subscribimos a los sensores necesarios
            Serial.println("Conectado al Broker");
            client.subscribe("ace_disposal/shop_computer/sensor/relay"); // Por el momento seria el unico suscrito porque desde vue3 lo controlaremos
        } else {
            Serial.println("No se pudo conectar al Broker");
            Serial.println(client.state());
            Serial.println("Reintento en 5 segundos");
            delay(5000);
        }
    }
}

// Con callback , estamos atentos a los mensajes subscritos para cogerlos
void callback(char *topic, byte *message, unsigned int length)
{

    String messageTemp;

    for (int i = 0; i < length; i++)
    {
        messageTemp += (char)message[i];
    }

    if (String(topic) == "ace_disposal/shop_computer/sensor/relay") // Si un cliente publica y manda un "on" o un "off" se enciede o apaga el relay
    {
        if (messageTemp == "on")
        {
            digitalWrite(pinRelay, HIGH);
            Serial.println("RELAY ON");
        }
        if (messageTemp == "off")
        {
            digitalWrite(pinRelay, LOW);
            Serial.println ("RELAY OFF");
        }
    }
}

// Podras mandar mensajes para probarlo asi :
// mosquitto_pub -h 5.78.130.1 -p 1883 -t ace_disposal/shop_computer/sensor/relay -m "on" -u "userShop" -P "qwerty123"