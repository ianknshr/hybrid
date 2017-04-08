// demo: CAN-BUS Shield, receive data with check mode
// send data coming to fast, such as less than 10ms, you can use this way
// loovee, 2014-6-13


#include <SPI.h>
#include "mcp_can.h"


// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 9;
unsigned char CAN_ID_1 = 0x70;
unsigned char CAN_ID_2 = 0x80;
MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin

void setup()
{
    Serial.begin(115200);

    START_INIT:

    if(CAN_OK == CAN.begin(CAN_1000KBPS))                   // init can bus : baudrate = 1000k <------ ALWAYS HAVE @ 1MBPS FOR OUR PURPOSES
    {
        Serial.println("CAN BUS Shield init ok!");
    }
    else
    {
        Serial.println("CAN BUS Shield init fail");
        Serial.println("Init CAN BUS Shield again");
        delay(100);
        goto START_INIT;
    }
}


void loop()
{
    unsigned char len = 0;
    unsigned char buf[8];

    if(CAN_MSGAVAIL == CAN.checkReceive())            // check if data coming
    {
        CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf

        unsigned char canId = CAN.getCanId();
        
        if(CAN_ID_1 == canId)
        {
            Serial.print("Yaw Rate 1:\t\t\t");
            signed int Yaw_Rate = (buf[1] * 256 + buf[0]) + 0x8000;
            Yaw_Rate *= 0.005;
            Serial.print(Yaw_Rate);
            Serial.print(" \"deg/s\"");
            Serial.print("\t\t");
            Serial.print("Acc Y-axis:\t");
            signed int Acc_Y = (buf[5] * 256 + buf[4]) + 0x8000;
            Acc_Y *= 0.0001274;
            Serial.print(Acc_Y);
            Serial.println(" \"g\"");
        }
        else if(CAN_ID_2 == canId)
        {
            Serial.print("Yaw Angular Acceleration:\t");
            signed int Yaw_Accel = (buf[1] * 256 + buf[0]) + 0x8000;
            Yaw_Accel *= 0.125;
            Serial.print(Yaw_Accel);
            Serial.print(" \"deg/s\" ");
            Serial.print("\t\t");
            Serial.print("Acc X-axis:\t");
            signed int Acc_X = (buf[5] * 256 + buf[4]) + 0x8000;
            Acc_X *= 0.0001274;
            Serial.print(Acc_X);
            Serial.println(" \"g\" ");   
        }
    }
    else{
      if(CAN_CTRLERROR == CAN.checkError())            // check for Error state on the shield
      {
          Serial.print("There is an error");
          Serial.println();
      }
    }
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
