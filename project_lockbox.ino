//DO NOT WIRE THIS WITH 5V --> 3.3
//dont wire IRQ
//rst 9, miso 12, mosi 11, sca 13, sda 10
#include <MFRC522.h>

#include <MFRC522Extended.h>

#include <deprecated.h>

#include <require_cpp11.h>

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

 * MFRC522 Arduino Arduino Arduino Arduino Arduino

 * Reader/PCD Uno/101 Mega Nano v3 Leonardo/Micro Pro Micro

 * Signal Pin Pin Pin Pin Pin Pin

 * -----------------------------------------------------------------------------------------

 * RST/Reset RST 9 5 D9 RESET/ICSP-5 RST

 * SPI SS SDA(SS) 10 53 D10 10 10

 * SPI MOSI MOSI 11 / ICSP-4 51 D11 ICSP-4 16

 * SPI MISO MISO 12 / ICSP-1 50 D12 ICSP-1 14

 * SPI SCK SCK 13 / ICSP-3 52 D13 ICSP-3 15

 *

 * More pin layouts for other boards can be found here: https://github.com/miguelbalboa/rfid#pin-layout

 */

//zip

//step 1: dowlnoad zip (do not unzip) https://github.com/miguelbalboa/rfid

//step 2: sketch -> include libraries -> add zip

//setp 3: find the one we just downloaded and say open



//dump file

//flie -> examples -> mfrc522 -> dumpinfo



//wire with 3.3 v not 5v because that will fry it





#include <SPI.h>

#include <MFRC522.h>


#include <Servo.h>
#define SPIN_PIN 2
Servo spin;  //make a servo object called spin

#define RST_PIN 9  // Configurable, see typical pin layout above

#define SS_PIN 10  // Configurable, see typical pin layout above



MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance


//bluetooth stuff:
int const RX_PIN = 8;  //receiving bluetooth
int const TX_PIN = 7;  //transmitting bluetooth
#define GREEN_LED 5
#define BUZZER 6


#include <SoftwareSerial.h>  //include library

SoftwareSerial tooth(TX_PIN, RX_PIN);  //create a softwareserial object, set tx and rx pins, tx goes first then rx

String input = " ";  //an empty string to store


//ultrasonic sensor stuff:
#define TRIGGER_PIN 4  //this is the pin that sends out the pulse
#define ECHO_PIN 3     //this is the pin that reads the distance

void setup() {

  Serial.begin(9600);  // Initialize serial communications with the PC

  while (!Serial)
    ;  // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)

  SPI.begin();  // Init SPI bus

  mfrc522.PCD_Init();  // Init MFRC522

  delay(4);  // Optional delay. Some board do need more time after init to be ready, see Readme

  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details

  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));


  tooth.begin(9600);  //can print on device


  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);


  //ultrasonic sensor stuff:
  pinMode(TRIGGER_PIN, OUTPUT);  //output bc it sends pulse out
  pinMode(ECHO_PIN, INPUT);      //bc it is reading what is coming in

  spin.attach(2);

}



void loop() {
  spin.write(60);
  delay(8900);
  spin.write(120);
  delay(8900);


byte authorizedUID[] = { 0x96, 0xDE, 0x29, 0x03 };  // Example UID (change to your real UID)
byte uidLength = 4;                                 // Length of the UID
if (mfrc522.uid.size == uidLength) {
  boolean authorized = true;
  for (byte i = 0; i < uidLength; i++) {
    if (mfrc522.uid.uidByte[i] != authorizedUID[i]) {
      authorized = false;
      break;
    }
  }

  if (authorized) {
    Serial.println("Access granted.");
  }else{
    Serial.println("Wrong RFID. Access not granted.");
  }
}



  //ultrasonic sensor stuff

  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);  //this is how long it has to wait to send out a pulse

  float duration = pulseIn(ECHO_PIN, HIGH);  // tell us the time from pulse sent to pulse received
  Serial.println(duration);

  //v=s/t
  float speed = 0.034;  //measured in cm/microseconds
  float distance = (speed * duration) / 2;

  Serial.println("distance:");
  Serial.println(distance);
  delay(100);

  if (distance <= 10) {
    Serial.println("Step back! You're too close!");
    digitalWrite(BUZZER, HIGH);
    delay(200);
    digitalWrite(BUZZER, LOW);
    delay(200);
  }

  tooth.print("Enter your password:");

  if (tooth.available() > 0) {  //if something is inputed on device
    input = tooth.read();
    tooth.print("Password: ");
    tooth.println(input);
  }

  if (input != "0329") {
    digitalWrite(BUZZER, HIGH);
    delay(1000);
    digitalWrite(BUZZER, LOW);
    delay(100);
  } else if (input == "0329") {
    Serial.println("Password correct!");
    digitalWrite(GREEN_LED, HIGH);
    delay(1000);
    digitalWrite(GREEN_LED, LOW);
    delay(1000);
  }

  for (int i = 0; i < 5; i++) {  //beeps if the confirmation code was incorrect
    digitalWrite(BUZZER, HIGH);
    delay(200);
    digitalWrite(BUZZER, LOW);
    delay(200);
  }



  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.

  if (!mfrc522.PICC_IsNewCardPresent()) {

    return;
  }



  // Select one of the cards

  if (!mfrc522.PICC_ReadCardSerial()) {

    return;
  }




  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);



  Serial.print(F("RFID Tag UID:"));

  printHex(mfrc522.uid.uidByte, mfrc522.uid.size);

  Serial.println("");



  mfrc522.PICC_HaltA();  // Halt PICC
}

void printHex(byte *buffer, byte bufferSize) {



  //Serial.begin("reading?");

  for (byte i = 0; i < bufferSize; i++) {

    Serial.print(buffer[i] < 0x10 ? " 0" : " ");

    Serial.print(buffer[i], HEX);
  }
}


