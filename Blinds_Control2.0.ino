#include <Servo.h>

int pos = 0;
int forward = 1;
Servo myservo;
const int zwaveOn = 5; //Zwave module on D5
const int button = 4; //button on D4
const int transistorServo = 8; //Transistor on D8 to turn on and off servo while not in use.
const int zwavePWMPIN = 11;

int zwaveState = 0; //Reading button
int lastZwave = 0; //Int for last read Zwave
int buttonState = 0; //Reading button
int lastButton = 0; //int for last read Zwave

int lastPos = 1; //Saves last servo postion
int servoOff = 0; //Count the times servo is on so it can turn it off

int ch3;
int tempNum;
int ch3pre;
int prepos;
int posch;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  myservo.attach(9);
  pinMode(zwaveOn, INPUT);
  pinMode(transistorServo, OUTPUT);

  pinMode(11, INPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Position start");
  Serial.println(pos);

  //Check Button States
  buttonState = digitalRead(button); //Read button
  zwaveState = digitalRead(zwaveOn); //Read zwave chip
  
  //Zwave change Blinds
  if (zwaveState == HIGH && zwaveState != lastZwave) {
    //pos = 90; //TEMP OFF DUE TO USE IN LAST VERSION
    lastZwave = 1;
  }
  if (zwaveState == LOW && zwaveState != lastZwave) {
    pos = 0;
    lastZwave = 0;
  }

  if (lastZwave == 1) {
    zwavePWM();
  }
  /*
  //Zwave change position of 
  if (zwaveState == HIGH && zwaveState == lastZwave) {
    //place the code to check the PWM signal on D11 then divide by X amount then check if the amount changed my X amount. Then check what section of position it should refer to
  }
  */
  //Print Servo Position
  Serial.println("Position after");
  Serial.println(pos);
  
  //delay(1000);
  
  //posChange();
  //Serial.println(pos);
  //delay(1000);


  //Button servo button change
  if (buttonState == HIGH) {
    posChange();
    lastButton = 1;
  }
  if (buttonState == LOW) {
    lastButton = 0;
  }

  //Checks to see if there was a postion change
  if (pos != lastPos) { 
    digitalWrite(transistorServo, HIGH); //Turn on Servo transistor
    myservo.write(pos);
    delay(250);
    servoOff = 0; //reset transistor count
  }
  lastPos = pos;

  //turns servo off after 10 loops
  if (servoOff == 50) { 
    digitalWrite(transistorServo, LOW); //Turn off Servo transistor
  } else {
    servoOff ++;
  }
}

void posChange() { //changes position depending on direction servo has been traveling 
  if (pos == 0) {
    forward = 1;
  }
  if (pos == 180) {
    forward = 0;
  }
  if (forward == 1) {
    pos = pos + 10;
  } else {
    pos = pos - 10;
  }
}

void zwavePWM() {

  ch3 = pulseIn(zwavePWMPIN, HIGH, 25000);

  Serial.print("Channel 3:");
  Serial.println(ch3);
 // delay(100); // I put this here just to make the terminal 
              // window happier
  if (ch3 > 750) {
    prepos = 10;
  }
  else if (ch3 > 660) {
    prepos = 20;
  }
  else if (ch3 > 586) {
    prepos = 40;
  }
  else if (ch3 > 490) {
    prepos = 60;
  }
  else if (ch3 > 410) {
    prepos = 80;
  }
  else if (ch3 > 324) {
    prepos = 100;
  }
  else if (ch3 > 240) {
    prepos = 120;
  }
  else if (ch3 > 150) {
    prepos = 140;
  }
  else if (ch3 > 70) {
    prepos = 160;
  }
  else {
    prepos = 180;
  }
  tempNum = abs(ch3-ch3pre);
  if  (tempNum > 30) {
    //pos = prepos;
    posch = 1;
  }
  if (posch >= 1) {
    if (posch >= 10) {
      pos = prepos;
      posch = 0;
    }
    else {
      posch++;
    }
  }
  ch3pre = ch3;
}
