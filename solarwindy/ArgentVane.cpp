/* The Argent Wind Vane contains 16 resistors, each one being engaged when the
vane is pointing in a certain direction. We use a voltage divider and an 
analog pin to measure the voltage, which translates back to the reistor that
is connected to the circuit. We map the voltage back to a resistance and 
hence work out direction the vane is pointing.

Tables used map voltage readings to directions:

Segment | 0     1     2    3     4    5     6    7     8    9     10    11    12    13    14    15
Ohms    | 688   891   1000 1410  2200 3140  3900 6570  8200 14120 16000 21880 33000 42120 64900 120000
Deg     | 112.5 67.5  90   157.5 135  202.5 180  22.5  45   247.5 225   337.5 0     292.5 315   270
Dir     | ESE   ENE   E    SSE   SE   SSW   S    NNE   NE   WSW   SW    NNW   N     WNW   NW    W
Volts   | 66    84    93   126   184  244   287  406   461  599   630   702  785    827   886   944
Thres   | 0     74    88   109   155  214   265  346   433  529   614   665  743    805   856   915

Ohms  - Resistance as stated in the spec.
Deg   - Direction in degrees corresponding to resistance.
Dir   - Direction in words.
Volts - Value we should be detecting on the input pin. This is calculated from our voltage divider
        with a resistor of 10k ohms. The formula is: 1023*(Ohms/(10000+Ohms)) . Note a reading of
        1 on the input pin corresponds to 0.004882814 Volts in real life.
Thres - INT(Volts[x+1]-((Volts[x+1]-Volts[x])/2)), this is what we will use to decide on our which
        segment our reading falls into, decrementing the index pointer if our reading >= threshold
        then we have found our segment.

Zoltan Olah (zol@me.com) released under the MIT license on Jan 13 2012.
*/

#include "ArgentVane.h"

const int ArgentVane::kThresholds[kNumSegments] =
    {0, 74, 88, 109, 155, 214, 265, 346, 433, 529, 614, 665, 743, 805, 856,
    915};
    
const float ArgentVane::kDegrees[kNumSegments] =
    {112.5, 67.5, 90.0, 157.5, 135.0, 202.5, 180.0, 22.5, 45.0, 247.5, 225.0,
    337.5, 0.0, 292.5, 315.0, 270.0};
    
const char *ArgentVane::kDirections[kNumSegments] =
    {"ESE", "ENE", "E", "SSE", "SE", "SSW", "S", "NNE", "NE", "WSW", "SW",
    "NNW", "N", "WNW", "NW", "W"};

// Find which segment (as an array index) the voltage belongs to
unsigned char ArgentVane::FindSegment(int voltage) {
  for (unsigned int i = kNumSegments - 1; i >= 0; i--) {
    if (voltage >= kThresholds[i])
      return static_cast<unsigned char>(i);
  }
  
  return -1;  // couldn't find it
}

// Map the segment to a direction string
char const *ArgentVane::ComputeDirection() {
  return kDirections[FindSegment(value())];
}

// Map the segment to the direction in degrees
float ArgentVane::ComputeDegrees() {
  return kDegrees[FindSegment(value())];  
}