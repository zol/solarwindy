// Base class for sensors that read a value of an analog pin.
// Zoltan Olah (zol@me.com) released under the MIT license on Jan 13 2012.

#ifndef SOLARWINDY_ANALOGSENSOR_H
#define SOLARWINDY_ANALOGSENSOR_H

// Simple base class for reading and storing values from the Analog pins.
class AnalogSensor {
 public:
  explicit AnalogSensor(int pin);
  void Init();
  int Read();
  int value() const { return value_; }
    
 private:
   int pin_;
   int value_;
};

#endif
