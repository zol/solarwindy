// Subclass this to begin using analog sensors. Be sure to call Init(),
// after constructing, then Read() to read and store the voltage reading on
// the pin. Finally, call value() to get the value of the last reading.
// Zoltan Olah (zol@me.com) released under the MIT license on Jan 13 2012.

#include "AnalogSensor.h"
#include "Arduino.h"

AnalogSensor::AnalogSensor(int pin) {
  pin_ = pin;
  value_ = -1;  // use -1 for no reading yet
}

void AnalogSensor::Init() {
  pinMode(pin_, INPUT);
}

// Read and store the voltage value on the pin.
int AnalogSensor::Read() {
  value_ = analogRead(pin_);
  return value_;
}
