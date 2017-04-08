//include can
#include <mcp_can.h>
#include <mcp_can_dfs.h>
// Include SPI (Serial Peripheral Interface) library.
#include <SPI.h>

//SS pin for CAN communication
const int SPI_CS_PIN = 10;

//SS pin for uSD card
const int SPI_SD_PIN = 9;

// Declare class for CAN Shield
MCP_CAN CAN(SPI_CS_PIN);

const byte ss = 8; //set slave select pin to 8 becuase it should be unused

void setup() {
  // Initialize serial for DEBUG.
  Serial.begin(9600);

  //initialize ss pin
  pinMode(ss, OUTPUT);

  // Initialize SPI.
  // SCK, MOSI, SS pins into output mode
  // also put SCK, MOSI into LOW state, and SS into HIGH state.
  // Then put SPI hardware into Master mode and turn SPI on
  SPI.begin();
  // Slow down the master a bit
  //SPI.setClockDivider(SPI_CLOCK_DIV8);
  // Master initialized.
  Serial.println("Master Initialized");

}

void loop() {
  
  if(CAN_MSGAVAIL == CAN.checkReceive())            // check if data coming
  {
    Serial.println("message here");
    CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
    Serial.println(buf[0]); //print first value of message

    //add data logging for uSD

    
    unsigned char canId = CAN.getCanId();
    
    sendSPI(80);  //send arbitrary data value through SPI
    Serial.println("Message Sent");
    delay(1000);
  }

}

void sendSPI(int value)
{
  //set ss to LOW before before data is sent
  digitalWrite(ss, LOW);
  //send value
  SPI.transfer(value);
  //set ss to HIGH afterwards
  digitalWrite(ss,HIGH);
}
