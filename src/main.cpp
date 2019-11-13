/*
  LoRa_Sender_MQTT:
  Support Devices: LoRa Shield + Arduino 
  
  Require Library:
  https://github.com/sandeepmistry/arduino-LoRa 
  
  Example sketch showing how to send or a message base on ThingSpeak(https://thingspeak.com) MQTT format. 
  The End node will send out a string "<End_Node_ID>field1=${TEMPERATURE_VALUE}&field2=${HUMIDITY_VALUE}" to LG01/LG02 gateway. 
  When the LG01/LG02 gateway get the data, it will parse and forward the data to ThingSpeak via MQTT protocol. 
  modified Dec 26 2018
  by Dragino Technology Co., Limited <support@dragino.com>
*/

#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include <parser.h>
#include <reporter.h>

long tem,hum;
int count=0;
int device_id=10009; // ID of this End node

long REPORT_FREQ = 10000;


long report_timer = millis();

Reporter reporter = Reporter(device_id);
Parser parser = Parser('$', ',', '*', 10);


void setup() {
  Serial.begin(9600);
  //while (!Serial);

  Serial.println("LoRa Sender");

  if (!LoRa.begin(915600000)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  } 
  LoRa.setSyncWord(0x34);

}

void loop() {
  parser.run(Serial);

  if(millis()-report_timer > REPORT_FREQ){
      reporter.report(parser);
      report_timer = millis();
  }
}
