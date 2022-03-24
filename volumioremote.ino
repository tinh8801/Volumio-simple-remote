#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#define PLAY_PIN         14          //D5 cua nodemcu
#define STOP_PIN         12          //D6 cua nodemcu
#define NEXT_PIN         13          //D7 cua nodemcu
#define PREVI_PIN        5           //D1 cua nodemcu
const char *ssid =  "TP-LINK_9B3852"; //ten mang wifi
const char *pass =  "91890429"; //pass wifi
//const char* host = "192.168.2.102";//IP cua Volumio
//const int httpPort = 80;
WiFiClient client;
ESP8266WiFiMulti wifimulti;
void setup() {
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(9600);
  Serial.print("Dang ket noi mang...");//khoi tao ket noi Wifi
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  wifimulti.addAP(ssid,pass);//ten va pass Wifi (SUA CHO NAY)
  //int retries = 0;
  //while ((wifimulti.run() != WL_CONNECTED) && (retries < 10)) {
  //retries++;
  //delay(500);
  //Serial.print("*");
  //}
  if (wifimulti.run() == WL_CONNECTED) {
  digitalWrite(LED_BUILTIN,HIGH); //den led tren nodemcu nhay 3 lan bao hieu ket noi thanh cong
  delay(500);
  digitalWrite(LED_BUILTIN,LOW);
  delay(500);
  digitalWrite(LED_BUILTIN,HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN,LOW);
  delay(500);
  digitalWrite(LED_BUILTIN,HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN,LOW);
  delay(500);
  digitalWrite(LED_BUILTIN,HIGH);
  Serial.println(F("Ket noi thanh cong (^-^)"));
  Serial.println(F("======================================================"));
  }else{
  digitalWrite(LED_BUILTIN,LOW);
  Serial.println(F("Ket noi that bai (#_#)"));
  
  }
  pinMode(PLAY_PIN,INPUT_PULLUP);//thiet lap cac chan I/O la ngo vao  
  pinMode(STOP_PIN,INPUT_PULLUP);
  pinMode(NEXT_PIN,INPUT_PULLUP);
  pinMode(PREVI_PIN,INPUT_PULLUP);
 
	//while (!Serial);
	
//   if (!client.connect(host, httpPort)) {
//    Serial.println("connection failed");
//    return;
//  }
}
void ErrorLed(){//Led sang roi tat bao cac loi lien quan ket noi
    digitalWrite(LED_BUILTIN,LOW);
    delay(1000);
    digitalWrite(LED_BUILTIN,HIGH);
}

void loop() 
{
   
 if(digitalRead(PLAY_PIN)==LOW){
  delay(50);
  if((digitalRead(PLAY_PIN)==LOW)&&(WiFi.status()==WL_CONNECTED)){
    Serial.println("play/pause command");
    HTTPClient http;
    http.begin(client,"http://192.168.2.102/api/v1/commands/?cmd=toggle");  //chinh lai cho dung voi IP cua Volumio
    int httpCode = http.GET();                                                                
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println(payload);
    }else{
      ErrorLed();
    }
    http.end();
      }else{
        Serial.println("Khong co ket noi Wifi");
        ErrorLed();
        }
        delay(1000);      
   }
 
 if(digitalRead(STOP_PIN)==LOW){
  delay(50);
if((digitalRead(STOP_PIN)==LOW)&&(WiFi.status()==WL_CONNECTED)){
   Serial.println("stop command");
    HTTPClient http;   
    http.begin(client,"http://192.168.2.102/api/v1/commands/?cmd=stop"); //chinh lai cho dung voi IP cua Volumio
    int httpCode = http.GET();                                                             
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println(payload);                     
    }else{
      ErrorLed();
    }
    http.end();
      }else{
        Serial.println("Khong co ket noi Wifi");
        ErrorLed();
        }
        delay(1000);
  }

 if(digitalRead(NEXT_PIN)==LOW){
  delay(50);
 if((digitalRead(NEXT_PIN)==LOW)&&(WiFi.status()==WL_CONNECTED)){
    Serial.println("next command");
    HTTPClient http;
    http.begin(client,"http://192.168.2.102/api/v1/commands/?cmd=next");  //chinh lai cho dung voi IP cua Volumio
    int httpCode = http.GET();                                                        
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println(payload);
    }else{
      ErrorLed();
    }
    http.end();
      }else{
        Serial.println("Khong co ket noi Wifi");
        ErrorLed();
        }
        delay(1000);
  }

 if(digitalRead(PREVI_PIN) == LOW){
  delay(50);
 if((digitalRead(PREVI_PIN)==LOW)&&(WiFi.status()==WL_CONNECTED)){
    Serial.println("prev command");
    HTTPClient http;
    http.begin(client,"http://192.168.2.102/api/v1/commands/?cmd=prev");  //chinh lai cho dung voi IP cua Volumio
    int httpCode = http.GET();                                                           
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println(payload);                 
    }else{
      ErrorLed();
    }
    http.end();
      }else{
        Serial.println("Khong co ket noi Wifi");
        ErrorLed();
        }
        delay(1000);
  }
}
