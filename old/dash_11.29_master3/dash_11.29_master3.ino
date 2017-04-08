//include can
#include <mcp_can.h>
#include <mcp_can_dfs.h>

#include <SoftwareSerial.h>

//SoftwareSerial mySerial(4,5); //RX=4 TX=5

const int SPI_CS_PIN = 10;

// Declare class for CAN Shield
MCP_CAN CAN(SPI_CS_PIN);

void setup() {
  // Initialize serial for DEBUG.
  Serial.begin(9600);
  while(!Serial)
  {
    delay(100);
  }

  //mySerial.begin(4800);
  //mySerial.println("SoftwareSerial Started");
  
  // Slow down the master a bit
  //SPI.setClockDivider(SPI_CLOCK_DIV8);
  // Master initialized.
  Serial.println("Master Initialized");
}


//variables for the can packet
unsigned char buf[8] = {0,0,0,0,0,0,0,0};
unsigned char len = 0;

// The loop() function runs continuously after setup().
void loop() {

  if(CAN_MSGAVAIL == CAN.checkReceive())            // check if data coming
  {
    Serial.println("message here");
    CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
    Serial.println(buf[0]);
    
    unsigned char canId = CAN.getCanId();
    //mySerial.write(buf[0]);
    Serial.println("Message Sent");
  }
}


