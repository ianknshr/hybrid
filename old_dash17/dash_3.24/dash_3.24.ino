#include <mcp_can.h>
#include <mcp_can_dfs.h>
#include <SPI.h>
#include "SdFat.h"

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 10;

// SD chip select pin.  Be sure to disable any other SPI devices such as Enet.
const int SD_CS_PIN = 9;


// Declare class for CAN Shield
MCP_CAN CAN(SPI_CS_PIN);

//initialize SD stuff
SdFat SD;

File myFile;

//define pin mappings
//save pint 0 and 1 for RX/TX
const int THREE_WAY_PINL = 3;
const int THREE_WAY_PINR = 4;
const int FAN_PIN1 = 5;
const int FAN_PIN2 = 6;
const int HV_ACTIVE_BUTTON = 2; 
const int HV_ACTIVE_LED = A1;
const int SPARE_BLUE_PIN = A2;
const int SPARE_YELLOW_PIN = A3;


//init button variables
bool three_wayl = 0;
bool three_wayr = 0;
unsigned char last_three_way = 0;
bool HV_active_LED = 0;
unsigned char fan_1 = 0;
unsigned char fan_2 = 0; 

void setup() {
  

  //init pins
  pinMode(THREE_WAY_PINL, INPUT);
  digitalWrite(THREE_WAY_PINL, HIGH);
  pinMode(THREE_WAY_PINR, INPUT);
  digitalWrite(THREE_WAY_PINR, HIGH);
  pinMode(HV_ACTIVE_BUTTON, INPUT);
  digitalWrite(HV_ACTIVE_BUTTON, HIGH);
  //initialize  interupt for HV button
  //attachInterrupt(digitalPinToInterrupt(HV_ACTIVE_BUTTON), HV_ISR, RISING);
  pinMode(HV_ACTIVE_LED, OUTPUT);
  
  
  
  // put your setup code here, to run once:
  Serial.begin(9600);
  

  //Wait for USB Serial 
  while (!Serial) {
    SysCall::yield();
  }

  // init can bus : baudrate = 1000k
  while(CAN.begin(CAN_1000KBPS) == CAN_FAILINIT)
  {
      delay(100);
  }
  /*
  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output
  // or the SD library functions will not work.
  pinMode(10, OUTPUT);

  while (!SD.begin(9)) {
    Serial.println("initialization failed!");
    delay(1000);
  }
  Serial.println("initialization done.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  //myFile = SD.open("test.csv", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.csv...");
    myFile.print("values");
    for(int i =0; i<15; i++)
    {
      myFile.print(",values");
    }
    myFile.println();
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.csv");
  }
  
  //consider adding masks

  Serial.println("Begin");
  */

}

//dSPACE buffer: {HV_active,threeway}
unsigned char to_dSPACE[2] = {0,0};
unsigned char buf[8];
unsigned char len;
unsigned char tot[16];
unsigned int canID;
unsigned long currentMillis;
unsigned long previousMillis = 0;
const long interval = 1;

#define ID1 0x50
#define ID2 0x60
#define DASH_ID 0x65
#define FAN_ID 0x70
#define HV_ID 0x800
#define THREE_WAY_ID 0x90

void loop() {

  //read in values from dash
  three_wayl = LOW == digitalRead(THREE_WAY_PINL);
  three_wayr = LOW == digitalRead(THREE_WAY_PINR);

  //convert two threeway switch inputs into one value from 0-2;
  to_dSPACE[1] = (three_wayl==HIGH) + (three_wayr==HIGH)*2;

  //check for threeway change
  if(to_dSPACE[1] != last_three_way)
  {
    //send CAN packet
    CAN.sendMsgBuf(HV_ID, 0, 2, to_dSPACE);
    last_three_way = to_dSPACE[1];
  }

  currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    //invert value
    to_dSPACE[0] = LOW == digitalRead(HV_ACTIVE_BUTTON);
    //send CAN packet
    CAN.sendMsgBuf(HV_ID, 0, 2, to_dSPACE);
    // save the last time you blinked the LED
    previousMillis = currentMillis;
  }

  
  Serial.print("3Way l: ");
  Serial.print(three_wayl);
  Serial.print(" 3Way r: ");
  Serial.print(three_wayr);
  Serial.print(" 3Way out: ");
  Serial.print(to_dSPACE[1]);
  Serial.print(" HVA: ");
  Serial.println(to_dSPACE[0]);
  
  


  //add timer to send general message every certain amount of time
  
/*
  //check for can message from dSPACE
  if(CAN_MSGAVAIL == CAN.checkReceive())            // check if data coming
  {
    //Serial.println("message here");
    CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
    canID = CAN.getCanId(); //get CAN id for switch

    switch(canID)
    {
      case ID1:
      {
        //Serial.println("one");
        for(int i=0; i<len; i++)
        {
          tot[i] = buf[i];
        }
      }
      case ID2:
      {
        //Serial.println("two");
        for(int i=0; i<len; i++)
        {
          tot[i+8] = buf[i];
        }
      }
      case FAN_ID:
      {
        fan_1 = buf[0];
        fan_2 = buf[1];
      }
    }

    //Serial.println("message here");
    CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
    canID = CAN.getCanId(); //get CAN id for switch

    switch(canID)
    {
      case ID1:
      {
        //Serial.println("one");
        for(int i=0; i<len; i++)
        {
          tot[i] = buf[i];
        }
      }
      case ID2:
      {
        //Serial.println("two");
        for(int i=0; i<len; i++)
        {
          tot[i+8] = buf[i];
        }
      }
    }

    //HV active led
    digitalWrite(HV_ACTIVE_LED, *HV_active_b);

    //fan signal
    analogWrite(FAN_PIN1, fan_1);
    analogWrite(FAN_PIN2, fan_2);

    //send data through serial
    Serial.write(tot,16);

    //log data
      // open the file. note that only one file can be open at a time,
      // so you have to close this one before opening another.
    myFile = SD.open("test.csv", FILE_WRITE);

    // if the file opened okay, write to it:
    while (myFile) {
      for(int i =0; i<16; i++)
      {
        myFile.print(tot[i]);
        if(i!=15) myFile.print(',');
      }
      myFile.println();
      // close the file:
      myFile.close();
    }
    
  }
*/
}

/*
void HV_ISR() {
  //invert value
  to_dSPACE[0]= !to_dSPACE[0];
  //send CAN packet
  CAN.sendMsgBuf(HV_ID, 0, 2, to_dSPACE);
}
*/

