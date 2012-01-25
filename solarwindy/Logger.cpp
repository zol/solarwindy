//Zoltan Olah (zol@me.com) released under the MIT license on Jan 13 2012.

#include "Logger.h"
#include <stdio.h>
#include <stdarg.h>
#include "Arduino.h"

void Logger::Init() {
  Serial.begin(9600);
}

// Log a quick debug message
void Logger::Debug(const char *message) {
  Serial.print("Debug:");
  Serial.println(message);
}

// Will repeat the message and prevent further execution
void Logger::Fatal(const char *message) {
  while (true) {
    delay(3000);
    Serial.print("Fatal error, system halted:");
    Serial.println(message);
  }
}

// Handy printf style logging routine. Note anything larger than kBufferSize
// will be concatenated.
void Logger::Printf(const char *format, ...) {
  va_list arguments;
  char buffer[kBufferSize];
  
  va_start(arguments, format);
  vsnprintf(buffer, kBufferSize, format, arguments);
  
  Serial.print(buffer);
}

// vsnprintf doesn't work with the standard arduino linker settings so we
// must piece this together ourselves, grrrr.
void Logger::PrintReading(float temp, float wind_speed, 
    const char *direction, unsigned long time) {
      
  char temp_buffer[16];
  char wind_speed_buffer[16];
  
  dtostrf(temp, 3, 2, temp_buffer);
  dtostrf(wind_speed, 3, 2, wind_speed_buffer);
  
  Printf("-- Temp %sc, Wind %skn %s at %u seconds since boot.\r\n",
    temp_buffer, wind_speed_buffer, direction, (time / 1000));
}

void Logger::PrintAggregateReading(float average_wind_speed,
    float wind_gust,
    unsigned long wind_gust_time,
    const char *wind_gust_direction,
    const char *most_frequent_wind_direction,
    unsigned long elapsed_time,
    unsigned long time) {
    
  // char wind_gust_buffer[16];
  // dtostrf(wind_gust, 3, 2, wind_gust_buffer);
  //   
  // Serial.println(most_frequent_wind_direction);
  // Printf("== Gust %skn %s at %u seconds since boot. Avg direction \
  //   %s, readings over %u secs, time since boot is %u.\r\n",
  //   wind_gust_buffer,
  //   wind_gust_direction,    
  //   (wind_gust_time / 1000),
  //   most_frequent_wind_direction,
  //   (elapsed_time / 1000),
  //   (time / 1000));
  
  Serial.print("== Avg Speed ");
  Serial.print(average_wind_speed);
  Serial.print("kn Gust ");
  Serial.print(wind_gust);
  Serial.print(" at ");
  Serial.print(wind_gust_time);
  Serial.print(" seconds since boot. Gust direction ");
  Serial.print(wind_gust_direction);
  Serial.print(" Avg Wind Direction ");
  Serial.print(most_frequent_wind_direction);
  Serial.print(" Readings over elapsed time:");
  Serial.print(elapsed_time / 1000);
  Serial.print(" secs, time since boot is ");
  Serial.println(time / 1000);
}
