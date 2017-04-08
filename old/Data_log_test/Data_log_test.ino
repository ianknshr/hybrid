#include <SPI.h>
#include "SdFat.h"

SdFat sd;

SdFile file;

// Log file base name.  Must be six characters or less.
#define FILE_BASE_NAME "Data"

// Error messages stored in flash.
#define error(msg) sd.errorHalt(F(msg))

void setup() {
  const uint8_t BASE_NAME_SIZE = sizeof(FILE_BASE_NAME) - 1;
  char fileName[13] = FILE_BASE_NAME "00.csv";


  
  Serial.begin(9600);
  pinMode(10,OUTPUT);
  pinMode(9,OUTPUT);

  // Initialize the SD card at SPI_HALF_SPEED to avoid bus errors with
  // breadboards.  use SPI_FULL_SPEED for better performance.
  if (!sd.begin(9, SPI_HALF_SPEED)) {
    sd.initErrorHalt();
  }

   // Find an unused file name.
  if (BASE_NAME_SIZE > 6) {
    error("FILE_BASE_NAME too long");
  }
  while (sd.exists(fileName)) {
    if (fileName[BASE_NAME_SIZE + 1] != '9') {
      fileName[BASE_NAME_SIZE + 1]++;
    } else if (fileName[BASE_NAME_SIZE] != '9') {
      fileName[BASE_NAME_SIZE + 1] = '0';
      fileName[BASE_NAME_SIZE]++;
    } else {
      error("Can't create file name");
    }
  }
  if (!file.open(fileName, O_CREAT | O_WRITE | O_EXCL)) {
    error("file.open");
  }


  
  Serial.println("SD card initialized");
  Serial.print(F("Logging to: "));
  Serial.println(fileName);

  // Write data header.
  file.print(F("values"));

  file.close();

  Serial.println("file closed");
}

int x=0;

void loop() {

  file.open();
  file.println(x);
  file.close();
  Serial.println(x);
  x++;
  delay(500);

}
