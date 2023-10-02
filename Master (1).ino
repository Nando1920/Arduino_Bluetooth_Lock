#include <SoftwareSerial.h>
#define blueLed 13
#define buttonPin 12
#define pass_lgth 4


SoftwareSerial BT(10, 11); 
int LDR = A0;
int sensorValue;
int btnState;
bool isLocked=false;
char password[] = "****";
char initial_password[]= "1234";
bool lockedDoor = false;
char a=0; // stores incoming character from other device

void setup()  
{
  // set digital pin 
  pinMode(blueLed, OUTPUT);
  pinMode(buttonPin, INPUT);
  
  // set the data rate for the Serial port
  BT.begin(9600);
  Serial.begin(38400);
 
}

void loop() 
{
  //initialize and delay Master inputs
  sensorValue = analogRead(LDR);
  btnState = digitalRead(buttonPin);
  delay(300);
  
  //Check for bluetooth messages from slave
  if (BT.available()){
      a = BT.read();
      Serial.println(a);
    }
  
  
  if (btnState == 1){
       BT.println('1');
       delay(300);
       if(lockedDoor){
          lockedDoor = false;
        }
       
    }

 
 

  Serial.println(lockedDoor);
  Serial.println(sensorValue);
  //Lock device in night mode only if its unlocked
  if(sensorValue<100 && !lockedDoor){
      
      BT.println('1');
      Serial.println(lockedDoor);
      lockedDoor = true;
      delay(600);
    }

  //Proximity Detected
  if(a=='2'){
      digitalWrite(blueLed,HIGH);
      delay(500);
      digitalWrite(blueLed,LOW);
      a=0;
    }

  //Password Received
  if(a=='3'){
      int i = 0;
      //Loop to retrieve all characters of the password
      while (BT.available()>0){
        a = BT.read();
        if(a != ' '){
          password[i] = a;
          i++;
        }
      }
      //compare input with actual password
      if(!strcmp(password,initial_password)){
        Serial.println("correct");
        BT.println('1');
        if(lockedDoor){
          lockedDoor = false;
        }
        }
      else{
          Serial.println("incorrect");
          Serial.println(password);          
        }
      a=0;
    }
}
