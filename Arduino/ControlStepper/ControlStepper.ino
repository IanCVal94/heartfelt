#include <AccelStepper.h>

// Define stepper motor and pins
#define STEP_PIN 3
#define DIR_PIN 4

AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

void setup() {
  stepper.setMaxSpeed(1000);  // Set maximum speed (steps per second)
  stepper.setAcceleration(500); // Set acceleration (steps per second^2)
  stepper.moveTo(200);        // Move 200 steps forward
}

void loop() {
  if (stepper.distanceToGo() == 0) {
    stepper.moveTo(-stepper.currentPosition()); // Reverse direction
  }
  stepper.run(); // Moves the motor one step at a time
}