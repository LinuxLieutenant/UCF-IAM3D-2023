#include <IBusBM.h>
#include <Servo.h>
 
//--------------------PWM PIN DESIGNATIONS-------------------//
//PWM Pins on the mega: (2-13, 44-46)

//Rudder
int rudderPin = 2;
//Claw
int clawPin = 3;
// Motor A pins (Inflation, 2 motors)
int enA = 4;
int in1 = 5;
int in2 = 6;
// Motor B pins (Left)
int enB = 7;
int in3 = 8;
int in4 = 9;
// Motor C pins (Right)
int enC = 10;
int in5 = 11;
int in6 = 12;
//Motor D pins (Thrust)
int enD = 44;
int in7 = 45;
int in8 = 46;

//------------------------------------------------------------//


// Create iBus Object
IBusBM ibus;
Servo rudder;
Servo claw;
 
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
  int ch = readChannel(channelInput, 0, 100, intDefaultValue);
  return (ch > 50);
}
 
void setup() {
  // Start serial monitor
  Serial.begin(115200);

  rudder.attach(rudderPin);   //Attach the rudder servo to pin 2 digital
  claw.attach(clawPin);     //Attach the claw servo to pin 3 digital
 
  // Attach iBus object to serial port
  ibus.begin(Serial1);

  //----------For the motor pins-------------//
  pinMode(enA, OUTPUT);
	pinMode(enB, OUTPUT);
  pinMode(enC, OUTPUT);
	pinMode(enD, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	pinMode(in3, OUTPUT);
	pinMode(in4, OUTPUT);
	pinMode(in5, OUTPUT);
	pinMode(in6, OUTPUT);
	pinMode(in7, OUTPUT);
	pinMode(in8, OUTPUT);
  // Turn off motors - Initial state
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
  digitalWrite(in5, LOW);
  digitalWrite(in6, LOW);
  digitalWrite(in7, LOW);
  digitalWrite(in8, LOW);
  //----------------------------------------//
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
  

  //-------THIS IS THE CODE TO EDIT TO ADD/MODIFY THE SERVO AND MOTOR VALUES--------------------------
  rudder.write(readChannel(0,0,180,0));  //This is the left stick lef/right
  claw.write(readChannel(5,0,180,0));   //This is the top right dial (channel 6)
  inflation();
  speed();

  delay(10);
}

void inflation() {
	// Turn on motors
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);

	analogWrite(enA, readChannel(4,0,100,0)); //channel 5
	
	// Now turn off motors
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
}
void direction() {
	// Turn on motors
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);
  digitalWrite(in5, LOW);
	digitalWrite(in6, HIGH);

	analogWrite(enB, readChannel(3,-100,100,0)); //channel 4
	analogWrite(enC, readChannel(3,-100,100,0)); //channel 4
	
	// Now turn off motors
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
  digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
}
void speed() {
	// Turn on motors
	digitalWrite(in7, LOW);
	digitalWrite(in8, HIGH);

	analogWrite(enD, readChannel(1,-100,100,0)); //channel 2
	
	// Now turn off motors
	digitalWrite(in7, LOW);
	digitalWrite(in8, LOW);
}