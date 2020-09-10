#include <Firebase.h>
#include <FirebaseArduino.h>
#include <FirebaseCloudMessaging.h>
#include <FirebaseError.h>
#include <FirebaseHttpClient.h>
#include <FirebaseObject.h>

#include <ESP8266WiFi.h>
#include <SPI.h>
#include "MFRC522.h"


#define FIREBASE_HOST "pj-alpha-f27c6.firebaseio.com"
#define FIREBASE_AUTH "K7KM7QmAJ36bee25JGRGXf4HMWy4pCmpGZaMkLbF"
#define RST_PIN  5 // RST-PIN für RC522 - RFID - SPI - Modul GPIO5 
#define SS_PIN  15  // SDA-PIN für RC522 - RFID - SPI - Modul GPIO4 

const char *ssid =  "id";     // change according to your Network - cannot be longer than 32 characters!
const char *pass =  "password"; // change according to your Network
String card="";
const int pinSensor = 2;
const int pinLed = 4;
int ismotion;
int tag[]={
  46,46,134,137 
};
bool access = false;
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

void setup() {
  Serial.begin(115200); 
  pinMode (pinSensor, INPUT); //定義 Sensor
  pinMode (pinLed, OUTPUT);   //定義 LED
  SPI.begin();           // Init SPI bus
  mfrc522.PCD_Init();    // Init MFRC522
  WiFi.begin(ssid,pass);
  while(WiFi.status() != WL_CONNECTED) {
Serial.print(".");
delay(500);
}
 if (WiFi.status() == WL_CONNECTED) {
    Serial.println(F("WiFi connected"));
  }
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
Firebase.set("detect",0);
  Serial.println(F("Ready!"));
  Serial.println(F("======================================================")); 
  Serial.println(F("Scan for Card and print UID:"));
}
void loop(){

  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
      byte *id = mfrc522.uid.uidByte;   // 取得卡片的UID
      byte idSize = mfrc522.uid.size;   // 取得UID的長度
 
      Serial.print("PICC type: ");      // 顯示卡片類型
      // 根據卡片回應的SAK值（mfrc522.uid.sak）判斷卡片類型
      MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
      Serial.println(mfrc522.PICC_GetTypeName(piccType));
 
      Serial.print("UID Size: ");       // 顯示卡片的UID長度值
      Serial.println(idSize);
     
      for (byte i = 0; i < idSize; i++) { 
         card += id[i];
      }
      Serial.print(card);
       for(int i= 0; i<idSize;i++){
          if(id[i] != tag[i]){
            access= false;
            
          }else{
            access =true;
          }
       }
      
      
      if(access){
        Serial.println("true");
        digitalWrite(pinLed,LOW);
        Firebase.set("detect",0);
        upload();
      delay(60000);
      }else{

        
        Serial.println("false");
        
      }
  
      mfrc522.PICC_HaltA();  // 讓卡片進入停止模式
     
     
     
  }else{
    detect();
  }
  card ="";
}
void upload(){

  const uint16_t port = 80;
const char * host = "192.168.43.191";
  WiFiClient client;
 
if (!client.connect(host, port)) {
Serial.println("connection failed");
Serial.println("wait 5 sec...");
delay(5000);
return;
}
 
  String url = "/rc522.php?card="+card;
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
  "Host: " + host + "\r\n" +
  "User-Agent: BoardDetectorESP8266davidouHomeSystem\r\n" +
  "Connection: close\r\n\r\n");
 
Serial.println("連線關閉");
client.stop();
Serial.print("wait");
delay(1000);
exit;
}
void detect(){
 ismotion = digitalRead(pinSensor);  // 從sensor讀資料
  if(ismotion == 1){       
    Firebase.setInt("detect",1);
    digitalWrite(pinLed,HIGH);
    delay(2000);// Led 燈亮
   Serial.println("detectived");
   if (Firebase.failed()) {
    Serial.println("streaming error");
    Serial.println(Firebase.error());
  }

  }
  else{
    noTone(pinLed); 
    Serial.println("Not Detectived"); 
    Firebase.set("detect",0);
  }
} 


