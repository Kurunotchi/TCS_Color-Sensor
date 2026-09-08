#include <Servo.h>

Servo myServo;   // create servo object
int potPin = A0; // potentiometer connected to analog pin A0
int val;         // variable to store potentiometer value

void setup() {
  myServo.attach(9); // attach servo signal pin to digital pin 9
}

void loop() {
  val = analogRead(potPin);          // read potentiometer (0–1023)
  val = map(val, 0, 1023, 0, 180);   // scale to servo range (0–180°)
  myServo.write(val);                // set servo position
  delay(15);                         // small delay for smooth movement
}