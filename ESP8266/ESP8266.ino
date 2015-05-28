/******************************************************************************
  The MIT License (MIT)

  Copyright (c) 2015 Juergen Skrotzky alias Jorgen (JorgenVikingGod@gmail.com)

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
******************************************************************************/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "SerialReceiver.h"
#include "config.h"

// initial mqtt client with mqtt broker server ip address
PubSubClient client(MQTT_SERVER_ADDRESS);

// helper debug message
void debugPrint(String strMessage) {
  Serial.print(SerialReceiver::resetChar);
  Serial.print("Debug: ");
  Serial.println(strMessage);
  //Serial.flush();
}

// callback method to handle received mqtt messages
void mqttMessageReceived(const MQTT::Publish& pub) {
  // handle message arrived
  passReceivedMessage(pub.topic(), pub.payload_string());
}

// passthrough mqtt message to Teensy over serial
void passReceivedMessage(String strTopic, String strMessage) {
  // get short version of mqtt topic (minimize serial traffic)
  // f.e. "mumalab/fridge/state" >> "m/s"
  strTopic = getMQTTTopic(strTopic, true);
  // replace some messages to short version (minimize serial traffic)
  // f.e. "plasma" >> "2" for topic "m/e"
  if (strTopic == "m/e") 
    strMessage = getMQTTEffect(strMessage, true);
  else if (strTopic == "m/td") 
    strMessage = getMQTTTickerDirection(strMessage, true);
  // send resetChar + topic + separator + message + \n
  // f.e. "#m/s:1\n"
  Serial.print(SerialReceiver::resetChar);
  Serial.print(strTopic);
  Serial.print(":");
  Serial.println(strMessage);
  Serial.flush();
}

// handle received serial data
void processSerialData(String strTopic, String strMessage)
{
  // publish mqtt messages from Teensy
  if (strTopic.startsWith("m/")) {
    // get original mqtt topic by short version 
    // f.e. "m/s" >> "mumalab/fridge/state"
    strTopic = getMQTTTopic(strTopic, false);    
    // replace short messages to original mqtt messages
    // f.e. "2" >> "plasma" for topic "mumalab/fridge/effect"
    if (strTopic == "mumalab/fridge/effect") 
      strMessage = getMQTTEffect(strMessage, false);
    else if (strTopic == "mumalab/fridge/ticker/direction") 
      strMessage = getMQTTTickerDirection(strMessage, false);
    // publish mqtt message with given topic
    client.publish(strTopic, strMessage);
  } else {
    debugPrint(strTopic + ":" + strMessage);
  }
}

void setup()
{
  // Setup console
  Serial.begin(115200);
  delay(10);

  // initial the serial receiver with 'Serial' serialport, ':' separator and '#' reset char
  // SerialReceiver will reset collection data on '#'
  SerialReceiver::initial(Serial, ':', '#');

  // bind callback function to handle incomming mqtt messages
  client.set_callback(mqttMessageReceived);

  // initial and connect WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  // check connection
  int retries = 0;
  while ((WiFi.status() != WL_CONNECTED) && (retries < 10)) {
    retries++;
    delay(500);
  }
  if (WiFi.status() == WL_CONNECTED) {
    debugPrint("");
    debugPrint("WiFi connected");
  }

  // connect to mqtt broker
  boolean MQTTconnected = false;
  while(!MQTTconnected) {
    if (client.connect(MQTT::Connect("FRIDGE").set_auth(MQTT_USER, MQTT_PASS))) {
      // get uptime as String class
      String strUptime = String(millis());
      int strUptime_len = strUptime.length() + 1;
      char charUptime[strUptime_len];
      strUptime.toCharArray(charUptime, strUptime_len);
      // publish uptime, to say I'm alive
      client.publish("mumalab/fridge/uptime", charUptime);
      // subscribe to all mqtt topics
      for (int i = 0; i < topicsMap_count; ++i) {
        client.subscribe(sMQTTTopicsMap[i].normalStr);
      }
      debugPrint("MQTT connected");
      MQTTconnected = true;
    } else {
      debugPrint("MQTT connection ERROR");
    }
  }
}

void loop()
{
  // handle mqtt messages and wifi connection
  client.loop();

  // handle received serial commands
  if (!SerialReceiver::isReady) {
    delay(10);
    return;
  }
  // handles the received data (command and message)
  processSerialData(SerialReceiver::command, SerialReceiver::payload);
  // reset current comand and be ready to get new from serial
  SerialReceiver::reset();
}

// handle serial receiver events
void serialEvent() {
  SerialReceiver::processSerialEvent();
}
