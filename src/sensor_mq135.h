    #include <Arduino.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Adafruit_Sensor.h>
#include <MQUnifiedsensor.h> // Incluir la biblioteca MQUnifiedsensor


// Declaracion de variables y Pines
const byte MQ135PIN = 36;
const String placa = "ESP-32";
const String type = "MQ-135"; //MQ135
const float Voltage_Resolution = 5.0;
const byte ADC_Bit_Resolution  = 12;
const float RatioMQ135CleanAir  = 3.6; //RS / R0 = 3.6 ppm  



extern float airQuality;


	MQUnifiedsensor MQ135(placa, Voltage_Resolution, ADC_Bit_Resolution, MQ135PIN, type);


 void initMQ135() {

    MQ135.setRegressionMethod(1);


    MQ135.init(); 


 }


void readMQ135() {


}
