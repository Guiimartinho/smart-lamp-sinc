#ifndef MQTT_MODULE_H
#define MQTT_MODULE_H

#include "Arduino.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "wifi_module.h"
#include "led_module.h"
#include "config.h"


#define MQTT_PROTECTED

// Definições do MQTT
#define MQTT_BROKER "broker.hivemq.com" // Substitua pelo endereço do broker MQTT
#define MQTT_PORT 1883 // Porta padrão para MQTT

#define MQTT_USERNAME ""
#define MQTT_PASSWORD ""

//#define DEVICE_ID ""
#define PAIR_DEVICE_ID "esp32-c3-abajur-NAME"

// Tópicos MQTT
#define MQTT_TOPIC_ROOT "XXXXXXXXX/CCCCCCCC/WWWWW" // Substitua pelo tópico raiz

extern String mqtt_root_topic;
extern String mqtt_set_topic;
extern String mqtt_status_topic;
extern String mqtt_out_topic;
extern String mqttMessage;


extern WiFiClient espClient;
extern PubSubClient client;

void setup_mqtt();
void loop_mqtt();
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void reconnect();

#endif
 