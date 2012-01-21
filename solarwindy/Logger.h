// Lump miscelaneous logging functions here.
// Zoltan Olah (zol@me.com) released under the MIT license on Jan 13 2012.

#ifndef SOLARWINDY_LOGGER_H
#define SOLARWINDY_LOGGER_H

#include "ObservationStore.h"

class Logger {
 public:
   static void Init();
   static void Fatal(const char *message);
   static void Debug(const char *message);
   static void Printf(const char *format, ...);
   static void PrintReading(float temp, float wind_speed, 
     const char *direction, unsigned long time);
   
  private:
   static const int kBufferSize = 128; // Largest string we will print
};

#endif
