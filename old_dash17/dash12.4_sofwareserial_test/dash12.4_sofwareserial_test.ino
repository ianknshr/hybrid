#include <SoftwareSerial.h>


const int RX_PIN = 3;
const int TX_PIN = 4;

SoftwareSerial mySerial(RX_PIN,TX_PIN);

void setup() {
  Serial.begin(57600);
  while(!Serial){
    delay(100);
  }

  mySerial.begin(4800);
  mySerial.println("Serial started");
}

void loop() {
  mySerial.write(80);
  delay(1000);
}
