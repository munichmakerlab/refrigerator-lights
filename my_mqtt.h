/*
 * refrigerator-lights: https://github.com/munichmakerlab/refrigerator-lights
 * Copyright (C) 2016 Juergen Skrotzky alias Jorgen (JorgenVikingGod@gmail.com)
 *
 * This sketch lights up a 15x10 WS2812b RGB LEDs.
 * The lights are fully controllable over WiFi by MQTT.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#ifndef my_mqtt_h
#define my_mqtt_h

#include <WiFiClient.h>
#include <PubSubClient.h>
#include <QueueArray.h> // http://playground.arduino.cc/Code/QueueArray
#include "config.h"

bool mqttNewMessage = false;
struct sMqttMessage {
  String topic;
  String payload;
  sMqttMessage(String strTopic="", String strPayload="") {
    topic = strTopic;
    payload = strPayload;
  };
  ~sMqttMessage(){
    topic = "";
    payload = "";
   };
};

QueueArray <sMqttMessage> mqttMessages;

// initial mqtt client with mqtt broker server ip address
WiFiClient client;
PubSubClient mqttClient(client, mqtt_host);

void publishMQTTMessage(String strTopic, String strMessage, bool bRetain=false) {
  if (bRetain)
    mqttClient.publish(MQTT::Publish(strTopic, strMessage).set_retain().set_qos(1));
  else
    mqttClient.publish(strTopic, strMessage);
  DEBUG_PRINTLN("send MQTT: topic='" + strTopic + "', message='" + strMessage + "'");
}

void subscribeMQTTTopic(String strTopic) {
  mqttClient.subscribe(strTopic);
  DEBUG_PRINTLN("subscribe MQTT: topic='" + strTopic + "'");
}

bool connectMQTT() {
  DEBUG_PRINTLN("Connect to MQTT...");
  DEBUG_PRINT("MQTT host: ");
  DEBUG_PRINTLN(mqtt_host);
  DEBUG_PRINT("MQTT user: ");
  DEBUG_PRINTLN(mqtt_user);
  DEBUG_PRINT("MQTT pass: ");
  DEBUG_PRINTLN(mqtt_pass);
  String strClientID = String(mqtt_client_id) + String("-") + String(strChipID);
  mqttClient.loop();
  mqttClient.disconnect();
  mqttClient.loop();
  mqttClient.connect(MQTT::Connect(strClientID).set_clean_session().set_will(strTopicPrefixChipID+"state","0",1,true).set_auth(mqtt_user,mqtt_pass).set_keepalive(30));
  mqttClient.loop();
  delay(100);
  if ( mqttClient.connected() ) {
    DEBUG_PRINTLN("\nMQTT reconnected");
    publishMQTTMessage(strTopicPrefixChipID + "state", "1", true);
    publishMQTTMessage(strTopicPrefix + "connected", strChipID + "," + strIPAddr);
    subscribeMQTTTopic(strTopicPrefixChipID + "#");
    return true;
  }
  return false;
}

void checkConnect() {
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1000) {
    delay(100);
    if ( WiFi.status() == WL_CONNECTED ) {
      if (connectMQTT())
        return;
    }
  }
}

void processMQTTLoop() {
  if ( WiFi.status() != WL_CONNECTED || mqttClient.connected() != 1 ) {
    checkConnect();
  }
  mqttClient.loop();
}

#endif //mqtt_h
