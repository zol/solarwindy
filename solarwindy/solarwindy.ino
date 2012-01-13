#include "ArgentAnemometer.h"
#include "ArgentVane.h"
#include "TMP36TempSensor.h"

#define PIN_ANEMOMETER  2
#define PIN_VANE        A0
#define PIN_TEMP        A1
#define PIN_LED         13

#define LOG_INTERVAL_MSECS 5000

ArgentAnemometer anemometer(PIN_ANEMOMETER);
ArgentVane vane(PIN_VANE);
TMP36TempSensor temp(PIN_TEMP);

unsigned long lastLogTime = 0;

void setup() {                
  Serial.begin(9600);
}

void loop() {
  unsigned long time = millis();
  unsigned long interval = time - lastLogTime;
	
  if (interval >= LOG_INTERVAL_MSECS) {
    Serial.print("Speed in knots: ");
    Serial.println(anemometer.knotsOverInterval(interval));
    
    Serial.print("Direction in raw: ");
    Serial.println(vane.raw());

    Serial.print("Temp in celsius: ");
    Serial.println(temp.celsius());

    anemometer.resetTicks();    
    lastLogTime = time;		
  }
}

