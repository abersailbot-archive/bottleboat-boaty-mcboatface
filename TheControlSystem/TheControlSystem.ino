#include <Wire.h>

int HMC6352Address = 0x42;

int slaveAddress = HMC6352Address >> 1;


void setup() {

  Serial.begin(9600);

  Wire.begin();
}


int getCompassHeading()
{
  Wire.beginTransmission(slaveAddress);

  Wire.write("A");

  Wire.endTransmission();

  delay(10);

  Wire.requestFrom(slaveAddress, 2);

  int compassValue = 0;
  
  if (Wire.available()) {
    compassValue = Wire.read() * 256;
    compassValue += Wire.read();
  }

  return compassValue / 10;
}
  

void loop() {
  
  int heading = getCompassHeading();

  Serial.println(heading);
  
  delay(500);
  
}

include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int midpoint = 60;

int pos = midpoint;   // variable to store the servo position
                      // should never pass +/- 30 from the midpoint

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  for (pos = 30; pos <= 90; pos += 1) { // goes from 30 degrees to 90 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 90; pos >= 30; pos -= 1) { // goes from 90 degrees to 30 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}
