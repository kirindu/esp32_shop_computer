#include <Arduino.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include "env_mqtt.h"

String topic = "sensor/temp";
String clientID;

WiFiClient espClient;
PubSubClient client(espClient);

extern byte pinRelay; // Con extern podemos traernos la referencia de las variables declaradas en el main.cpp

void reconnectToBroker() // Nos conectamos al Broker y nos sucribimos
{

    while (!client.connected())
    {

        Serial.println("Attempting MQTT conection...");
        clientID += String(random(0xffff), HEX);

        if (client.connect(clientID.c_str(), mqtt_user, mqtt_password))
        {
            // Nos subscribimos a los sensores necesarios
            Serial.println("Conectado al Broker");
            client.subscribe("ace_disposal/shop_computer/sensor/relay"); // Por el momento seria el unico suscrito porque desde vue3 lo controlaremos
        }
        else
        {
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

    if (String(topic) == "ace_disposal/shop_computer/sensor/relay")
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