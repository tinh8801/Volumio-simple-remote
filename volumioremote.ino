/*
 * --------------------------------------------------------------------------------------------------------------------
 * Example sketch/program showing how to read data from a PICC to serial.
 * --------------------------------------------------------------------------------------------------------------------
 * This is a MFRC522 library example; for further details and other examples see: https://github.com/miguelbalboa/rfid
 * 
 * Example sketch/program showing how to read data from a PICC (that is: a RFID Tag or Card) using a MFRC522 based RFID
 * Reader on the Arduino SPI interface.
 * 
 * When the Arduino and the MFRC522 module are connected (see the pin layout below), load this sketch into Arduino IDE
 * then verify/compile and upload it. To see the output: use Tools, Serial Monitor of the IDE (hit Ctrl+Shft+M). When
 * you present a PICC (that is: a RFID Tag or Card) at reading distance of the MFRC522 Reader/PCD, the serial output
 * will show the ID/UID, type and any data blocks it can read. Note: you may see "Timeout in communication" messages
 * when removing the PICC from reading distance too early.
 * 
 * If your reader supports it, this sketch/program will read all the PICCs presented (that is: multiple tag reading).
 * So if you stack two or more PICCs on top of each other and present them to the reader, it will first output all
 * details of the first and then the next PICC. Note that this may take some time as all data blocks are dumped, so
 * keep the PICCs at reading distance until complete.
 * 
 * @license Released into the public domain.
 * 
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */

#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          D8         // Configurable, see typical pin layout above
const char *ssid =  "SSID"; // change according to your Network - cannot be longer than 32 characters!
const char *pass =  "PASSWORD"; // change according to your Network
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
const char* host = "192.168.1.4";
  const int httpPort = 80;
void setup() {
    Serial.begin(9600);
    Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  int retries = 0;
  while ((WiFi.status() != WL_CONNECTED) && (retries < 10)) {
  retries++;
  delay(500);
  Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED) {
  Serial.println(F("WiFi connected"));
  }
  
  Serial.println(F("Ready!"));
  Serial.println(F("======================================================")); 
  Serial.println(F("Scan for Card and print UID:"));
  WiFiClient client;
		// Initialize serial communications with the PC
	while (!Serial);		// Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
	SPI.begin();			// Init SPI bus
	mfrc522.PCD_Init();		// Init MFRC522
	mfrc522.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
  
	Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
//   if (!client.connect(host, httpPort)) {
//    Serial.println("connection failed");
//    return;
//  }
}

void loop() 
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  // WiFiClient client;
  if (content.substring(1) == "04 35 AF 52 EC 4A 81") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Serial.println();
   if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    Serial.println("wifi est connecte proceed to request");
    HTTPClient http;  //Declare an object of class HTTPClient
 
    http.begin("http://192.168.1.4/api/v1/commands/?cmd=play");  //Specify request destination
    int httpCode = http.GET();                                                                  //Send the request
 
    if (httpCode > 0) { //Check the returning code
 
      String payload = http.getString();   //Get the request response payload
      Serial.println(payload);                     //Print the response payload
 
    }
 
    http.end();   //Close connection
    
      }
      else {
        // if you couldn't make a connection:
        Serial.println("connection failed");
      }
    delay(3000);
  }
  else if  (content.substring(1) == "04 55 AC 52 EC 4A 81"){
   
    Serial.println("Authorized access");
    Serial.println();
   if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    Serial.println("wifi est connecte proceed to request");
    HTTPClient http;  //Declare an object of class HTTPClient
 
    http.begin("http://192.168.1.4/api/v1/commands/?cmd=pause");  //Specify request destination
    int httpCode = http.GET();                                                                  //Send the request
 
    if (httpCode > 0) { //Check the returning code
 
      String payload = http.getString();   //Get the request response payload
      Serial.println(payload);                     //Print the response payload
 
    }
 
    http.end();   //Close connection
    
      }
      else {
        // if you couldn't make a connection:
        Serial.println("connection failed");
      }
    delay(3000); 
  
  }
 else   {
    Serial.println(" Access denied");
    delay(3000);
  }
} 
