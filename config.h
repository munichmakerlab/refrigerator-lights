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

#ifndef config_h
#define config_h

// Debug mode
// set to 0 to disable debug prints
#define _debug 1

// LED settings
#define COLOR_ORDER           GRB
#define CHIPSET               WS2812B
#define DATA_PIN              D2

int brightness               = 100;

//***********************************************************
const char *ssid1            =  "<SSID1>";
const char *pass1            =  "<PASS1>";
const char *ssid2            =  "<SSID2>";
const char *pass2            =  "<PASS2>";
const char *ssid3            =  "<SSID3>";
const char *pass3            =  "<PASS3>";
char mqtt_host[40]           =  "<MQTT-HOST>";
char mqtt_port[6]            =  "1883";
char mqtt_user[20]           =  "<MQTT-USER>";
char mqtt_pass[33]           =  "<MQTT-PASS>";
char http_user[20]           =  "<HTTP-USER>";
char http_pass[33]           =  "<HTTP-PASS>";
char mqtt_client_id[20]      =  "FridgeLights";
//***********************************************************

// initial matrix layout (to get led strip index by x/y)
#define MATRIX_WIDTH   15
#define MATRIX_HEIGHT  10
#define MATRIX_TYPE    HORIZONTAL_ZIGZAG_MATRIX
#define MATRIX_SIZE    (MATRIX_WIDTH*MATRIX_HEIGHT)
#define NUMPIXELS      MATRIX_SIZE

cLEDMatrix<MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_TYPE> leds;
cLEDMatrix<MATRIX_WIDTH, -MATRIX_HEIGHT, MATRIX_TYPE> tetrisLeds;
cLEDMatrix<MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_TYPE> ledsFront;
cLEDMatrix<MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_TYPE> ledsBack;

/*
 * helper variables for connection (ip, mac, formatted UID)
 */
String strIPAddr;
String strClientID;
String strChipID;
String strTopicPrefix;
String strTopicPrefixChipID;
String strHostname;

String LedR = "0";
String LedG = "255";
String LedB = "0";
String colorEffect = "solid";
String background = "black";
String effectMode = "normal";

uint16_t PlasmaTime, PlasmaShift;
uint32_t LoopDelayMS, LastLoop;

/*
 * https://github.com/BenTommyE/Arduino_getStringPartByNr
 */
String getStringPartByNr(String data, char separator, int index)
{
  // spliting a string and return the part nr index
  // split by separator
  int stringData = 0;        //variable to count data part nr
  String dataPart = "";      //variable to hole the return text
  for(int i = 0; i<data.length(); i++) {    //Walk through the text one letter at a time
    if(data[i]==separator) {
      //Count the number of times separator character appears in the text
      stringData++;
    }else if(stringData==index) {
      //get the text when separator is the rignt one
      dataPart.concat(data[i]);
    }else if(stringData>index) {
      //return text and stop if the next separator appears - to save CPU-time
      return dataPart;
      break;
    }
  }
  //return text if this is the last part
  return dataPart;
}

int countCharacters(const String &str, char character) {
  int count = 0;
  for(int i = 0; i<str.length()-1; i++) {    //Walk through the text one letter at a time
    if(str[i] == character)
      count++;
  }
  return count;
}

template <typename Generic>
void DEBUG_PRINT(Generic text) {
  if(_debug) {
    Serial.print(text);
  }
}

template <typename Generic>
void DEBUG_PRINTLN(Generic text) {
  if(_debug) {
    Serial.println(text);
  }
}

#endif //config_h
