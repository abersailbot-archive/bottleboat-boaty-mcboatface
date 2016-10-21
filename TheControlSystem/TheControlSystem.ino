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

