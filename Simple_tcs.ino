#include <Wire.h>
#include "Adafruit_TCS34725.h"

// Initialize with default integration time and gain
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

void setup() {
  Serial.begin(9600);

  if (tcs.begin()) {
    Serial.println("TCS34725 found!");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1); // Halt if sensor not found
  }
}

void loop() {
  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c);

  // Normalize values to 0–255 range
  uint32_t sum = c;
  float red   = (float)r / sum * 255.0;
  float green = (float)g / sum * 255.0;
  float blue  = (float)b / sum * 255.0;

  Serial.print("R: "); Serial.print((int)red);
  Serial.print(" G: "); Serial.print((int)green);
  Serial.print(" B: "); Serial.print((int)blue);
  Serial.println();

  delay(1000);
}