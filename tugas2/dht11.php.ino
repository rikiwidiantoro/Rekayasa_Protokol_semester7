#include "DHT.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <SPI.h>

#define DHTPIN D2

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

float humidityData;
float temperatureData;
const char* ssid = "Mbah Doel 5";
const char* password = "ash12345678";
//WifiClient client
char server[] = "192.168.43.20"; 

WiFiClient client;

void setup()
{
  Serial.begin(115200);
  delay(10);
  dht.begin();
//  connect to WiFi network 
  Serial.println();
  Serial.println();
  Serial.print("Connection ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED){
    delay(600);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

//  start the server
//server begin();
Serial.println("Server started");
Serial.print(WiFi.localIP());
delay(1000);
Serial.println("connecting...");
  
}

void loop()
{
  humidityData =  dht.readHumidity();
  temperatureData = dht.readTemperature();
  Sending_To_phpmyadmindatabase();
  delay(30000); //interval
}

void Sending_To_phpmyadmindatabase() //connecting with mysql
{
  if (client.connect(server, 001)) {
    Serial.println("connected");
//    Make a HTTP request;
    Serial.print("GET /dht/dht11.php?humidity");
    client.print("GET /dht/dht11.php?humidity");
    Serial.println(humidityData);
    client.print(humidityData);
    client.print("$temperature");
    Serial.println("$temperature");
    client.print(temperatureData);
    Serial.println(temperatureData);
    client.print(" "); //space before http/
    client.print("HTTP/1/1");
    client.println();
    client.println("Host: 192.168.43.106");
    client.println("Connection: close");
    client.println();
  }else{
//    if you didn't get A connection to the server 
  Serial.println("connection failed");
  }
  
}
