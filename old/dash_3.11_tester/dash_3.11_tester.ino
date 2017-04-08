//include can
#include <mcp_can.h>
#include <mcp_can_dfs.h>

int x =0; //counter

const int SPI_CS_PIN = 9;

// Declare class for CAN Shield
MCP_CAN CAN(SPI_CS_PIN);


void setup() {
  // Initialize serial for troubleshooting.
  Serial.begin(9600);
  
  // init can bus : baudrate = 1000k
  while(CAN.begin(CAN_1000KBPS) == CAN_FAILINIT)
  {
      delay(100);
  }

  Serial.println("Begin");
  
}
// The loop function runs continuously after setup().
void loop() {
  x++;
  if(x==10000) x=0;
  // put your main code here, to run repeatedly:
  unsigned char buf[8] = {x,0,0,0,0,0,0,0};
  
  //send CAN signal
  CAN.sendMsgBuf(0x50,0, 8, buf);
  CAN.sendMsgBuf(0x50,0, 8, buf);
  //Serial.println("Message Sent: ");
  if(Serial.available()>=16)
  {
    for(int i=0; i<16; i++)
    {
    byte incoming = Serial.read();
    //Serial.println("Read");
    Serial.println(incoming);
    }
  }
  delay(1000);
  
  
  
}

