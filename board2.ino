  #include "DHT.h"
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <BH1750.h>
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE); 
BH1750 lightMeter;
int measurePin = A0; //Connect dust sensor to Arduino A0 pin
int ledPower = 14;   //Connect 3 led driver pins of dust sensor to Arduino D2
  
int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;
  
float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;
String reading; 
long previousMillis = 0;
long interval = 10000;
WiFiServer server(80); 
void setup(){  
  Serial.begin(9600); 
   pinMode(ledPower,OUTPUT);
   WiFi.begin("dafuq", "58565856");
   WiFi.mode(WIFI_STA);
   dht.begin();
     pinMode(12, OUTPUT);
  digitalWrite(12, 0);
Wire.begin();
lightMeter.begin();
 
Serial.println();
Serial.println();
Serial.print("Wait for WiFi... ");

while(WiFi.status() != WL_CONNECTED) {
Serial.print(".");
delay(500);
}
 
Serial.println("");
Serial.println("[成功]WiFi 已連接");
Serial.print("IP address: ");
Serial.println(WiFi.localIP());
 server.begin();
  Serial.println("Server started");
  envir();
}
 String floatToString(float x, byte precision = 2) {
char tmp[50];
dtostrf(x, 0, precision, tmp);
return String(tmp);
}
void loop(){
  led();
    unsigned long currentMillis = millis();
 if(currentMillis - previousMillis > interval) {
   previousMillis = currentMillis;   
 envir();}
}
void led(){
WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  client.setTimeout(1000);
  Serial.println(req);
  
  
  // Match the request
  int val;
  if (req.indexOf("/gpio/0") != -1)
    val = 0;
  else if (req.indexOf("/gpio/1") != -1)
    val = 1;
  

  // Set GPIO2 according to the request
  digitalWrite(12, val);
  
 loop();
}
  
void envir(){
  temp();
  dust();
  light();  
  }
void temp(){
  float h = dht.readHumidity();
float t = dht.readTemperature();
float f = dht.readTemperature(true);
if (isnan(h) || isnan(t) || isnan(f)) {
Serial.println("Failed to read from DHT sensor!");
return;
}
Serial.print("濕Humidity: ");
Serial.print(h);
Serial.print("%\t");
Serial.print("溫: ");
Serial.print(t);
Serial.print("*C\t");
Serial.print(f);
Serial.print("*F\n");
 
const uint16_t port = 80;
const char * host = "192.168.43.191"; // ip or dns
Serial.print("連線至");
Serial.println(host);
 
// Use WiFiClient class to create TCP connections
WiFiClient client;
 
if (!client.connect(host, port)) {
Serial.println("connection failed");
Serial.println("wait 5 sec...");
delay(5000);
return;
}
 
  String url = "/dht11.php?celsius="+floatToString( dht.readTemperature())+"&humidity="+h+"&fahrenheit="+f;
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
  "Host: " + host + "\r\n" +
  "User-Agent: BoardDetectorESP8266davidouHomeSystem\r\n" +
  "Connection: close\r\n\r\n");
client.stop();
 
}
void dust(){
  digitalWrite(ledPower,LOW); // power on the LED
  delayMicroseconds(samplingTime);
  
  voMeasured = analogRead(measurePin); // read the dust value
  
  delayMicroseconds(deltaTime);
  digitalWrite(ledPower,HIGH); // turn the LED off
  delayMicroseconds(sleepTime);
  
  // 0 - 5V mapped to 0 - 1023 integer values
  // recover voltage
  calcVoltage = (voMeasured+120) * (5.0 / 1024.0);
  
  // linear eqaution taken from http://www.howmuchsnow.com/arduino/airquality/
  // Chris Nafis (c) 2012
  dustDensity = 0.17 * calcVoltage - 0.1;
  
  Serial.print("Raw Signal Value (0-1023): ");
  Serial.print(voMeasured+120);
  
  Serial.print(" - Voltage: ");
  Serial.print(calcVoltage);
  
  Serial.print(" - Dust Density: ");
  Serial.print(dustDensity * 1000); // 這裡將數值呈現改成較常用的單位( ug/m3 )
  Serial.println(" ug/m3 ");
  delay(1000);
  reading = String(dustDensity*1000);
  if((dustDensity*1000>0)&& (dustDensity*1000<300)){
  const uint16_t port = 80;
const char * host = "192.168.43.191";
WiFiClient client;
if (!client.connect(host, port)) {
Serial.println("connection failed");
Serial.println("wait 5 sec...");
delay(5000);
return;
}
 String url = "/dust.php?reading="+reading;
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
  "Host: " + host + "\r\n" +
  "User-Agent: BoardDetectorESP8266davidouHomeSystem\r\n" +
  "Connection: close\r\n\r\n");
client.stop();
  }
}
void light(){
  uint16_t lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  delay(1000);
   const uint16_t port = 80;
const char * host = "192.168.43.191"; 
WiFiClient client;
 
if (!client.connect(host, port)) {
Serial.println("connection failed");
Serial.println("wait 5 sec...");
delay(5000);
return;
}
String light;
light = String(lux);
 String url = "/temt6000.php?light="+light;
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
  "Host: " + host + "\r\n" +
  "User-Agent: BoardDetectorESP8266davidouHomeSystem\r\n" +
  "Connection: close\r\n\r\n");
client.stop();  

}


