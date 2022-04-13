/*
 * Level 2 - 04 node
 * Path to master : 04] -> 02 -> 01
 * accept incoming from  05
 * Frame: [  sender's address | previous sender's address | receiver's address | data_length | data ]
 */


#include <SPI.h>
#include <LoRa.h> 

#define potPin A0


byte thisNode = 0x04;

byte masterNode = 0x01;
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

int thisPot = 0;
int receivedPot = 0;

void setup() {
  
  Serial.begin(9600);
  
  //init LoRa
  while (!Serial);
  Serial.println("LoRa Receiver");
  if (!LoRa.begin(433E6)) { // or 915E6
    Serial.println("Starting LoRa failed!");
    while (1);
  }

}

void loop() {
  int val = analogRead(potPin);
      thisPot = map(val, 0, 1024, 0, 255);
      sendMessage(1, (String) thisPot, thisNode, masterNode);
}


void sendMessage(int type, String data, byte snAdd, byte rcAdd){
  delay(100);
  LoRa.beginPacket();  
  LoRa.write(type);
  LoRa.write(snAdd); // original sender's address
  LoRa.write(thisNode); // latest sender's address
  LoRa.write(rcAdd); // receiver's address
  LoRa.write(data.length());
  LoRa.print(data);
  LoRa.endPacket();
  delay(100);
 }
