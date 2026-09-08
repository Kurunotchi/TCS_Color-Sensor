#include <Servo.h>

Servo myServo;

const int trigPin = 9;   // Ultrasonic trigger pin
const int echoPin = 10;  // Ultrasonic echo pin
long duration;
int distance;

void setup() {
  myServo.attach(6);     // Servo signal pin on D6
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  // Send ultrasonic pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read echo
  duration = pulseIn(echoPin, HIGH);

  // Convert to distance (cm)
  distance = duration * 0.034 / 2;

  // Map distance (e.g., 2–40 cm) to servo angle (0–180°)
  int angle = map(distance, 2, 40, 0, 180);
  angle = constrain(angle, 0, 180);

  myServo.write(angle);

  // Debug output
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm | Servo angle: ");
  Serial.println(angle);

  delay(100);
}