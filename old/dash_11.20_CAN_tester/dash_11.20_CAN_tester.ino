#include <mcp_can.h>
#include <mcp_can_dfs.h>
#include <SPI.h>


// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 9;

// Declare class for CAN Shield
MCP_CAN CAN(SPI_CS_PIN);

int x=0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // init can bus : baudrate = 1000k
  while(CAN.begin(CAN_1000KBPS) == CAN_FAILINIT)
  {
      delay(100);
  }

  Serial.println("Begin");

}

unsigned char buf[8];
unsigned char len;

void loop() {

  if(CAN_MSGAVAIL == CAN.checkReceive())            // check if data coming
  {
    //Serial.println("message here");
    CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
    canID = CAN.getCanId(); //get CAN id for switch
    Serial.print("CAN ID: ");
    Serial.print(canID);
    for(int i=0; i<len; i++)
    {
      Serial.print(" ");
      Serial.print(buf[i]);
    }
    Serial.println();
  }
  

}
