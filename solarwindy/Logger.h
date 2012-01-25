// Lump miscelaneous logging functions here.
// Zoltan Olah (zol@me.com) released under the MIT license on Jan 13 2012.

#ifndef SOLARWINDY_LOGGER_H
#define SOLARWINDY_LOGGER_H

class Logger {
 public:
   static void Init();
   static void Fatal(const char *message);
   static void Debug(const char *message);
   static void Printf(const char *format, ...);
   static void PrintReading(float temp, float wind_speed,
     const char *direction, unsigned long time);
   static void PrintAggregateReading(float average_wind_speed,
     float wind_gust,
     unsigned long wind_gust_time,
     const char *wind_gust_direction,
     const char *most_frequent_wind_direction,
     unsigned long elapsed_time,
     unsigned long time);

  private:
   static const int kBufferSize = 128; // Largest string we will print
};

#endif
