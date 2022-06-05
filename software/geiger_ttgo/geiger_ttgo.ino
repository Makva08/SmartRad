/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com  
*********/

// Load Wi-Fi library
#include <WiFi.h>
#include <PubSubClient.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

static const int RXPin = 3, TXPin = 1;
static const uint32_t GPSBaud = 9600;

TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

const char* ssid = "Marisha"; // Enter your WiFi name
const char* password =  "1maridato1"; // Enter WiFi password
const char* mqttServer = "167.99.128.35";
const int mqttPort = 1883;
const char* mqttUser = "geiger";
const char* mqttPassword = "geiger";
int pulseCount[60];
const byte GEIGER_PIN = 15;

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

void setup() {
  Serial.begin(9600);
  ss.begin(GPSBaud);
  // Connect to Wi-Fi network with SSID and password
//  Serial.print("Connecting to ");
//  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }
  // Print local IP address and start web server
//  Serial.println("");
//  Serial.println("WiFi connected.");
//  Serial.println("IP address: ");
//  Serial.println(WiFi.localIP());

  pinMode(GEIGER_PIN,INPUT);

  client.setServer(mqttServer, mqttPort);
  client.setKeepAlive(120);
  while (!client.connected()) {
    //Serial.println("Connecting to MQTT...");
 
    if (client.connect("geiger", mqttUser, mqttPassword)) {
    
      //Serial.println("connected");  
 
    } else {

//      Serial.print("failed with state ");
//      Serial.print(client.state());
      delay(2000);
 
    }
  }
  
  for(int i=0;i<60;i++)
    pulseCount[i] = 0;
    
  startTime = millis();
  attachInterrupt(GEIGER_PIN, interruptHandler, CHANGE);
}

   
float latitude , longitude,newLat,newLng;
String lat_str , lng_str;

char charBuff[100];

void loop(){
  int measurement = 0;
  while (ss.available() > 0){
     char temp = ss.read();
     gps.encode(temp);
    // Serial.write(temp);
   }
  newLat = gps.location.lat();
  if(abs((newLat-latitude)*1000000) >= GPS_THRESH)
    latitude = newLat;
  lat_str = String(latitude , 6); // latitude location is stored in a string
  
  newLng = gps.location.lng();
  if(abs((newLng-longitude)*1000000) >= GPS_THRESH)
    longitude = newLng;
  lng_str = String(longitude , 6);

  if(millis()-sendTime >= CYCLE){
      for(int i=0;i<60;i++){
      measurement += pulseCount[i];
      pulseCount[i] = 0;
    }
    String str = "{\"location\":\""+lat_str+";"+lng_str+"\",\"measurement\":"+String(measurement)+"}";
    str.toCharArray(charBuff,100);
    Serial.println(str);
    client.publish("geiger", charBuff); //Topic name
    sendTime = millis();
  }
  
}
