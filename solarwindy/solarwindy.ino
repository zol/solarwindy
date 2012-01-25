// Arduino Sketch for solarwindy, a solar powered quad band autonomous
// weather station (http://www.github.com/zol/solarwindy).
//
// 1. We read the sensors every kReadInterval.
// 2. We keep a rolling store of kTransmitInterval's worth of data.
// 3. At every kTransmitInterval, we upload a summary of our stored data.
//
// Zoltan Olah (zol@me.com) released under the MIT license on Jan 13 2012.

#include "ArgentAnemometer.h"
#include "ArgentVane.h"
#include "TMP36TempSensor.h"
#include "ObservationStore.h"
#include "Logger.h"
#include "MemoryFree.h"

const int kTransmitInterval = 1; // Min intervals we use between transmits
const int kReadInterval = 5; // Sec intervals we read and log the sensors
const int kDebugLedPin = 13; // We flash this led to signal program flow

unsigned long lastReadTime = 0; // Used to calculate the next time to read
unsigned long lastTransmitTime = 0; // As above

unsigned int debug_led_state = LOW;

ArgentAnemometer anemometer(2);
ArgentVane vane(A0);
TMP36TempSensor temp(A1);
ObservationStore store(12); // 1 minute worth of observations

void setup() {
  Logger::Init();

  vane.Init();
  temp.Init();
  anemometer.Init();

  pinMode(kDebugLedPin, OUTPUT);

  if (store.Init() == false)
    Logger::Fatal("ObservationStore Init() failed. Check free memory.");
    
  Logger::Printf("Solarwindy initalized. Free Memory: %d bytes.\n", freeMemory());
}

void loop() {
  unsigned long time = millis(); //  Current time since boot up
  unsigned long readInterval = time - lastReadTime;
  unsigned long transmitInterval = time - lastTransmitTime;

  //  This block will run every kReadInterval
  if (readInterval >= (kReadInterval * 1000)) {
    lastReadTime = time;
    
    // flip the led each time we go aroudn the loop
    debug_led_state = debug_led_state == HIGH ? LOW : HIGH;
    digitalWrite(kDebugLedPin, debug_led_state);

    vane.Read();
    temp.Read();
    anemometer.Read(readInterval);
    anemometer.ResetTicks();

    ObservationStore::Observation observation;
    observation.wind_speed = anemometer.ComputeKnots();
    observation.wind_segment = vane.ComputeSegment();
    observation.time = time;

    store.Add(observation);
    
    Logger::PrintReading(temp.ComputeCelsius(), observation.wind_speed,
      vane.ComputeDirection(), time);
  }
  
  //  This block will run every kTransmitInterval
  if (transmitInterval >= (static_cast<unsigned long>(kTransmitInterval) * 1000 * 60)) {
    lastTransmitTime = time;

    ObservationStore::AggregateObservation ao = 
      store.ComputeAggregate();

    Logger::PrintAggregateReading(ao.average_wind_speed, 
      ao.wind_gust, 
      ao.wind_gust_time, 
      ArgentVane::SegmentToDirection(ao.wind_gust_segment),
      ArgentVane::SegmentToDirection(ao.most_frequent_wind_segment), 
      ao.elapsed_time,
      time);
    
    // TODO: upload the aggregate values
  }
}
