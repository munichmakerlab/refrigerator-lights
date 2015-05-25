#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "SerialReceiver.h"

const char *ssid =	"SSID";		// cannot be longer than 32 characters!
const char *pass =	"PASS";		//

// Update these with values suitable for your network.
IPAddress server(0, 0, 0, 0); // add here the IP address of your MQTT broker

PubSubClient client(server);

// helper debug message
void debugPrint(String strMessage) {
  Serial.print("Debug: ");
  Serial.println(strMessage);
}

// callback method to handle received mqtt messages
void mqttMessageReceived(const MQTT::Publish& pub) {
  // handle message arrived
  passReceivedMessage(pub.topic(), pub.payload_string());
}

// passthrough mqtt message to Teensy over serial
void passReceivedMessage(String strTopic, String strMessage) {
  Serial.print(strTopic);
  Serial.print(":");
  Serial.println(strMessage);
}

// handle received serial data
void processSerialData(String strTopic, String strMessage)
{
  // publish mqtt messages from Teensy
  if (strTopic.startsWith("mumalab")) {
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

  // initial the serial receiver with ':' separator
  SerialReceiver::initial(':');

  client.set_callback(mqttMessageReceived);

  WiFi.begin(ssid, pass);

  int retries = 0;
  while ((WiFi.status() != WL_CONNECTED) && (retries < 10)) {
    retries++;
    delay(500);
  }
  if (WiFi.status() == WL_CONNECTED) {
    debugPrint("WiFi connected");
  }

  boolean MQTTconnected = false;
  while(!MQTTconnected) {
    if (client.connect(MQTT::Connect("FRIDGE-AP").set_auth("MQTT-USER", "MQTT-PASS"))) {
      // get uptime as String class
      String strUptime = String(millis());
      int strUptime_len = strUptime.length() + 1;
      char charUptime[strUptime_len];
      strUptime.toCharArray(charUptime, strUptime_len);
      // publish uptime, to say I'm alive
      client.publish("mumalab/fridge/uptime", charUptime);
      // subscribe to mqtt topics
      client.subscribe("mumalab/fridge/state");
      client.subscribe("mumalab/fridge/brightness");
      client.subscribe("mumalab/fridge/effect");
      client.subscribe("mumalab/fridge/ticker/color");
      client.subscribe("mumalab/fridge/ticker/bounce");
      client.subscribe("mumalab/fridge/ticker/direction");
      client.subscribe("mumalab/fridge/ticker/text");
      MQTTconnected = true;
      debugPrint("MQTT connected");
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
  // handles the received data (command and payload == topic and message)
  processSerialData(SerialReceiver::command, SerialReceiver::payload);
  // reset old data and wait for new serial data
  SerialReceiver::reset();
}

// handle serial receiver events
void serialEvent() {
  // passin serial port and collect data
  SerialReceiver::processSerialEvent(Serial);
}
