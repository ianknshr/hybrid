////Notes:
//You can only move this from 20 to 165 degrees. This is likely because of PWM compatibility problems with this servo.
//Right now the servo is controlled from pin 9
//Run the sweeptest() to see the servo moving
//Run the userinput() to go to a specific position. Enter the position into the serial monitor (type a space before the number you input). 
//Run the demoshift() to see the servo do some shifting commands
//Remember to set the constants to appropriate values

#include <Servo.h> 
#include <mcp_can.h>
#include <mcp_can_dfs.h>

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 9;

// Declare class for CAN Shield
MCP_CAN CAN(SPI_CS_PIN);

Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 

///////////////////////////////////////Constants
const int DOWN_POS = 150;      
const int UP_POS = 80;         
const int HALF_POS = 90;           
const int REST_POS = 105;           
const int DELAY_TIME = 800;
const int SWITCH_PIN = 10;

#define DOWN_SHIFT_ID   0x70
#define UP_SHIFT_ID     0x80
#define HALF_SHIFT_ID   0x90

int pos = 0;    // variable to store the servo position
String readString;

// variables used for CAN messages
unsigned char len = 0;
unsigned char buf[8];

////////////////////////////////////// Setup
void setup() 
{ 
  //important stuff
  myservo.attach(5);  // attaches the servo on pin 9 to the servo object 
  Serial.begin(9600);

  pinMode(10, OUTPUT);           // set pin to input
  digitalWrite(SWITCH_PIN,LOW);

  // init can bus : baudrate = 1000k
  while(CAN.begin(CAN_1000KBPS) == CAN_FAILINIT)
  {
      delay(100);
  }
   
  
} 

/////////////////////////////////////// Loop
void loop() 
{ 
  //userinput();
  CANinput();
} 

///////////////////////////////////// Shifting Function
void shift(int shift_type)
{
  switch (shift_type) 
  {
    case 0: //downshift
      digitalWrite(SWITCH_PIN, HIGH);
      myservo.write(DOWN_POS);
      delay(DELAY_TIME);
      myservo.write(REST_POS);
      delay(500);
      digitalWrite(SWITCH_PIN, LOW);
      break;
    case 1: //upshift
      digitalWrite(SWITCH_PIN, HIGH);
      myservo.write(UP_POS);
      delay(DELAY_TIME);
      myservo.write(REST_POS);
      delay(500);
      digitalWrite(SWITCH_PIN, LOW);
      break;
    case 2: //halfshift
      digitalWrite(SWITCH_PIN, HIGH);
      myservo.write(HALF_POS);
      delay(DELAY_TIME);
      myservo.write(REST_POS);
      delay(500);
      digitalWrite(SWITCH_PIN, LOW);
  }     
}


/////////////////////////////////// Extra code for testing stuff
void demoshift() //do a down, up, then half shift for demo purposes
{
  myservo.write(REST_POS);
  delay(1000);
  shift(0);
  delay(1000);
  shift(1);
  delay(1000);
  shift(2);
  delay(1500);
}

void sweeptest() //Sweep across the range of motion to confirm that the servo runs
{
  for(pos = 0; pos < 180; pos += 1)  // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(20);                       // waits 15ms for the servo to reach the position 
  } 
  for(pos = 180; pos>=1; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(20);                       // waits 15ms for the servo to reach the position 
  } 
}

void userinput() //set the angle using the USB serial (copied from internet)
{
   while (Serial.available()) {
      delay(1);  
      if (Serial.available() >0) {
        char c = Serial.read();  //gets one byte from serial buffer
        readString += c; //makes the string readString
      } 
    }

    if (readString.length() >0) {
      Serial.println(readString);  //so you can see the captured string 
      int n;
      char carray[8]; //converting string to number
      readString.toCharArray(carray, sizeof(carray));
      //n = atoi(carray); 
      //myservo.writeMicroseconds(n); // for microseconds
      //Serial.println(n);
      //myservo.write(n); //for degees 0-180
      //readString="";
      switch(carray[0]) {
        case 'u':
          shift(1);
          break;
        case 'd':
          shift(0);
          break;
        case 'h':
          shift(2);
          break;
        defualt:
          Serial.println("I dont do that");
          break;
      }
      readString="";
      
      
    } 
}

void CANinput() //set the angle using the USB serial (copied from internet)
{
  while(CAN_MSGAVAIL == CAN.checkReceive())            // check if data coming
  {
    CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
    unsigned long canId = CAN.getCanId();            // get CAN id to match to task
        
    switch(canId)
    {
      case DOWN_SHIFT_ID:
        shift(0);
        break;
      case UP_SHIFT_ID:
        shift(1);
        break;
      case HALF_SHIFT_ID:
        shift(2);
        break;
      defualt:
        Serial.println("I dont do that");
        break;
    }    
  } 
}
