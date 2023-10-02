#include <SoftwareSerial.h>
#include <Keypad.h>
#include<EEPROM.h>
#include <Stepper.h>
#define TRIGPIN 7
#define ECHOPIN 8
#define redLed 6
#define greenLed 5
#define step_1 38
#define step_2 40
#define step_3 42
#define step_4 44

#define pass_lgth 4

SoftwareSerial BT(10, 11); 
// creates a "virtual" serial port/UART
// connect BT module TX to D10
// connect BT module RX to D11
// connect BT Vcc to 5V, GND to GND
float duration=0;
float distance_cm=0;
float distance_inch=0;
bool isLocked = false;
char a;
char password[] = "3****";


const byte ROWS = 4; 
const byte COLS = 4; 
int charCount=0;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {35, 37, 39, 41}; 
byte colPins[COLS] = {43, 45, 47, 49}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);   
Stepper motor(60, step_1, step_2, step_3, step_4); 

void setup() {
  pinMode(step_1, OUTPUT);
  pinMode(step_2, OUTPUT);
  pinMode(step_3, OUTPUT);
  pinMode(step_4, OUTPUT);
  pinMode(ECHOPIN, INPUT);
  pinMode(TRIGPIN, OUTPUT);
  pinMode(greenLed, OUTPUT);   
  pinMode(redLed, OUTPUT);
  // set the data rate for the SoftwareSerial port
  BT.begin(9600);
  
  Serial.begin(38400);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  int distanceLock = echoState();
  char customKey = customKeypad.getKey();
  
  
  if (distanceLock<100){
      BT.write('2');
    }
  
  if (customKey){
    Serial.write(customKey);
    password[charCount+1] = customKey;
    charCount++;
  }

  if (charCount == pass_lgth){
      
      
      BT.write(password);
      
      while(charCount != 0){
          password[charCount--]=0;
        }
    }
  
  if (BT.available()){
      a = BT.read();
      Serial.write(a);
    }
  
 
  if(a == '1'){
      switcher();
    }
 
}

int echoState(){
  digitalWrite(TRIGPIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGPIN,HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN,LOW); 
  duration = pulseIn(ECHOPIN, HIGH); // return 0 if no high pulse in 1sec. duration is in miliseconds
  
  //Serial.print(duration);
  //Serial.println("milliseconds");
  distance_cm=(duration/29)/2; 
  //Serial.print(distance_cm);
  //Serial.println("cm");
  delay(500);
  return distance_cm;
    }

void switcher(){
    if(!isLocked){
        digitalWrite(redLed,HIGH);
        digitalWrite(greenLed,LOW);
        isLocked = true;
        motor.step(15);
        a=0;
      }
    else if(isLocked){
        digitalWrite(greenLed,HIGH);
        digitalWrite(redLed,LOW);
        isLocked = false;
        motor.step(-15);
        a=0;
      }
  }
