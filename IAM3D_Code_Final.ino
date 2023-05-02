/*
  This is the code for 2023 UCF ASME IAM3D competition Hovercraft Controller
  You will need a FLYSKY FS-i6 controller and FS-iA6B reciever as well as 2 micro servos, an arduino (any will work), 4 speed controllers and at least 4 brushless dc motors.
*/
#include <IBusBM.h>
#include <Servo.h>


//50% Code
 
//--------------------PWM PIN DESIGNATIONS-------------------//
//PWM Pins on the mega: (2-13, 44-46)
//Rudder
int rudderPin = 3;
//Claw
int clawPin = 4;
// Motor A pins (Inflation, 2 motors)
int inflatorPin1 = 6;
int inflatorPin2 = 9;
//int inflatorPin2 = 9;
//Motor D pins (Thrust)
int thrustPin = 10;
int thrust2Pin = 7;
//------------------------------------------------------------//

// Create Servo, Motor, Ibus Objects
IBusBM ibus;
Servo thrust2;
Servo rudder;
Servo claw;
Servo inflator1;
Servo inflator2;
Servo thrust;
 
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

  delay(1000);


  //Attach servos and Electronic Speed Controllers (ESCs)
  thrust2.attach(thrust2Pin,1000,2000);
  rudder.attach(rudderPin);                //Attach the rudder servo to pin 2 digital
  claw.attach(clawPin);                    //Attach the claw servo to pin 3 digital
  inflator1.attach(inflatorPin1,1000,2000); //Attach the inflator motor to pin 4 digital
  inflator2.attach(inflatorPin2,1000,2000);
  thrust.attach(thrustPin,1000,2000);      //Attach the thrust motor to pin 5 digital      //Attach the right motor to pin 7 digital

  //Set initial throttle input for the ESCs to 0
  inflator1.write(0);
  inflator2.write(0);
  thrust.write(90);
  thrust2.write(90);
  rudder.write(90);

  //This delay is very important. the ESCs must have 0 throttle input for at least 1 second
  delay(200);
//  thrust.write(90); 
  inflator1.write(180);
  inflator2.write(180);
  delay(200);
  inflator1.write(0);
  inflator2.write(0);

  delay(100);
  rudder.write(60);
  claw.write(120);
  delay(500);
  rudder.write(120);
  claw.write(60);
  delay(500);
  claw.write(120);
  rudder.write(90);
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
  rudder.write(map(readChannel(0,10,1000,0),10,1000,0,180));   //0 is the right stick lef/right (channel 1) pin 2
    
  claw.write(map(readChannel(5,10,1000,0),10,1000,0,180));     //5 is the top right dial (channel 6) pin 3
  
  inflator1.write(map(readChannel(4,10,1000,0),10,1000,0,180));//4 is the top left dial (channel 5) pin 4 and pin 8
  inflator2.write(map(readChannel(4,10,1000,0),10,1000,0,180));

  float power = map(readChannel(1,-1000,1000,0),-1000,1000,0,180);
  
  thrust.write(power);   //1 is right stick up/down (channel 2) pin 5
  thrust2.write(power);

  delay(0);
}
