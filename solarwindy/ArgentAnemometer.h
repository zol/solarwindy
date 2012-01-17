// Library for interfacing with the Argent Data Systems Anemometer.
// Zoltan Olah (zol@me.com) released under the MIT license on Jan 13 2012.

#ifndef SOLARWINDY_ARGENTANEMOMETER_H
#define SOLARWINDY_ARGENTANEMOMETER_H

// Class that handles the collection of ticks from the anemometer sensor
// and provides us with calculated wind speed.
class ArgentAnemometer {
 public:
  ArgentAnemometer(int pin);
  void Init();
  void Read(unsigned long interval);
  void ResetTicks();
  float ComputeKnots();

 private:
  static void TickInterrupt();

  int pin_;
  unsigned long value_;  // saved number of ticks
  unsigned long interval_;  // msec interval in which we collected value_
  static volatile int ticks_;  // accumulator
};

#endif
