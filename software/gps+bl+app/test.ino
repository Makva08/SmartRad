/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com  
*********/

// Load Wi-Fi library
#include <WiFi.h>
#include <PubSubClient.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include "BluetoothSerial.h"

static const int RXPin = 3, TXPin = 1;
static const uint32_t GPSBaud = 9600;

TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

BluetoothSerial SerialBT;

char ssid[50] = "Geiger"; // Enter your WiFi name
char password[50] =  "220Volts50Hz"; // Enter WiFi password
char mqttServer[50] = "167.99.128.35";
const int mqttPort = 1883;
const char* mqttUser = "geiger";
const char* mqttPassword = "geiger";
int pulseCount[60];
const byte GEIGER_PIN = 15;

bool connectedToMQTT = false;

int startTime = 0;
int sendTime = 0;

#define CYCLE 60000
#define GPS_THRESH 200

WiFiClient espClient;
PubSubClient client(espClient);

int prevIndex = 0;

bool pulseStarted = false;

void ICACHE_RAM_ATTR interruptHandler(){
  if(digitalRead(GEIGER_PIN) == 0 && pulseStarted == false){
    pulseStarted = true;
  }
  if(pulseStarted){
    int passedTime = millis()-startTime;
    if(passedTime >= CYCLE){
      passedTime = 0;
      startTime = millis();
    }
    int index = passedTime / 1000;
  
    if(prevIndex != index)
      pulseCount[index] = 0;
      
    prevIndex = index;
    
    pulseCount[index] += 1;
    pulseStarted = false;
  }
}

void connect(){
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  int wifiTry = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    wifiTry += 1;
    if(wifiTry == 3)
      break;
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqttServer, mqttPort);
  client.setKeepAlive(120);
  if (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("geiger", mqttUser, mqttPassword)) {
    
      Serial.println("connected");  
 
    } else {

      Serial.print("failed with state ");
      Serial.print(client.state());
 
    }
  }
   connectedToMQTT = true;
}

void setup() {
  Serial.begin(9600);
  SerialBT.begin("GEIGER");
  Serial.println("Connecting to GPS");
  ss.begin(GPSBaud);  
  Serial.println("GPS connected");
  pinMode(GEIGER_PIN,INPUT);
  
  for(int i=0;i<60;i++)
    pulseCount[i] = 0;
    
  startTime = millis();
  attachInterrupt(GEIGER_PIN, interruptHandler, CHANGE);
  connect();
}

   
float latitude , longitude,newLat,newLng;
String lat_str , lng_str;

char charBuff[100];

String credentials;

void loop(){
  int measurement = 0;
  while (ss.available() > 0){
     char temp = ss.read();
     gps.encode(temp);
    // Serial.write(temp);
   }

  while(SerialBT.available() > 0){
    char temp = SerialBT.read();
    credentials += temp;
  }

  if(credentials.length() > 0){
    String ssidTemp,passTemp,mqttTemp;
    int b = 0;
    for(int i=0;i<credentials.length();i++){
      if(credentials[i] != '\n'){
        if(b == 0){
          ssidTemp += credentials[i];
        }
        if(b == 1){
          passTemp += credentials[i];
        }
        if(b == 2){
          mqttTemp += credentials[i];
        }
      }else{
        b += 1;
      }
    }
    ssidTemp.toCharArray(ssid,50);
    Serial.print(ssid);
    Serial.print(" ");
    passTemp.toCharArray(password,50);
    Serial.print(password);
    Serial.print(" ");
    mqttTemp.toCharArray(mqttServer,50);
    Serial.println(mqttServer);
    credentials = "";
    connect();
  }
  newLat = gps.location.lat();
  if(abs((newLat-latitude)*1000000) >= GPS_THRESH)
    latitude = newLat;
  lat_str = String(latitude , 12); // latitude location is stored in a string
  
  newLng = gps.location.lng();
  if(abs((newLng-longitude)*1000000) >= GPS_THRESH)
    longitude = newLng;
  lng_str = String(longitude , 12);

  if(millis()-sendTime >= CYCLE){
      for(int i=0;i<60;i++){
      measurement += pulseCount[i];
      pulseCount[i] = 0;
    }
    String str = "{\"location\":\""+lat_str+";"+lng_str+"\",\"measurement\":"+String(measurement)+"}";
    str.toCharArray(charBuff,100);
    Serial.println(str);

    if(connectedToMQTT)
      client.publish("geiger", charBuff); //Topic name
    sendTime = millis();
  }
  
}
