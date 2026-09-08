/*
  TCS34725 Color Sensor + Color Indicator LEDs
  ---------------------------------------------
  Green  -> D2
  Black  -> D3
  Yellow -> D4
  Red    -> D5
*/

#include <Wire.h>
#include "Adafruit_TCS34725.h"

Adafruit_TCS34725 tcs =
  Adafruit_TCS34725(
    TCS34725_INTEGRATIONTIME_50MS,
    TCS34725_GAIN_4X
  );

// ------------------------------------------------
// COLOR REFERENCE DATA
// ------------------------------------------------

struct ColorSample {
  const char* name;
  uint16_t r, g, b, c;
};

ColorSample references[] = {
  {"Green",  321, 534, 447, 1326},
  {"Black",  208, 297, 286, 813},
  {"Yellow", 613, 597, 424, 1656},
  {"Red",    526, 495, 481, 1252}
};

// IMPORTANT: There are 4 colors
const int numReferences = 4;

const float MAX_MATCH_DISTANCE = 200.0;

// ------------------------------------------------
// LED PINS
// ------------------------------------------------

const int GREEN_LED  = 2;
const int BLACK_LED  = 3;
const int YELLOW_LED = 4;
const int RED_LED    = 5;

// ------------------------------------------------
// SETUP
// ------------------------------------------------

void setup() {

  Serial.begin(9600);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLACK_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  // Turn all LEDs OFF
  turnOffLEDs();

  if (tcs.begin()) {
    Serial.println("TCS34725 sensor found!");
  } 
  else {
    Serial.println("No TCS34725 found ... check your wiring");

    while (1);
  }
}

// ------------------------------------------------
// MAIN LOOP
// ------------------------------------------------

void loop() {

  uint16_t r, g, b, c;

  // Read sensor
  tcs.getRawData(&r, &g, &b, &c);

  // Match color
  String colorName = matchColor(r, g, b, c);

  // Print RAW values
  Serial.print("Raw -> R:");
  Serial.print(r);

  Serial.print(" G:");
  Serial.print(g);

  Serial.print(" B:");
  Serial.print(b);

  Serial.print(" C:");
  Serial.print(c);

  Serial.print(" | Color: ");
  Serial.println(colorName);

  // Turn ON corresponding LED
  showColorLED(colorName);

  delay(500);
}

// ------------------------------------------------
// COLOR MATCHING FUNCTION
// ------------------------------------------------

String matchColor(
  uint16_t r,
  uint16_t g,
  uint16_t b,
  uint16_t c
) {

  float bestDistance = -1;
  const char* bestMatch = "Unknown";

  for (int i = 0; i < numReferences; i++) {

    float dr = (float)r - references[i].r;
    float dg = (float)g - references[i].g;
    float db = (float)b - references[i].b;
    float dc = (float)c - references[i].c;

    // Euclidean distance
    float distance =
      sqrt(
        dr * dr +
        dg * dg +
        db * db +
        dc * dc
      );

    if (bestDistance < 0 || distance < bestDistance) {

      bestDistance = distance;
      bestMatch = references[i].name;
    }
  }

  // Too far from all reference colors
  if (bestDistance > MAX_MATCH_DISTANCE) {
    return "Unknown";
  }

  return String(bestMatch);
}

// ------------------------------------------------
// LED CONTROL
// ------------------------------------------------

void showColorLED(String color) {

  // First turn everything OFF
  turnOffLEDs();

  if (color == "Green") {

    digitalWrite(GREEN_LED, HIGH);

  }

  else if (color == "Black") {

    digitalWrite(BLACK_LED, HIGH);

  }

  else if (color == "Yellow") {

    digitalWrite(YELLOW_LED, HIGH);

  }

  else if (color == "Red") {

    digitalWrite(RED_LED, HIGH);

  }

  // Unknown = all LEDs remain OFF
}

// ------------------------------------------------
// TURN OFF ALL LEDs
// ------------------------------------------------

void turnOffLEDs() {

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BLACK_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);
}
