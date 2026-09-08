/*
  TCS34725 Color Sensor - Match Against Known Samples
  -------------------------------------------------------
  Compares each new reading to 3 reference readings you
  recorded (Green, Black, Yellow) and prints whichever
  one it's closest to.

  Library: Adafruit_TCS34725 (Library Manager)
  Wiring: SDA -> A4, SCL -> A5, VCC -> 3.3V/5V, GND -> GND
*/

#include <Wire.h>
#include "Adafruit_TCS34725.h"

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

// ---- Reference samples (from your actual readings) ----
// Format: {R, G, B, C}
struct ColorSample {
  const char* name;
  uint16_t r, g, b, c;
};

//Dito natin ilalagay ang RAW data given by our sensor

ColorSample references[] = {
  {"Green",  321, 534, 447, 1326}
  //Add data here of every you test
};

const int numReferences = 4;

// If the closest match is farther than this distance, report "Unknown"
// instead of forcing a match. Raise/lower this to loosen/tighten matching.
const float MAX_MATCH_DISTANCE = 200.0;

void setup() {
  Serial.begin(9600);

  if (tcs.begin()) {
    Serial.println("TCS34725 sensor found!");
  } else {
    Serial.println("No TCS34725 found ... check your wiring");
    while (1);
  }
}

void loop() {
  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c);

  String colorName = matchColor(r, g, b, c);

  Serial.print("Raw -> R:"); Serial.print(r);
  Serial.print(" G:"); Serial.print(g);
  Serial.print(" B:"); Serial.print(b);
  Serial.print(" C:"); Serial.print(c);

  Serial.print("  |  Color: ");
  Serial.println(colorName);

  delay(500);
}

String matchColor(uint16_t r, uint16_t g, uint16_t b, uint16_t c) {
  float bestDistance = -1;
  const char* bestMatch = "Unknown";

  for (int i = 0; i < numReferences; i++) {
    float dr = (float)r - references[i].r;
    float dg = (float)g - references[i].g;
    float db = (float)b - references[i].b;
    float dc = (float)c - references[i].c;

    // Euclidean distance across all 4 channels
    float distance = sqrt(dr * dr + dg * dg + db * db + dc * dc);

    if (bestDistance < 0 || distance < bestDistance) {
      bestDistance = distance;
      bestMatch = references[i].name;
    }
  }

  if (bestDistance > MAX_MATCH_DISTANCE) {
    return "Unknown";
  }

  return String(bestMatch);
}
