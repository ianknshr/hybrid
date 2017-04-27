//
//  ICE_Controller_dfs.h
//
//  Created by Andrew Stenberg
//

#ifndef ICE_Controller_dfs_h
#define ICE_Controller_dfs_h

// Servo pins
#define THROTTLE_PIN 8
#define SHIFTING_PIN 12
#define CLUTCH_PIN   4

// Declare the CAN id's for all the messages
#define THROTTLE_ID 0x14
#define RPM_ID      0x1E
#define GEAR_ID     0x28
#define CLUTCH_ID   0x50
#define SHIFT_ID    0x32

// clutch ID's
#define DISENGAGED     1
#define ENGAGED        0

// shift ID's
#define HALF_SHIFT_UP     4
#define FULL_SHIFT_UP     1
#define SHIFT_DOWN        2
#define SHIFT_ERROR       3
#define NO_SHIFT          0

// throttle angles
#define THROTTLE_MIN_ANGLE 2000
#define THROTTLE_MAX_ANGLE 1440

// clutch angles
#define ENGAGECLUTCH_ANGLE     1200
#define DISENGAGECLUTCH_ANGLE  1920
#define HALFENGAGECLUTCH_ANGLE 1800

// shifting angles
#define REST_SHIFT_ANGLE    1463
#define SHIFT_DOWN_ANGLE    1700
#define HALF_SHIFT_UP_ANGLE 1300
#define FULL_SHIFT_UP_ANGLE 1200

// time intervals for servo rotation
#define SHIFT_INTERVAL         250
#define CLUTCH_TIME_INCREMENT  20
#define CLUTCH_ANGLE_INCREMENT 30

#endif /* ICE_Controller_dfs_h */
