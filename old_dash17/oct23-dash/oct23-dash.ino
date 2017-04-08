#include <mcp_can.h>
#include <mcp_can_dfs.h>
#include <SPI.h>
#include "dash.h"


// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 9;

// Declare class for CAN Shield
MCP_CAN CAN(SPI_CS_PIN);

//define pin mappings
//save pint 0 and 1 for RX/TX
const int THREE_WAY_PIN1 = 2;
const int THREE_WAY_PIN2 = 3;
const int HV_ACTIVE_PIN = 4; 
const int SPARE_BLUE_PIN = 5;
const int SPARE_YELLOW_PIN = 6;




//define CANids
const int ENG_START_ID = 0x20;
const int ENG_KILL_ID = 0x21;
const int THREE_WAY = 0x22;
const int SPARE_BLUE = 0x40;
const int SPARE_YELLOW = 0x401;

const int DASH_ID = 0x50;

//init button variables
int three_way1 = 0;
int three_way2 = 0;
int three_way_output = 0;
int HV_active =0;


void setup() {
  

  //init pins
  pinMode(ENG_START_PIN, INPUT);
  pinMode(ENG_KILL_PIN, INPUT);
  pinMode(THREE_WAY_PIN1, INPUT);
  pinMode(THREE_WAY_PIN2, INPUT);
  
  
  // put your setup code here, to run once:
  Serial.begin(115200);

  // init can bus : baudrate = 1000k
  while(CAN.begin(CAN_1000KBPS) == CAN_FAILINIT)
  {
      delay(100);
  }

  //consider adding masks

}

void loop() {

  //read in values from dash
  three_way1 = digitalRead(THREE_WAY_PIN1);
  three_way2 = digitalRead(THREE_WAY_PIN2);

  //convert two threeway switch inputs into one value from 0-2;
  three_way_output = (three_way1==HIGH) + (three_way2==HIGH)*2;
  

  //add timer to send general message every certain amount of time
  
  
  // put your main code here, to run repeatedly:
  unsigned char stmp[8] = {eng_start,eng_kill,three_way_output,0,0,0,0,0};
  
  CAN.sendMsgBuf(DASH_ID,0, 8, stmp);
  delay(1000);                       // send data per 100ms



  //check for can message from dSPACE
  //read in message if its there
  //store message
  //send message through serial at iMhz

}
