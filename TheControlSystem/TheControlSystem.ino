#include <Wire.h>
#include <Servo.h>

int HMC6352Address = 0x42;

int slaveAddress = HMC6352Address >> 1;
int compassOffset = 120;
const int MIDPOINT = 60;
const int RANGE = 30;
const int DESIREDHEADING = 50;
const int SPEED = 128;

Servo rudder;


void setup() {

  Serial.begin(9600);
  rudder.attach(9);
  Wire.begin();
  analogWrite(5, SPEED);
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

  return (compassValue / 10) - compassOffset ;
}


void setRudder(int rudderPos) {
  // constraints mimimum value
  if (rudderPos < MIDPOINT - RANGE)
  {
    rudderPos = MIDPOINT - RANGE;

  }
  // constraints maximum value
  if (rudderPos > MIDPOINT + RANGE)
  {
    rudderPos = MIDPOINT + RANGE;

  }
  rudder.write(rudderPos);

}

int getHeadingDiff (int firstHeading, int desiredHeading) { //function to find heading difference when compass heading switches between 0 and 360
  if (firstHeading > 180) {
    return (360 - firstHeading) + desiredHeading;
  }else{
    return desiredHeading - firstHeading;
  }
}

int control(int compassHeading, int desiredHeading){
  static double integral = 0;
  static double pGain = 0.8;
  static double iGain = 0.2; 
  double error = getHeadingDiff(compassHeading, desiredHeading);
  // prevents integral windup 
  if (abs(error) < 10){
    integral = integral + error;
  }
  else {
    integral = 0;
  }
  double p = error * pGain; 
  double i = integral * iGain;

  return p + i;  
}

int wrapHeading(int compassHeading) {
    if (compassHeading < 0) {
      return 360 - compassHeading;
    } else {
      return compassHeading;
    }
}



void loop() {

  int heading = wrapHeading(getCompassHeading());
  int rudderpos = control(heading, DESIREDHEADING);

  setRudder(MIDPOINT + rudderpos);
  
  Serial.print("Compass heading ");
  Serial.println(heading);
  Serial.print("Rudder position ");
  Serial.println(rudderpos);

  delay(500);

}

