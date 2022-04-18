#include <WiFi.h>
#include <PubSubClient.h>
 
const char* ssid = "Makva08"; // Enter your WiFi name
const char* password =  "FollowMe";  // Enter WiFi password
const char* mqttServer = "xxx.xx.xxx.xx";  //Your ip adress (if u dont know it google "my ip adress")
const int mqttPort = 1883;
const char* mqttUser = "geiger";
const char* mqttPassword = "geiger";
 
WiFiClient espClient;
PubSubClient client(espClient);
 
void setup() {
 
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("geiger", mqttUser, mqttPassword)) {
 
      Serial.println("connected");  
 
    } else {
 +
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
  
  char* str = "{\"location\":\"41.7179281 44.7819988\",\"measurement\":1024}";
//  client.subscribe("0");
  
 while (1){
  client.publish("geiger", str); //Topic name
  delay(1000);
 }

}
 
void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
 
  Serial.println();
  Serial.println("-----------------------");
 
}
 
void loop() {
  client.loop();
}
