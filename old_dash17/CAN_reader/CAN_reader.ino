#include <mcp_can.h>
#include <mcp_can_dfs.h>
#include <SPI.h>


// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 9;

// Declare class for CAN Shield
MCP_CAN CAN(SPI_CS_PIN);


unsigned char buf[8] = {1,2,3,4,5,6,7,8};
unsigned char len;
unsigned int canID;


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

void loop() {

  //CAN.sendMsgBuf(0x20, 0, 8, buf);
  //Serial.println("message sent...");
  //delay(1000);
  

  if(CAN_MSGAVAIL == CAN.checkReceive())            // check if data coming
  {
    //Serial.println("message here");
    CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
    canID = CAN.getCanId(); //get CAN id for switch
    Serial.print("CAN ID: 0x");
    Serial.print(canID, HEX);
    for(int i=0; i<len; i++)
    {
      Serial.print(" 0x");
      Serial.print(buf[i], HEX);
    }
    Serial.println();
  }

}
