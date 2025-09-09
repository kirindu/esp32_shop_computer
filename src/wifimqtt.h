#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "env_wifi.h"
#include <PubSubClient.h>

bool connectWiFi() {  // Cambia a bool para retornar éxito/fallo
    WiFi.mode(WIFI_STA); // Modo Estacion

    Serial.print("Conectando a ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    byte count = 0;

    while (WiFi.status() != WL_CONNECTED && count < 30) {  // Limita a 30 intentos (~30s)
        delay(1000);
        Serial.print(".");
        count++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nSe conecto al WiFi");
        Serial.println("Tu IP es :");
        Serial.println(WiFi.localIP());
        return true;
    } else {
        Serial.println("\nFallo al conectar WiFi después de 30 intentos. Intentará de nuevo más tarde.");
        return false;
    }
}