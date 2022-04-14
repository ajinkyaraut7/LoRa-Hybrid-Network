/*
 * Level 1 - 03 node
 * Path to master : 03] -> 01
 * accept incoming from  05
 * Frame: [ message type (0 - control, 1 - data) | sender's address | previous sender's address | receiver's address | data_length | data ]
 */


#include <SPI.h>
#include <LoRa.h> 

#define potPin A0


byte thisNode = 0x03;

byte masterNode = 0x01;
byte node2 = 0x02;
byte node4 = 0x04;
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

String thisNodePot = ".";
String node5pot = ".";

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
  thisNodePot = (String) map(val, 0, 1024, 0, 254);
  // parse for a packet, and call onReceive with the result:
  onReceive(LoRa.parsePacket());
}


void sendMessage(int type, String data, byte snAdd, byte rcAdd){
  LoRa.beginPacket();  
  LoRa.write(type);   // type 0 - control signal and 1 - data signal
  LoRa.write(snAdd); // original sender's address
  LoRa.write(thisNode); // latest sender's address
  LoRa.write(rcAdd); // receiver's address
  LoRa.write(data.length());
  LoRa.print(data);
  LoRa.endPacket();
  delay(100);
 }

 void onReceive(int packetSize) {
  if (packetSize == 0) return;          // if there's no packet, return
 
  // read packet header bytes:
  int type = LoRa.read();       // type 0 - control signal and 1 - data signal
  int sAdd = LoRa.read();          // original sender's address
  byte prevSAdd = LoRa.read();            // sender's address
  byte rAdd = LoRa.read();     // incoming msg ID
  int incomingLength = LoRa.read();    // incoming msg length
 
  String incoming = "";
 
  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }
  Serial.println("*****type:" + (String) type);
  if(type == 0 && sAdd == masterNode && incoming == "node3"){
    delay(100);
    sendMessage(1, thisNodePot, thisNode, masterNode);
    delay(100);
    sendMessage(1, node5pot, node5, masterNode);
    delay(100);
    Serial.println("Message sent to master");
    Serial.println((String)thisNodePot + ":" + (String) thisNode + ":" + (String) masterNode );
    Serial.println(node5pot + ":" + (String) node5 + ":" + (String) masterNode );
  }
  else if (incomingLength != incoming.length()) {   // check length for error
    Serial.println("error: message length does not match length");                             
  }
  else if(rAdd == masterNode && sAdd == node5 && type == 1){
    node5pot = incoming;
  }
  else{
    Serial.println("Undetected if else condition");
  }
}
