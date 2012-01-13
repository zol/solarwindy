/*
  ArgentAnemometer.h - Library for interfacing with the Argent Data Systems Anemometer.
  Created by Zoltan Olah on Jan 13 2012.
  Released into the public domain.
*/
#ifndef ArgentAnemometer_h
#define ArgentAnemometer_h

class ArgentAnemometer
{
  public:
    ArgentAnemometer(int pin);
    float knotsOverInterval(unsigned long intervalMsecs);
    void resetTicks();
    
    static void tickInterrupt();
    
  private:
    int _pin;
    
    
    static volatile int ticks;
};

#endif
