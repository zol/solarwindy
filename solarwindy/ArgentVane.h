/*
  ArgentAnemometer.h - Library for interfacing with the Argent Data Systems Wind Vane.
  Created by Zoltan Olah on Jan 13 2012.
  Released into the public domain.
*/
#ifndef ArgentVane_h
#define ArgentVane_h

class ArgentVane
{
  public:
    ArgentVane(int pin);
    int raw();
    
  private:
    int _pin;
};

#endif
