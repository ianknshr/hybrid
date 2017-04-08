#include <SPI.h> //include SPI Library
#include <Servo.h> //include servo

//set pins
const int slaveSelectPin = 11;  //Master output Slave input
const int servoPin = 9;

//PWM parameters
const int MaxPW = 2100;
const int MinPW = 900;

//Set starting angle
const int StartAngle = 90;

//initialize servo
Servo ETCservo;

byte InputSPI; // SPI input containers
int angle;     //servo angle

void setup() {

  //attatch Servo to Pin 9
  ETCservo.attach(servoPin, MinPW, MaxPW);

  //Set Servo to starting locaton
  ETCservo.write(0);

  
}

void loop() {

  int i;
for (i=0;i<=180;i+=1) {
ETCservo.write(i);
delay(7);
}

for (i=180;i>=0;i-=1) {
ETCservo.write(i);
delay(7);
}

}

