/*
  ArgentAnemometer.h - Library for interfacing with the TMP36 Temperature Sensor.
  Created by Zoltan Olah on Jan 13 2012.
  Released into the public domain.
*/
#ifndef TMP36TempSensor_h
#define TMP36TempSensor_h

class TMP36TempSensor
{
  public:
    TMP36TempSensor(int pin);
    int raw();
    float celsius();
    
  private:
    int _pin;
};

#endif
