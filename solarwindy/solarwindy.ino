// Arduino Sketch for solarwindy, a solar powered quad band autonomous
// weather station (http://www.github.com/zol/solarwindy).
//
// 1. We read the sensors every kReadInterval.
// 2. We keep a rolling store of kTransmitInterval's worth of data.
// 3. At every kTransmitInterval, we upload a summary of our stored data.
//
// Zoltan Olah (zol@me.com) released under the MIT license on Jan 13 2012.

#include <Ethernet.h>
#include <SPI.h>
#include "ArgentAnemometer.h"
#include "ArgentVane.h"
#include "TMP36TempSensor.h"
#include "Logger.h"
#include "MemoryFree.h"
#include "ObservationStore.h"

const char *kHost = "skybob.meteor.com"; // Host running skybob.
const int kPort = 80; // Skybob's port.

const int kTransmitInterval = 5; // Sec intervals we use between transmits
const int kDebugLedPin = 5; // We flash this led to signal program flow

const uint8_t kMac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

unsigned long lastTransmitTime = 0; // Stores the last time we transmitted

unsigned int debug_led_state = LOW;

ArgentAnemometer anemometer(2);
ArgentVane vane(A0);
TMP36TempSensor temp(A1);
EthernetClient client;

void setup() {
  Logger::Init();
  
  Logger::Printf("Obtaining dhcp lease...\n");
  Ethernet.begin(mac);
  
  delay(1000);
    Logger::Printf("Transmitting...");
    if (client.connect("skybob.meteor.com", 80)) {
      client.print("GET / HTTP/1.1\r\n\r\n");  
      Logger::Printf("done\n");
    } else {
      Logger::Debug("Failed to connect to host.");
    }

}

void transmit(EthernetClient *client, float temp, float wind_speed, const char *direction) {
}

void loop() {

 
  // chomp up bytes and ensure we disconnect to allow future requests to work.
  if (client.available()) { char c = client.read(); Serial.print(c);}
  if (!client.connected()) { client.stop(); }
}
