#include <MFRC522.h>

#include <MFRC522Extended.h>

#include <deprecated.h>

#include <require_cpp11.h>

#include <SPI.h>

#include <MFRC522.h>



#define RST_PIN 9 // Configurable, see typical pin layout above

#define SS_PIN 10 // Configurable, see typical pin layout above



MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance





//bluetooth stuff:
int const RX_PIN = 3; //receiving bluetooth
int const TX_PIN = 4; //transmitting bluetooth
#define RED_LED 2
#define GREEN_LED 5
#define BUZZER 6

#include <SoftwareSerial.h> //include library

SoftwareSerial tooth(TX_PIN, RX_PIN); //create a softwareserial object, set tx and rx pins, tx goes first then rx

char a ; //an empty character to store 


//ultrasonic sensor stuff:
#define TRIGGER_PIN 4 //this is the pin that sends out the pulse
#define ECHO_PIN 3 //this is the pin that reads the distance

void setup() {



//RFID stuff:
Serial.begin(9600); // Initialize serial communications with the PC

while (!Serial); // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)

SPI.begin(); // Init SPI bus

mfrc522.PCD_Init(); // Init MFRC522

delay(4); // Optional delay. Some board do need more time after init to be ready, see Readme

mfrc522.PCD_DumpVersionToSerial(); // Show details of PCD - MFRC522 Card Reader details

Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));


//bluetooth stuff:
  Serial.begin(9600); // means can print on serial monitor
  tooth.begin(9600); //can print on device

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

//ultrasonic sensor stuff:
  pinMode(TRIGGER_PIN, OUTPUT); //output bc it sends pulse out
  pinMode(ECHO_PIN, INPUT); //bc it is reading what is coming in

}

void loop() {


digitalWrite(BUZZER, HIGH);
delay(100);
digitalWrite(BUZZER, LOW);
delay(100);


//RFID stuff :

 // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.

if ( ! mfrc522.PICC_IsNewCardPresent()) {

return;

}



 // Select one of the cards

if ( ! mfrc522.PICC_ReadCardSerial()) {

return;

}




MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);



Serial.print(F("RFID Tag UID:"));

printHex(mfrc522.uid.uidByte, mfrc522.uid.size);

Serial.println("");



mfrc522.PICC_HaltA(); // Halt PICC

}

void printHex(byte *buffer, byte bufferSize) {



 //Serial.begin("reading?");

for (byte i = 0; i < bufferSize; i++) {

Serial.print(buffer[i] < 0x10 ? " 0" : " ");

Serial.print(buffer[i], HEX);

}

}
