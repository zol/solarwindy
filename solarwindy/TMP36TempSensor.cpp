#include "Arduino.h"
#include "TMP36TempSensor.h"

TMP36TempSensor::TMP36TempSensor(int pin)
{
  _pin = pin;
    
  pinMode(_pin, INPUT);
}

int TMP36TempSensor::raw()
{
  return analogRead(_pin);
}

float TMP36TempSensor::celsius()
{
  /* multiply raw by 0.004882814 to get voltage,
     then convert from 10mV per degree with 500mV offset
     to degrees times 100 */
  return (float)(((raw() * 0.004882814) - 0.5) * 100);
}
