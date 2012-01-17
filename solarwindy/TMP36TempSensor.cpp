// Simply convert the stored voltage to a temperature reading.
// Zoltan Olah (zol@me.com) released under the MIT license on Jan 13 2012.

#include "TMP36TempSensor.h"

float TMP36TempSensor::ComputeCelsius() {
  // multiply raw by 0.004882814 to get voltage,
  // then convert from 10mV per degree with 500mV offset
  // to degrees times 100 
  return (float)(((value() * 0.004882814) - 0.5) * 100);
}