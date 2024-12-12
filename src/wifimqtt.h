#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "env.h"

String topic = "sensor/temperature";
String clientID;
int temperature = 0;

WiFiClient espClient;
PubSubClient client(espClient);

extern byte myLED; // Con extern podemos traernos la referencia de las variables declaradas en el main.cpp

void connectWiFi()
{
    WiFi.mode(WIFI_STA); // Modo Estacion

    Serial.print("Conectando a ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    byte count = 0;

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(".");
        count++;

        if (count > 30)
        {
            ESP.restart();
        }
    }
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("Se conecto al WiFi");
    }
    Serial.println();
    Serial.println("Tu IP es :");
    Serial.println(WiFi.localIP());
}

void reconnectToBroker()
{

    while (!client.connected())
    {

        Serial.println("Attempting MQTT conection...");
        clientID += String(random(0xffff), HEX);

        if (client.connect(clientID.c_str(), mqtt_user, mqtt_password))
        {
            // Nos subscribimos a los sensores necesarios
            Serial.println("Conectado al Broker");
            client.subscribe("sensor/temperature");
            client.subscribe("ledState");
            client.subscribe("sensor/humidity");
            client.subscribe("jsonFromNodeRed");
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

    if (String(topic) == "sensor/temperature")
    {
        Serial.println(messageTemp);
    }
    else if (String(topic) == "ledState")
    {
        if (messageTemp == "on")
        {
            digitalWrite(myLED, HIGH);
            Serial.print("LED ENCENDIDO");
        }
        if (messageTemp == "off")
        {
            digitalWrite(myLED, LOW);
            Serial.print("LED APAGADO");
        }
    }
    else if (String(topic) == "sensor/humidity")
    {
        Serial.println(messageTemp);
    }
    // Para deserializar formato json que estamos recibiendo usamos la herraminta que esta en https://arduinojson.org/
    // Alli elegimos el tipo de Board , como Expressif ESP32 Dev Module o DOIT ESP32 DEVKIT V1, por ejemplo
    // Seleccionamos Deseriualize y como Input Stream
    else if (String(topic) == "jsonFromNodeRed")
    {
        // Stream& input;

        JsonDocument doc;

        DeserializationError error = deserializeJson(doc, messageTemp);

        if (error)
        {
            Serial.print("deserializeJson() failed: ");
            Serial.println(error.c_str());
            return;
        }

        const char *device = doc["device"];   // "ESP32 001"
        int temperature = doc["temperature"]; // 33
        int humidity = doc["humidity"];       // 90
        long time = doc["time"];              // 1351824120

        float gps_lat = doc["gps"]["lat"]; // 51.5064
        float gps_lon = doc["gps"]["lon"]; // -0.12721

        // Ahora si queremos impimir en el puerto serial lo anterior :
        String output = String(device) + " " + String(temperature) + " " + String(humidity) + " " + String(time) + " " + String(gps_lat) + " " +  String(gps_lon);
         Serial.println(output);
    }
}
