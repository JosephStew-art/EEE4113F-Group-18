/**********
 * Mbasa Mguguma
 * Webserver for system data representation
 */

//required libraries
#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <WebServer.h>
#include "SPIFFS.h"
#include <Arduino_JSON.h>
#include "FS.h"
#include "SD.h"
#include <SPI.h>

//function declaration here:
//int myFunction(int, int);

// Network credentials
const char* ssid="ESP32Cam";
const char* password="4113_18";

// set web server port
AsyncWebServer server(80);

//set web socket
//AsyncWebSocket ws("/ws");

//set event source
AsyncEventSource events("/events");

//Json Variable to store sensor readings
JSONVar readings;
// var for http request
String header;

//var for sensor data
float tData, hData;

//GPIO pins
//#define tempPin 12
//#define SD_CS 5

//Current time
unsigned long currentTime = millis();
//Previous time
unsigned long prevTime = 0;
//timeout time in milliseconds
const long timeOutTime=300000;

void initWiFi(){
  //Connect to the WiFi network
  Serial.print("Setting up Access Point :");
  Serial.println(ssid);
  //WiFi.mode();
  WiFi.softAP(ssid, password);
  long int sTime= millis();
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
    if ((sTime+1000000)<millis()){
      break;
    }
  }
  //print out local IP address
  Serial.println("");
  Serial.println("WiFi connected succesfully!");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
}
//initialize SPIFFS
void initSPIFFS(){
  if(!SPIFFS.begin()==true){
    Serial.println("mounting SPIFFS error");
    return;
  }
  Serial.println("SPIFFS mounted succesfully.");
  
}
//initialize SD card
/**
void initSDCard(){
  SD.begin(SD_CS);
  if(!SD.begin(SD_CS)){
    Serial.println("Card not mounted");
    return;
  }
  uint8_t cardType = SD.cardType();
  if(cardType== CARD_NONE){
    Serial.println("No SD card");
    return;
  }
  Serial.println("Initializing SD card");
  if(!SD.begin(SD_CS)) {
    Serial.println("ERROR, initialization failed!");
    return;
  }
}*/
/*void readFile(fs::FS &fs, const char *path){
  Serial.printf("Reading file: %s  \n", path);
  //ope file
  File file = fs.open(path);
  if (!file){
    Serial.println("Failed to open file for reading");
    return;
  }
  Serial.println("Data in the file:");
  while(file.available()){
    Serial.write(file.read());
  }
  file.close();
}*/

String getSensorReadings(){
  //for testing purposes, random generated values
  int h=rand()%100;
  int t=rand()%60+1;
  int v=rand()%12+1;
  int c=rand()%3+1;
  readings["temperature"] = String(t);
  readings["humidity"] = String(h);
  readings["voltage"] = String(v);
  readings["current"] = String(c);
  String sData = JSON.stringify(readings);
  return sData;
}
//initialize websocket
/*void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len){
  //handle web socket events
  switch(type){
    case WS_EVT_CONNECT:
      Serial.println("WebSocket client connected!");
      break;
    case WS_EVT_DISCONNECT:
      Serial.println("WebSocket client disconnected");
      break;
    case WS_EVT_DATA:
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      Serial.println("ERROR!");
      break;
  }
}*/
/*void initWebSocket(){
  ws.onEvent(onEvent);
  server.addHandler(&ws);
};*/
String readTemperature(){
  //for testing purposes, random generated temperature values
  int t = rand()%60+1;
  Serial.println(t);
  return String(t);
}

String readHumidty(){
  //for testing purposes, random generated himidity values
  int h= rand()%100+1;
  Serial.println(h);
  return String(h);
}

String readVoltage(){
  //for testing purposes, random generated himidity values
  int v= rand()%12+1;
  Serial.println(v);
  return String(v);
}

String readCurrent(){
  //for testing purposes, random generated himidity values
  int c= rand()%3+1;
  Serial.println(c);
  return String(c);
}

void setup() {
  // Setup code
  Serial.begin(115200);
  //initialize inputs
  //pinMode(tempPin, INPUT);

  //initialize SPIFFS
  initSPIFFS();
  
  //initialize WiFi
  initWiFi();

  //initialize WebSocket
  //initWebSocket();

  //
  //Route to home page upon request
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });
  //server.serveStatic("/", SPIFFS, "/");

  //Load css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  //Route to sensor page
  server.on("/sensors",HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/sensors.html", "text/html");
  });
  //Route to power page
  server.on("/power",HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/power.html", "text/html");
  });

  //send voltage data upon request on the /voltage url for the graphs
  server.on("/voltage", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readVoltage().c_str());
  });
  //send temperature data upon request on the /tempearture url for the graph
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readTemperature().c_str());
  });

  //send current data upon request on the /current url for the graph
  server.on("/current", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readCurrent().c_str());
  });

  //send humidity data upon request on the /humidity url for the graphs
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readHumidty().c_str());
  });

  //request latest sensor readings for the gauges
  server.on("/readings", HTTP_GET, [](AsyncWebServerRequest *request){
    String sReadings = getSensorReadings();
    request->send(200, "application/json", sReadings);
    sReadings = String();
  });
  //load javascript file
  server.on("/layout.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/layout.js", "text/js");
  });
  //download power data
  server.on("/downloadP", HTTP_GET, [](AsyncWebServerRequest *request){
    //
    request->send(SD, "/path/to/file/Pdata.txt", "application/txt", true);
  });
  //download sensor data
  server.on("/downloadS", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SD, "/path/to/file/Sdata.txt", "application/txt", true);
  });

  //send event when there's one
  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client connected! with ID: %u\n",client->lastId());
    }
    //send event with message
    client->send("welcome to the ESP32 webserver!", NULL, millis(),10000);
  });
  server.addHandler(&events);
  
  //start server
  server.begin();
}

void loop() {
  //send events to the client every 30 seconds
  //ws.cleanupClients();
  currentTime = millis();
  if((currentTime-prevTime)>timeOutTime) {
    events.send("ping",NULL,millis());
    events.send(getSensorReadings().c_str(), "new_readings", millis());
    prevTime=currentTime;
  }
  
}

