#include "Arduino.h"
#include "ArgentVane.h"

ArgentVane::ArgentVane(int pin)
{
  _pin = pin;
    
  pinMode(_pin, INPUT);
}

// Expected raw voltage is
// 1023*(Resistance/(10000+Resistance))
int ArgentVane::raw()
{
  return analogRead(_pin);
}
