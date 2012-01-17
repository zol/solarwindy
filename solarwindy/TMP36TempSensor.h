// Library for interfacing with the TMP36 Temperature Sensor.
// Zoltan Olah (zol@me.com) released under the MIT license on Jan 13 2012.

#ifndef SOLARWINDY_TMP36TEMPSENSOR_H
#define SOLARWINDY_TMP36TEMPSENSOR_H

#include "AnalogSensor.h"

// Very simple class to read the TMP36 Temperature Sensor
class TMP36TempSensor : public AnalogSensor {
 public:
  explicit TMP36TempSensor(int pin) : AnalogSensor(pin) {}

  float ComputeCelsius();
};

#endif
