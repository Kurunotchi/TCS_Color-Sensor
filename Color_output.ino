/*
  TCS34725 Color Sensor - Match Against Known Samples
  -------------------------------------------------------
  Compares each new reading to reference readings you
  recorded (Green, Black, Yellow, Red) and prints/lights
  up an LED for whichever one it's closest to.

  Library: Adafruit_TCS34725 (Library Manager)
  Wiring: SDA -> A4, SCL -> A5, VCC -> 3.3V/5V, GND -> GND

  LED assumed COMMON-CATHODE (HIGH = on).
  If yours is common-anode, swap HIGH/LOW in setLED().
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
  {"Green",  321, 534, 447, 1326},
  {"Black",  208, 297, 286, 813},
  {"Yellow", 613, 597, 424, 1656},
  {"Red",    526, 495, 481, 1252}
};

// Automatically matches the actual array size — no more manual mismatch bugs
const int numReferences = sizeof(references) / sizeof(references[0]);

// If the closest match is farther than this distance, report "Unknown"
// instead of forcing a match. Raise/lower this to loosen/tighten matching.
const float MAX_MATCH_DISTANCE = 200.0;

int red = 3;
int blue = 4;
int green = 5;

void setup() {
  Serial.begin(9600);

  pinMode(red, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(green, OUTPUT);

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

  setLED(colorName);

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

void setLED(String colorName) {
  // Start with everything off
  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
  digitalWrite(blue, LOW);

  if (colorName == "Red") {
    digitalWrite(red, HIGH);
  } else if (colorName == "Green") {
    digitalWrite(green, HIGH);
  } else if (colorName == "Yellow") {
    digitalWrite(red, HIGH);
    digitalWrite(green, HIGH);
  } else if (colorName == "Black") {
    // all off — already handled above
  } else {
    // Unknown — LED stays off
  }
}
