#include <mcp_can.h>
#include <mcp_can_dfs.h>
#include <SPI.h>
#include <Servo.h>
#include "ICE_Controller_dfs.h"


// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 9;

// Declare class for CAN Shield
MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin

// Declare Servos
Servo Throttle_Servo;
Servo Shifting_Servo;
Servo Clutch_Servo;

// current gear
unsigned char dSPACE_gear;
unsigned char local_gear;

// initialize to clutch position to disengaged
unsigned int clutch_pos;

// variables used for CAN messages
unsigned char len = 0;
unsigned char buf[8];

// variables for if servos were activated
bool active_Shift  = false;
bool active_Clutch = false;

// variable for keeping track of time
INT32U current_Time = millis();
INT32U prev_Shift_Time = current_Time;
INT32U prev_Clutch_Time = current_Time;

// Initialize
void setup()
{
    // asign servos to pins
    Throttle_Servo.attach(THROTTLE_PIN);
    Shifting_Servo.attach(SHIFTING_PIN);
    Clutch_Servo.attach(CLUTCH_PIN);
    
    // initial clutch pos
    clutch_pos = ENGAGECLUTCH_ANGLE;
    
    // Setup default positions for servos on startup
    Throttle_Servo.writeMicroseconds(THROTTLE_MIN_ANGLE);
    Shifting_Servo.writeMicroseconds(REST_SHIFT_ANGLE);
    Clutch_Servo.writeMicroseconds(clutch_pos);
    
    // Initialize the Shield
    Serial.begin(115200);
    
    // init can bus : baudrate = 1000k
    while(CAN.begin(CAN_1000KBPS) == CAN_FAILINIT)
    {
        delay(100);
    }
    
    // Setup CAN masks
    CAN.init_Mask(0, 0, 0x03F);
    CAN.init_Mask(1, 0, 0x03F);
    
    // Setup CAN Filters
    CAN.init_Filt(0, 0, THROTTLE_ID);
    CAN.init_Filt(1, 0, GEAR_ID);
    CAN.init_Filt(2, 0, CLUTCH_ID);
    CAN.init_Filt(3, 0, SHIFT_ID);
}

void update_shifting_state()
{
    // move shifting servo back
    if(active_Shift)
    {
        // grab current time
        current_Time = millis();
        
        if((current_Time-prev_Shift_Time) > SHIFT_INTERVAL)
        {
            active_Shift = false;
            Shifting_Servo.writeMicroseconds(REST_SHIFT_ANGLE);
        }
    }
}

void update_clutch_servo()
{
    if(active_Clutch)
    {
        // grab current time
        current_Time = millis();
        
        if((current_Time-prev_Clutch_Time) > CLUTCH_TIME_INCREMENT)
        {
            clutch_pos = clutch_pos - CLUTCH_ANGLE_INCREMENT;
            
            if (clutch_pos <= ENGAGECLUTCH_ANGLE) {
                clutch_pos = ENGAGECLUTCH_ANGLE;
                Clutch_Servo.writeMicroseconds(clutch_pos);
                active_Clutch = false;
            }
            else {
                Clutch_Servo.writeMicroseconds(clutch_pos);
                prev_Clutch_Time = millis();
            }
        }
    }
}

// Running State
void loop()
{
    
    while(CAN_MSGAVAIL == CAN.checkReceive())            // check if data coming
    {
        CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf

        unsigned long canId = CAN.getCanId();            // get CAN id to match to task
        
        switch(canId)
        {
            // Control for Throttle Servo //////////
            case THROTTLE_ID:
            {
                unsigned char pos;
                pos = buf[0];
                double percentage = double(pos)/255;
                int angle = THROTTLE_MIN_ANGLE-percentage*(THROTTLE_MIN_ANGLE-THROTTLE_MAX_ANGLE);
                Throttle_Servo.writeMicroseconds(angle);
                break;
            }
            ////////////////////////////////////////
            
            // opperate clutch /////////////////////
            case CLUTCH_ID:
            {
                unsigned char clutch_id = buf[0];
                
                // Move clutch servo
                switch(clutch_id)
                {
                    case ENGAGED:  // engage clutch
                    {
                        if(!active_Clutch){
                            
                            if(clutch_pos == ENGAGECLUTCH_ANGLE)
                                break;
                        
                            clutch_pos = HALFENGAGECLUTCH_ANGLE;
                            Clutch_Servo.writeMicroseconds(clutch_pos);
                        
                            active_Clutch = true;
                            prev_Clutch_Time = millis();
                        }
                        break;
                    }
                    case DISENGAGED:  // disengaged clutch clutch
                    {
                        clutch_pos = DISENGAGECLUTCH_ANGLE;
                        Clutch_Servo.writeMicroseconds(clutch_pos);
                        break;
                    }
                }
                
                break;
            }
            ////////////////////////////////////////
            
            // Set Gear ////////////////////////////
            case GEAR_ID:
            {
                dSPACE_gear = buf[0];
                break;
            }
            ////////////////////////////////////////
            
            ////////////////////////////////////////
            case SHIFT_ID:
            {
                if (!active_Shift) {
                    
                    unsigned char shift = buf[0];
                    
                    switch(shift)
                    {
                        case FULL_SHIFT_UP:
                        {
                            prev_Shift_Time = millis();
                            active_Shift = true;
                            Shifting_Servo.writeMicroseconds(FULL_SHIFT_UP_ANGLE);
                            break;
                        }
                        case HALF_SHIFT_UP:
                        {
                            prev_Shift_Time = millis();
                            active_Shift = true;
                            Shifting_Servo.writeMicroseconds(HALF_SHIFT_UP_ANGLE);
                            break;
                        }
                        case SHIFT_DOWN:
                        {
                            prev_Shift_Time = millis();
                            active_Shift = true;
                            Shifting_Servo.writeMicroseconds(SHIFT_DOWN_ANGLE);
                            break;
                        }
                        case SHIFT_ERROR:
                        case NO_SHIFT:
                        default:
                        {
                            break;
                        }
                    }
                }
                break;
            }
            ////////////////////////////////////////
                
            // default /////////////////////////////
            default:
            {
                break;
            }
            ////////////////////////////////////////
                
        }
        
        update_shifting_state();
        update_clutch_servo();
    }
    
    update_shifting_state();
    update_clutch_servo();
}
 
 
