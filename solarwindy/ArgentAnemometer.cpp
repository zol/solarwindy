// Accumulates ticks from the Anemometer. The anemometer closes a switch
// upon each rotation, we track this by pulling the voltage high on the pin
// and attaching an interrupt to count the falling states.
// Use similarly to AnalogSensor with regard to Init() and Read() except
// here Read() takes an interval (in msecs) argument, and both are stored.
// This is required so as we can compute the speed.
// Zoltan Olah (zol@me.com) released under the MIT license on Jan 13 2012.

#include "ArgentAnemometer.h"
#include "Arduino.h"

volatile int ArgentAnemometer::ticks_ = 0;

ArgentAnemometer::ArgentAnemometer(int pin) {
  pin_ = pin;
  value_ = interval_ = -1;  // no readings yet
}

void ArgentAnemometer::Init() {
  pinMode(pin_, INPUT);
  digitalWrite(pin_, HIGH);
  
  // TODO write some code to enforce this:
  // we deduct 2 from the pin to map it to the interrupt.
  // NOTE: pin mut be either 2 or 3
  attachInterrupt(pin_ - 2, TickInterrupt, FALLING);
}

void ArgentAnemometer::Read(unsigned long interval) {
  value_ = ticks_;
  interval_ = interval;
}

// Compute the wind speed in knots according to the tick 
// count over the given interval (floats are pretty fast on the arduino)
// According to the sensor, 1 rev/sec = 1.492 mph or 40.019 meters per minute 
// or 1.2965 international knots. 1.150779 = mph to kn multiplication factor
float ArgentAnemometer::ComputeKnots() {
  return (float)(value_ / (interval_ / 1000.0) * 1.492 * 1.150779);
}

void ArgentAnemometer::ResetTicks() {
  ticks_ = 0;  
}

void ArgentAnemometer::TickInterrupt() {
  ticks_++;
}