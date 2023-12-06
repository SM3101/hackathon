int trig = 7;
int echo = 6;

int buzzer = 4;

int switch_input = 2;

int greenPin = A1;

int redPin = A0;

int yellowPin = A2;

int timeInMicro;

int distanceInCm;

//void redLEDOn(){
  //digitalWrite(redPin1, HIGH);
  //digitalWrite(redPin2, LOW);
  //delay(500);
  //digitalWrite(redPin1, LOW);
  //digitalWrite(redPin2, HIGH);
  //delay(500);
//}

//void greenLEDOn(){
  //digitalWrite(greenPin1, HIGH);
  //digitalWrite(greenPin2, LOW);
  //delay(500);
  //digitalWrite(greenPin1, LOW);
  //digitalWrite(greenPin2, HIGH);
  //delay(500);
//}

#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(switch_input, INPUT);
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();

}

void loop() {
  // put your main code here, to run repeatedly:

  int switch_state = digitalRead(switch_input);
  
  if (switch_state == 1){
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);

    timeInMicro = pulseIn(echo, HIGH);

    distanceInCm = timeInMicro/29/2;
    //Serial.println(distanceInCm);
    delay(100);

    if (distanceInCm < 50){
      digitalWrite(yellowPin, HIGH);

      //RFID Starts

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
      //Serial.print("UID tag :");
      String content= "";
      byte letter;
      for (byte i = 0; i < mfrc522.uid.size; i++) 
      {
        //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        //Serial.print(mfrc522.uid.uidByte[i], HEX);
        content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
        content.concat(String(mfrc522.uid.uidByte[i], HEX));
      }
      Serial.println();
      //Serial.print("Message : ");
      content.toUpperCase();
      if (content.substring(1) == "45 5F AF 15") //change here the UID of the card/cards that you want to give access
      {
        Serial.println("Authorized access");
        Serial.println();
        digitalWrite(yellowPin, LOW);
        digitalWrite(greenPin, HIGH);
        digitalWrite(buzzer, HIGH);
        delay(100);
        digitalWrite(buzzer, LOW);
        delay(100);
        digitalWrite(buzzer, HIGH);
        delay(100);
        digitalWrite(buzzer, LOW);
        delay(3000);
        digitalWrite(greenPin, LOW);
      }
 
      else{
        Serial.println(" Access denied");
        Serial.println();
        digitalWrite(yellowPin, LOW);
        digitalWrite(redPin, HIGH);
        digitalWrite(buzzer, HIGH);
        delay(3000);
        digitalWrite(redPin, LOW);
        digitalWrite(buzzer, LOW);
      }
      //RFID Ends
    }

    else{
      digitalWrite(yellowPin, LOW);
    }
  }
  else{
    digitalWrite(trig, LOW);
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, LOW);
    digitalWrite(yellowPin, LOW);
    digitalWrite(buzzer, LOW);
  }

  delay(10);

}