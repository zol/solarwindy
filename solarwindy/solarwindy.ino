// Arduino Sketch for solarwindy, a an autonomous
// weather station (http://www.github.com/zol/solarwindy).
//
// At every kTransmitInterval, we read the sensors and upload them via http GET.
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

const char *kHost = "10.0.1.6"; // Host running skybob.
const int kPort = 3000; // Skybob's port.

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

  vane.Init();
  temp.Init();
  anemometer.Init();

  pinMode(kDebugLedPin, OUTPUT);

  Logger::Printf("Solarwindy initalized. Free Memory: %d bytes. IP Address:", freeMemory());
  Serial.println(Ethernet.localIP());
}

void transmit(EthernetClient *client, float temp, float wind_speed, const char *direction) {
  char body[256];
  char temp_buffer[16];
  char wind_speed_buffer[16];
  
  dtostrf(temp, 3, 2, temp_buffer);
  dtostrf(wind_speed, 3, 2, wind_speed_buffer);
  
  snprintf(body, 256, "GET /create_observation?windSpeed=%s&windDirection=%s&temp=%s HTTP/1.1\r\n\r\n",
    wind_speed_buffer, direction, temp_buffer);
  
  Logger::Printf("Transmitting...");
  if (client->connect(kHost, kPort)) {
    client->print(body);  
    Logger::Printf("done\n");
  } else {
    Logger::Debug("Failed to connect to host.");
  }
}

void loop() {
  unsigned long time = millis(); //  Current time since boot up
  unsigned long transmitInterval = time - lastTransmitTime;

  //  This block will run every kReadInterval
  if (transmitInterval >= (kTransmitInterval * 1000)) {
    lastTransmitTime = time;
    
    // flip the led each time we go aroudn the loop
    debug_led_state = debug_led_state == HIGH ? LOW : HIGH;
    digitalWrite(kDebugLedPin, debug_led_state);

    vane.Read();
    temp.Read();
    anemometer.Read(transmitInterval);
    anemometer.ResetTicks();

    Logger::PrintReading(temp.ComputeCelsius(), anemometer.ComputeKnots(),
      vane.ComputeDirection(), time);
    transmit(&client, temp.ComputeCelsius(), anemometer.ComputeKnots(),
      vane.ComputeDirection());
  }
  
  // chomp up bytes and ensure we disconnect to allow future requests to work.
  if (client.available()) { char c = client.read(); }
  if (!client.connected()) { client.stop(); }
}
