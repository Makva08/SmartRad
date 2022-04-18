#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

TinyGPSPlus gps;
SoftwareSerial ss(4, 5);

const char* ssid = "Geiger";
const char* password = "220Volts50Hz";
WiFiServer server(80);

float latitude , longitude;
String lat_str , lng_str;

void setup()
{
  Serial.begin(9600);
  ss.begin(9600);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}


void loop(){
 while (ss.available() > 0){
   char temp = ss.read();
   gps.encode(temp);
   Serial.write(temp);
 }
  latitude = gps.location.lat();
  lat_str = String(latitude , 6); // latitude location is stored in a string
  longitude = gps.location.lng();
  lng_str = String(longitude , 6);
   
 WiFiClient client = server.available();
  if (!client)
  {
    return;
  }
  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n <!DOCTYPE html> <html> <head> <title>GPS DATA</title> <style>";
  s += "a:link {background-color: YELLOW;text-decoration: none;}";
  s += "table, th, td </style> </head> <body> <h1  style=";
  s += "font-size:300%;";
  s += " ALIGN=CENTER> GPS DATA</h1>";
  s += "<p ALIGN=CENTER style=""font-size:150%;""";
  s += "> <b>Location Details</b></p> <table ALIGN=CENTER style=";
  s += "width:50%";
  s += "> <tr> <th>Latitude</th>";
  s += "<td ALIGN=CENTER >";
  s += lat_str;
  s += "</td> </tr> <tr> <th>Longitude</th> <td ALIGN=CENTER >";
  s += lng_str;
//  s += "</td> </tr> <tr>  <th>Date</th> <td ALIGN=CENTER >";
//  s += gps.date.value();
 
  s += "</body> </html>";

  client.print(s); // all the values are send to the webpage
  delay(100);
}
