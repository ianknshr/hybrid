// Include SPI (Serial Peripheral Interface) library.
#include <SPI.h>

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
// The loop() function runs continuously after setup().
void loop() {

  sendSPI(80);
  Serial.println("Message Sent");
  delay(1000);
  
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

