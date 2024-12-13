#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "env_wifi.h"
#include <PubSubClient.h>


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
