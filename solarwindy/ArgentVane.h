// Library for interfacing with the Argent Data Systems Wind Vane.
// Zoltan Olah (zol@me.com) released under the MIT license on Jan 13 2012.

#ifndef SOLARWINDY_ARGENTVANE_H
#define SOLARWINDY_ARGENTVANE_H

#include "AnalogSensor.h"

// Reads a wind vane using an analog input and translates the resistance
// to wind direction.
class ArgentVane : public AnalogSensor {
 public:
  explicit ArgentVane(int pin) : AnalogSensor(pin) {}
  char const *ComputeDirection();
  float ComputeDegrees();
  unsigned char ComputeSegment() { return FindSegment(value()); }
  static const char *SegmentToDirection(unsigned char segment)
      { return kDirections[segment]; }

 private:
  static const int kNumSegments = 16;
  static const int kThresholds[];
  static const float kDegrees[];
  static const char *kDirections[];

  unsigned char FindSegment(int voltage);
};

#endif
