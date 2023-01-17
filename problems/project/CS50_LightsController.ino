#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "LittleFS.h"

#define ON HIGH
#define OFF LOW

// WiFi connection Variables
char *ssid = "TONTOMAS";
char *pass = "1234567860";
IPAddress ESPIP;

// Assign pins to variables
uint8_t KITCHEN = 5;
uint8_t DININGROOM = 4;
uint8_t LIVINGROOM = 14;
uint8_t MASTER = 12;

// Server that will listen to port 80
ESP8266WebServer server(80);

void setup(){
  
  // Init pins as outputs
  pinMode(KITCHEN, OUTPUT);
  pinMode(DININGROOM, OUTPUT);
  pinMode(LIVINGROOM, OUTPUT);
  pinMode(MASTER, OUTPUT);
  
  // For debugging purposes, it is needed a serial channel to
  // communicate with the computer's serial monitor: initialize
  // and select a baud rate of 115200
  Serial.begin(115200);
  
  // Start the file sistem of the ESP
  if(!LittleFS.begin()){
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }
  // Start connection to the wifi network with ssid and pass
  Serial.println("\n\nConfiguring server ...");
  WiFi.begin(ssid,pass);
  // Wait in this loop until there is a connection and the
  // network has assigned an IP address to the ESP
  Serial.print("Connecting to WiFi network ");
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWiFi Connection successful!");
  // Get IP assigned to the ESP and display it on serial port
  ESPIP = WiFi.localIP();
  Serial.print("ESP IP Address: ");
  Serial.println(ESPIP);

  // Create routes and specify what function handles it
  server.on("/",root);
  server.on("/getCurrentStatus", currentStatus);
  server.on("/Kitchen",toggleKITCHENState);
  server.on("/Diningroom",toggleDININGROOMState);
  server.on("/Livingroom",toggleLIVINGROOMState);
  server.on("/Master",toggleMASTERState);
  server.on("/Allon",allon);
  server.on("/Alloff",alloff);
  

  // Start the server after route set up
  server.begin();
  Serial.println("\nHTTP server started ... ");
}

void loop()
{ 
  server.handleClient();
}

void root()
{
  File file = LittleFS.open("/index.html", "r");
  if(!file){
    Serial.println("Failed to open file for reading");
    return;
  }
  server.streamFile(file,"text/html");
  file.close();
}

void allon()
{
  digitalWrite(KITCHEN, ON);
  digitalWrite(DININGROOM, ON);
  digitalWrite(LIVINGROOM, ON);
  digitalWrite(MASTER, ON);
  server.send(204,"");
}

void alloff()
{
  digitalWrite(KITCHEN, OFF);
  digitalWrite(DININGROOM, OFF);
  digitalWrite(LIVINGROOM, OFF);
  digitalWrite(MASTER, OFF);
  server.send(204,"");
}

void currentStatus()
{
  String allStatus;
  allStatus += getKITCHENState();
  allStatus += '-';
  allStatus += getDININGROOMState();
  allStatus += '-';
  allStatus += getLIVINGROOMState();
  allStatus += '-';
  allStatus += getMASTERState();
  server.send(200,"text/plain", allStatus);
}

// KITCHEN
String getKITCHENState()
{
  String led_state = digitalRead(KITCHEN) ? "ON" : "OFF";
  return led_state;
}
void toggleKITCHENState()
{
  digitalWrite(KITCHEN,!digitalRead(KITCHEN));
  String state = getKITCHENState();
  server.send(200,"text/plain", state);
}

// DINNINGROOM
String getDININGROOMState()
{
  String led_state = digitalRead(DININGROOM) ? "ON" : "OFF";
  return led_state;
}
void toggleDININGROOMState()
{
  digitalWrite(DININGROOM,!digitalRead(DININGROOM));
  String state = getDININGROOMState();
  server.send(200,"text/plain", state);
}

// LIVINGROOM
String getLIVINGROOMState()
{
  String led_state = digitalRead(LIVINGROOM) ? "ON" : "OFF";
  return led_state;
}
void toggleLIVINGROOMState()
{
  digitalWrite(LIVINGROOM,!digitalRead(LIVINGROOM));
  String state = getLIVINGROOMState();
  server.send(200,"text/plain", state);
}

// MASTER
String getMASTERState()
{
  String led_state = digitalRead(MASTER) ? "ON" : "OFF";
  return led_state;
}
void toggleMASTERState()
{
  digitalWrite(MASTER,!digitalRead(MASTER));
  String state = getMASTERState();
  server.send(200,"text/plain", state);
}
