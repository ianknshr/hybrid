//include can
#include <mcp_can.h>
#include <mcp_can_dfs.h>
// Include SPI (Serial Peripheral Interface) library. Does not support SPI Slave.
#include <SPI.h>
boolean SSlast = LOW;         // SS last flag.
const byte led = 9;           // Slave LED digital I/O pin.
boolean ledState = HIGH;      // LED state flag.
const byte cmdBtn = 1;        // SPI cmdBtn master command code.
const byte cmdLEDState = 2;   //

int x =0; //counter

const int SPI_CS_PIN = 9;

// Declare class for CAN Shield
MCP_CAN CAN(SPI_CS_PIN);

const byte ss = 8;  //set slave select pin to 8

//Initialize SPI slave.
void SlaveInit(void) {
  // Initialize SPI pins.
  pinMode(SCK, INPUT);
  pinMode(MOSI, INPUT);
  pinMode(MISO, INPUT);
  pinMode(ss, INPUT);
  // Enable SPI as slave.
  SPCR = (1 << SPE);
}
// SPI Transfer.
byte SPItransfer(byte value) {
  SPDR = value;
  while(!(SPSR & (1<<SPIF)));
  delay(10);
  return SPDR;
}
// The setup() function runs after reset.
void setup() {
  // Initialize serial for troubleshooting.
  Serial.begin(9600);
  digitalWrite(led, ledState);
  // Initialize SPI Slave.
  SlaveInit();
  Serial.println("Slave Initialized");

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
  Serial.println("Message Sent: ");
  delay(1000);

  
  // Slave Enabled?
  if (digitalRead(ss)==LOW) {
      // Yes, take MISO pin.
      pinMode(MISO, OUTPUT);
      Serial.println("***Slave Enabled.");
      // Write -1 slave response code and receive master command code
      byte rx = SPItransfer(255);
      Serial.println("rx:" + String(rx) + ".");
  }
  else {
    // No, first time?
    if (SSlast != HIGH) {
      // Yes, release MISO pin.
      pinMode(MISO, INPUT);
      Serial.println("Slave Disabled.");
      // Update SSlast.
      SSlast = HIGH;
    }
  }
}

