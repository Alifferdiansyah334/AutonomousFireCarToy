#include <ESP32Servo.h>  // Include the Servo library

// Define the pin connected to the servo signal wire
const int servoPin = 17;

// Define the pin connected to the flame sensor
const int flameSensorPin = 13;

// Create a Servo object
Servo myServo;

void setup() {
  // Attach the servo to the specified pin
  myServo.attach(servoPin);

  // Initialize the flame sensor pin as input
  pinMode(flameSensorPin, INPUT);

  // Start the serial communication for debugging
  Serial.begin(115200);
}

void loop() {
  // Read the flame sensor value
  int flameSensorValue = digitalRead(flameSensorPin);

  // Print the flame sensor value to the Serial Monitor
  Serial.print("Flame Sensor Value: ");
  Serial.println(flameSensorValue);

  if (flameSensorValue == LOW) {
    // Flame detected (assuming LOW means flame detected)
    Serial.println("Flame detected!");
    // Additional actions can be placed here when flame is detected
  } else {
    // No flame detected
    Serial.println("No flame detected.");
    // Additional actions can be placed here when no flame is detected
  }

  // Move the servo from 0 to 180 degrees
  for (int angle = 0; angle <= 180; angle++) {
    myServo.write(angle);  // Set the servo position
    delay(15);             // Wait for the servo to reach the position
  }

  // Move the servo from 180 to 0 degrees
  for (int angle = 180; angle >= 0; angle--) {
    myServo.write(angle);  // Set the servo position
    delay(15);             // Wait for the servo to reach the position
  }

  // Optionally, add a delay here to control the speed of the servo movement
  // If you want the servo to move continuously without delay, you can remove the delay here
}
