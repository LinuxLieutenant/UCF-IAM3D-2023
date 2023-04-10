/*
  This is the code for 2023 UCF ASME IAM3D competition Hovercraft Controller
  You will need a FLYSKY FS-i6 controller and FS-iA6B reciever as well as 2 micro servos, an arduino (any will work), 4 speed controllers and at least 4 brushless dc motors.
*/
#include <IBusBM.h>
#include <Servo.h>
 
//--------------------PWM PIN DESIGNATIONS-------------------//
//PWM Pins on the mega: (2-13, 44-46)
//Rudder
int rudderPin = 2;
//Claw
int clawPin = 3;
// Motor A pins (Inflation, 2 motors)
int inflatorPin = 4;
//Motor D pins (Thrust)
int thrustPin = 5;
// Motor B pins (Left)
int leftPin = 6;
// Motor C pins (Right)
int rightPin = 7;
//------------------------------------------------------------//

// Create Servo, Motor, Ibus Objects
IBusBM ibus;
Servo rudder;
Servo claw;
Servo inflator;
Servo thrust;
Servo left;
Servo right;
 
// Read the number of a given channel and convert to the range provided.
// If the channel is off, return the default value
int readChannel(byte channelInput, int minLimit, int maxLimit, int defaultValue) {
  uint16_t ch = ibus.readChannel(channelInput);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}
 
// Read the channel and return a boolean value
bool readSwitch(byte channelInput, bool defaultValue) {
  int intDefaultValue = (defaultValue) ? 100 : 0;
  int ch = readChannel(channelInput, 0, 100, intDefaultValue); //defalt max is 100, default min is 0
  return (ch > 50);
}
 
void setup() {
  // Start serial monitor
  Serial.begin(115200);
  // Attach iBus object to serial port
  ibus.begin(Serial);

  //Attach servos and Electronic Speed Controllers (ESCs)
  rudder.attach(rudderPin);                //Attach the rudder servo to pin 2 digital
  claw.attach(clawPin);                    //Attach the claw servo to pin 3 digital
  inflator.attach(inflatorPin,1000,2000);  //Attach the inflator motor to pin 4 digital
  thrust.attach(thrustPin,1000,2000);      //Attach the thrust motor to pin 5 digital
  left.attach(leftPin,1000,2000);          //Attach the left motor to pin 6 digital
  right.attach(rightPin,1000,2000);        //Attach the right motor to pin 7 digital

  //Set initial throttle input for the ESCs to 0
  inflator.write(0);
  thrust.write(0);
  left.write(0);
  right.write(0);

  //This delay is very important. the ESCs must have 0 throttle input for at least 1 second
  delay(1000); 
}
 
void loop() {

  // Cycle through first 5 channels and determine values
  // Print values to serial monitor
  // Note IBusBM library labels channels starting with "0"
 
  for (byte i = 0; i < 5; i++) {
    int value = readChannel(i, -100, 100, 0);
    Serial.print("Ch");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(value);
    Serial.print(" | ");
  }
 
  // Print channel 6 (switch) boolean value
  Serial.print("Ch6: ");
  Serial.print(readSwitch(5, false));
  Serial.println();
  /*
  To assign a channel in the code, take the channel number and subtract 1 (e.g. Channel 1 would be addressed as 0).
  Channel 1 is right Stick left/right
  Channel 2 is right stick up/down
  Channel 3 is left stick up/down
  Channel 4 is left stick left/right 
  channel 5 is left dial
  channel 6 is right dial
  */
  //-------THIS IS THE CODE TO EDIT TO ADD/MODIFY THE SERVO AND MOTOR VALUES--------------------------
  rudder.write(map(readChannel(0,10,1000,0),10,1000,10,180));   //0 is the right stick lef/right (channel 1) pin 2  
  claw.write(map(readChannel(5,10,1000,0),10,1000,10,180));     //5 is the top right dial (channel 6) pin 3
  inflator.write(map(readChannel(4,10,1000,0),10,1000,10,180)); //4 is the top left dial (channel 5) pin 4
  thrust.write(map(readChannel(1,10,1000,0),10,1000,10,180));   //1 is right stick up/down (channel 2) pin 5
  //left.write(readChannel(3,-180,180,0));    //3 is left stick left/right (channel 4) pin 6
  right.write(map(readChannel(3,10,1000,0),10,1000,10,180));     //3 is left stick left/right (channel 4) pin 7
  //This if statement makes it so the left motor and the right motor are controlled by the same axis
  if (readChannel(3, -1000, 1000, 0)<0) 
    left.write(map(readChannel(3,-1000,-10,0), -10, -1000, 10, 180));
  delay(0);
}