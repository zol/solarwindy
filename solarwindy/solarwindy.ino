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

const int kTransmitInterval = 10; // Min intervals we use between transmits
const int kReadInterval = 5; // Sec intervals we read and log the sensors

unsigned long lastReadTime = 0; // Used to calculate the next time to read
unsigned long lastTransmitTime = 0; // As above

ArgentAnemometer anemometer(2);
ArgentVane vane(A0);
TMP36TempSensor temp(A1);

ObservationStore store(120); // 10 minutes worth of observations

void setup() {
  vane.Init();
  temp.Init();
  anemometer.Init();
  
  Serial.begin(9600); // for debugging
}

void loop() {
  unsigned long time = millis(); //  Current time since boot up
  unsigned long readInterval = time - lastReadTime;
  unsigned long transmitInterval = time - lastTransmitTime;

  //  This block will run every kReadInterval
  if (readInterval >= (kReadInterval * 1000)) {
    lastReadTime = time;
    vane.Read();
    temp.Read();
    anemometer.Read(readInterval);
    anemometer.ResetTicks();

    ObservationStore::Observation observation;
    observation.wind_speed = anemometer.ComputeKnots();
    observation.wind_segment = vane.ComputeSegment();
    observation.temp = temp.ComputeCelsius();
    observation.time = time;
    
    store.Add(observation);
    
    // Serial.print("Speed in knots: ");
    // Serial.println(anemometer.ComputeKnots());
    // 
    // Serial.print("Direction: ");
    // Serial.println(vane.ComputeDirection());
    // 
    // Serial.print("Temp in celsius: ");
    // Serial.println(temp.ComputeCelsius());
  }
  
  //  This block will run every kTransmitInterval
  if (transmitInterval >= (kTransmitInterval * 1000 * 60)) {
    lastTransmitTime = time;
    
    ObservationStore::AggregateObservation aggregate = 
      store.ComputeAggregate();
      
    // TODO: log + upload the values
  }
}

