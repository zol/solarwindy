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

const int kTransmitInterval = 10; // Min intervals we use between transmits
const int kReadInterval = 5; // Sec intervals we read and log the sensors
unsigned long lastReadTime = 0; // Used to calculate the next time to read

ArgentAnemometer anemometer(2);
ArgentVane vane(A0);
TMP36TempSensor temp(A1);

void setup() {
  vane.Init();
  temp.Init();
  anemometer.Init();
  
  Serial.begin(9600); // for debugging
}

void loop() {
  unsigned long time = millis(); //  Current time since boot up
  unsigned long interval = time - lastReadTime; // Interval since last read

  //  This block will run every kReadInterval
  if (interval >= (kReadInterval * 1000)) {
    lastReadTime = time;
    vane.Read();
    temp.Read();
    anemometer.Read(interval);
    anemometer.ResetTicks();
    
    Serial.print("Speed in knots: ");
    Serial.println(anemometer.ComputeKnots());
    
    Serial.print("Direction: ");
    Serial.println(vane.ComputeDirection());

    Serial.print("Temp in celsius: ");
    Serial.println(temp.ComputeCelsius());
  }
}

