#include "Arduino.h"
#include "ArgentAnemometer.h"

volatile int ArgentAnemometer::ticks = 0;

ArgentAnemometer::ArgentAnemometer(int pin)
{
  _pin = pin;
    
  pinMode(_pin, INPUT);
  digitalWrite(_pin, HIGH);
  // we deduct 2 from the pin to map it to the interrupt.
  // NOTE: pin mut be either 2 or 3
  attachInterrupt(_pin - 2, ArgentAnemometer::tickInterrupt, FALLING);
}

// calculate the wind speed in knots according to the tick 
// count over the given interval (floats are pretty fast on the arduino)
// According to the sensor,
// 1 rev/sec = 1.492 mph or 40.019 meters per minute 
// or 1.2965 international knots
// 1.150779 = mph to kn multiplication factor
float ArgentAnemometer::knotsOverInterval(unsigned long intervalMsecs)
{
  return (float)(ArgentAnemometer::ticks / (intervalMsecs / 1000.0) * 1.492 * 1.150779);
}

void ArgentAnemometer::resetTicks()
{
  ArgentAnemometer::ticks = 0;  
}

void ArgentAnemometer::tickInterrupt()
{
  ArgentAnemometer::ticks++;
  
  /*digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);*/
}
