# CS50: Lights Controller
[![Build Status](https://travis-ci.org/joemccann/dillinger.svg?branch=master)]()
## _An ESP8266 local web server_
#### Video Demo:  <https://youtu.be/pckg3LFcBEoE>
#### Description:
- The device will review the current status of the lights, and control these statuses (On/Off).
- It will have the ability to control the status of the lights from **any** device that has a browser.

This project utilizes a development board NodeMCU 1.0 that has the following featrures and I use them in different ways:
- **USB to UART converter:** The ESP8266 has a serial output (UART) that in order to be able to comunicate with the computer needs to be transformed and this converter does the trick. This signal is used to either uploading data to the flash memory of the ESP or for seeing the serial prints that are made via software from the device.
- **3.3 V Power Supply:** Without it there would no be power to turn on the Integrated Circuits on the board and takes as input the 5 V output of the computer's USB.
- **ESP8266 module:** This module has an integrated microcontroller, an external flash memory, and the RC circuits needed to transmit and receive data over the WiFi protocol. All of these features are used in this project.

The envirorment used for this development and program the microcontroler inside the IC is the Arduino IDE and in order to make it work with the ESP8266 it is mandatory that you update the IDE's board manager so it is able to compile and upload code to the board. 

Here is a tutorial on how to do so [Installing ESP8266 board](https://randomnerdtutorials.com/how-to-install-esp8266-board-arduino-ide/).

Here I'm going to describe the files of the project:

**CS50_LightsController.ino** Contains the code to upload to the ESP to set it as a local web server and respond to the request the client sends.
The code starts including the important libraries that are going to be needed for this development:
```
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "LittleFS.h"
```
It defines the states "ON" and "OFF" refereing to the states a digital pin can be, and initializes some global variables that represent the maping of the pins, and the SSID and PASSWORD of the local Wi-Fi network that it's going to be used:
```
#define ON HIGH
#define OFF LOW
char *ssid = --SSID--;
char *pass = --PASSWORD--;
IPAddress ESPIP;

uint8_t KITCHEN = 5;
uint8_t DININGROOM = 4;
uint8_t LIVINGROOM = 14;
uint8_t MASTER = 12;
```
On the setup function the pinMode assigns the pin mapping above and set them as OUTPUTS:
```
pinMode(KITCHEN, OUTPUT);
pinMode(DININGROOM, OUTPUT);
pinMode(LIVINGROOM, OUTPUT);
pinMode(MASTER, OUTPUT);
```
Initialize and enable the serial comunication, flash file management, and WiFi connection it checks for WiFi connection status and waits for the module to connect. After the conection is successful and it has an assigned IP prints it to the serial output.
```
Serial.begin(115200);

if(!LittleFS.begin()){
Serial.println("An Error has occurred while mounting LittleFS");
return;
}

Serial.println("\n\nConfiguring server ...");
WiFi.begin(ssid,pass);

Serial.print("Connecting to WiFi network ");
while(WiFi.status() != WL_CONNECTED)
{
    Serial.print(".");
    delay(500);
}
Serial.println("\nWiFi Connection successful!");

ESPIP = WiFi.localIP();
Serial.print("ESP IP Address: ");
Serial.println(ESPIP);
```
Create the server routes, defines what functions are going to handle each rout, initializes the server and prints a confirmation:
```
server.on("/",root);
server.on("/getCurrentStatus", currentStatus);
server.on("/Kitchen",toggleKITCHENState);
server.on("/Diningroom",toggleDININGROOMState);
server.on("/Livingroom",toggleLIVINGROOMState);
server.on("/Master",toggleMASTERState);
server.on("/Allon",allon);
server.on("/Alloff",alloff);

server.begin();
Serial.println("\nHTTP server started ... ");
```
Runs an infinite loop that handels client request:
```
void loop()
{ 
  server.handleClient();
}
```

**/data/index.html** Contains the web page, the CSS and JS for the web client.

#### Hardware Needed:
- ESP8266. - Remember to instal the drivers for the board on the Arduino IDE

#### Libraries used:
- [<ESP8266WiFi.h>](https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/readme.html) - Documentation
- [<ESP8266WebServer.h>](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer) - Git
- [<LittleFS.h>](https://arduino-esp8266.readthedocs.io/en/latest/filesystem.html) - Documentation

#### Arduino tools needed:
- [ESP8266LittleFS-X.zip](https://github.com/earlephilhower/arduino-esp8266littlefs-plugin/releases) - Git
