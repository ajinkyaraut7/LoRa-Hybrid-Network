/*
 * // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis() / 1000);
 */
 /*
 * Level 0 - 01/master node
 * Path to master : N/A
 * accept incoming from  02 and 03
 * Frame: [ message type (0 - control, 1 - data) | sender's address | previous sender's address | receiver's address | data_length | data ]
 */


#include <LiquidCrystal.h>
#include <SPI.h>
#include <LoRa.h> 

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 8, en = 7, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

 
byte thisNode = 0x01;

byte node4 = 0x04;
byte node2 = 0x02;
byte node3 = 0x03;
byte node5 = 0x05;
byte node6 = 0x06;

String SenderNode = "";
String outgoing;              // outgoing message
 
byte msgCount = 0;            // count of outgoing messages
 
// Tracks the time since last event fired
unsigned long previousMillis=0;
unsigned long int previoussecs = 0; 
unsigned long int currentsecs = 0; 
unsigned long currentMillis = 0;
int interval= 1 ; // updated every 1 second
int Secs = 0; 

String node2pot = ".";
String node3pot = ".";
String node4pot = ".";
String node5pot = ".";
String node6pot = ".";

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Initializing...");
  delay(1000);
  lcd.clear();

  //init LoRa
  Serial.begin(9600);
 
  while (!Serial);
  Serial.println("LoRa Receiver");
  if (!LoRa.begin(433E6)) { // or 915E6
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  currentMillis = millis();
   currentsecs = currentMillis / 1000; 
    if ((unsigned long)(currentsecs - previoussecs) >= interval) {
     Secs = Secs + 1;
     Serial.println("***" + (String)Secs);
     if ( Secs >= 11 )
    {
      Secs = 0; 
    }
    if ( (Secs >= 1) && (Secs <= 5) )
    { 
      // transmit "node2"
      sendMessage(0, "node2", thisNode, node2);
    }
 
        if ( (Secs >= 6 ) && (Secs <= 10))
    {
      // transmit "node3"
      sendMessage(0, "node3", thisNode, node3);
    }
    
   previoussecs = currentsecs;
    }
  onReceiving(LoRa.parsePacket());
  lcdDisplay();
}


void onReceiving(int packetSize){
    if (packetSize == 0) return;          // if there's no packet, return
 
  // read packet header bytes:
  int type = LoRa.read();       // 0 - control signal and 1 - data signal
  int sAdd = LoRa.read();          // original sender's address
  byte prevSAdd = LoRa.read();            // sender's address
  byte rAdd = LoRa.read();     // incoming msg ID
  int incomingLength = LoRa.read();    // incoming msg length
 
  String incoming = "";
 
  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }
//  lcd.print(incoming);
//  Serial.println(incoming);
  if (incomingLength != incoming.length()) {   // check length for error
    Serial.println("error: message length does not match length");  
  }

  else if (rAdd == thisNode && sAdd == node2 && type == 1){
    node2pot = incoming;
    Serial.println("Node2: "+ node2pot); 
  }
  else if(rAdd == thisNode && sAdd == node3 && type == 1){
    node3pot = incoming;
    Serial.println("Node3: "+ node3pot); 
  }
  else if(rAdd == thisNode && sAdd == node4 && type == 1 && prevSAdd == node2) {
    node4pot = incoming;    
    Serial.println("Node4: "+ node4pot);   
  }
  else if(rAdd == thisNode && sAdd == node5 && type == 1 && prevSAdd == node3) {
    node5pot = incoming;    
    Serial.println("Node5: "+ node5pot); 
  }
  else if(rAdd == thisNode && sAdd == node6 && type == 1 && prevSAdd == node3) {
    node6pot = incoming;     
    Serial.println("Node6: "+ node6pot);  
  }
  else {
    Serial.println("Error: Random message received:"+ incoming);
  }
}


void sendMessage(int type, String data, byte snAdd, byte rcAdd){
  LoRa.beginPacket();  
  LoRa.write(type);   // type of signal
  LoRa.write(snAdd); // original sender's address
  LoRa.write(thisNode); // latest sender's address
  LoRa.write(rcAdd); // receiver's address
  LoRa.write(data.length());
  LoRa.print(data);
  LoRa.endPacket();
  delay(100);
 }

void lcdDisplay(){
  lcd.setCursor(0,0);
  lcd.print(node2pot);
  lcd.setCursor(3,0);
  lcd.print(" | ");
  lcd.print(node3pot);
  lcd.setCursor(9,0);
  lcd.print(" | ");
  lcd.print(node4pot);
  lcd.setCursor(15,0);
  lcd.print(" | ");
  lcd.setCursor(0,1);
  lcd.print(node5pot);
  lcd.setCursor(3,1);
  lcd.print(" | ");
  lcd.print(node6pot);
  lcd.setCursor(9,1);
  lcd.print(" | ");
  lcd.print((String)Secs);
}
 
