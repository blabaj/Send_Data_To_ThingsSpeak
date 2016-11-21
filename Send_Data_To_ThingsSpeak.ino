// www.arduinesp.com
//
// Plot DTH11 data on thingspeak.com using an ESP8266
// April 11 2015
// Author: Jeroen Beemster
// Website: www.arduinesp.com

#include <DHT.h>
#include <ESP8266WiFi.h>
// replace with your channel’s thingspeak API key,
String apiKey = "";  //your Thingspeak API key
const char* ssid = ""; // your WiFi SSID
const char* password = ""; // WiFi password

const char* server = "api.thingspeak.com";
#define DHTPIN 2 // SO CONNECT THE DHT11/22 SENSOR TO PIN D4 OF THE NODEMCU

DHT dht(DHTPIN, DHT22,15); //CHANGE DHT11 TO DHT22 IF YOU ARE USING DHT22
WiFiClient client;

void setup() {
Serial.begin(115200);
delay(10);
dht.begin();
WiFi.mode(WIFI_STA);

WiFi.begin(ssid, password);

Serial.println();
Serial.println();
Serial.print("Connecting to ");
Serial.println(ssid);

WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi connected");
Serial.println("######## Diagnostics #########");
Serial.print ("Flash chip size: ");
Serial.print(ESP.getFlashChipSize());
Serial.println("b");
Serial.print ("Chip frequency: ");
Serial.print (ESP.getFlashChipSpeed());
Serial.println ("Hz");
WiFi.printDiag(Serial);
Serial.print("My local IP is: ");
Serial.println(WiFi.localIP());
Serial.println("----Have a nice day and may the force be with you----");
}

void loop() {

float h = dht.readHumidity();
float t = dht.readTemperature();
if (isnan(h) || isnan(t)) {
Serial.println("Failed to read from DHT sensor!");
return;
}

if (client.connect(server,80)) { // "184.106.153.149" or api.thingspeak.com
String postStr = apiKey;
postStr +="&field1=";
postStr += String(t);
postStr +="&field2=";
postStr += String(h);
postStr += "\r\n\r\n";

client.print("POST /update HTTP/1.1\n");
client.print("Host: api.thingspeak.com\n");
client.print("Connection: close\n");
client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
client.print("Content-Type: application/x-www-form-urlencoded\n");
client.print("Content-Length: ");
client.print(postStr.length());
client.print("\n\n");
client.print(postStr);

Serial.print("Temperature: ");
Serial.print(t);
Serial.print(" degrees Celcius Humidity: ");
Serial.print(h);
Serial.println("% send to Thingspeak");
}
client.stop();
Serial.println("Waiting...");
// thingspeak needs minimum 15 sec delay between updates
delay(20000);
}
