#include <SPI.h>
#include <EthernetENC.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "src/IODevice.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Arduino lift and relays");

  setupDetectionSensors();
  Serial.println("Proximities Ready.");

  setupRelay();
  Serial.println("Relay Ready.");

  setupMqttClient();
  Serial.println("Mqtt client Ready.");
}

void loop() {
  // put your main code here, to run repeatedly:
  sensorLoop();
  relayLoop();
  mqttLoop();
}
