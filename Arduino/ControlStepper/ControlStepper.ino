#include <AccelStepper.h>

// Define stepper motor and pins
#define STEP_PIN_A 1
#define DIR_PIN_A 0
#define STEP_PIN_B 3
#define DIR_PIN_B 2

// Create two instances of AccelStepper
AccelStepper stepperA(AccelStepper::DRIVER, STEP_PIN_A, DIR_PIN_A);
AccelStepper stepperB(AccelStepper::DRIVER, STEP_PIN_B, DIR_PIN_B);

void setup() {
  // Configure Stepper A
  stepperA.setMaxSpeed(1000);    // Set maximum speed (steps per second)
  stepperA.setAcceleration(500); // Set acceleration (steps per second^2)
  stepperA.moveTo(200);          // Move 200 steps forward

  // Configure Stepper B
  stepperB.setMaxSpeed(1000);    // Set maximum speed (steps per second)
  stepperB.setAcceleration(500); // Set acceleration (steps per second^2)
  stepperB.moveTo(200);          // Move 200 steps forward
}

void loop() {
  // Check and update Stepper A
  if (stepperA.distanceToGo() == 0) {
    stepperA.moveTo(-stepperA.currentPosition()); // Reverse direction
  }
  
  // Check and update Stepper B
  if (stepperB.distanceToGo() == 0) {
    stepperB.moveTo(-stepperB.currentPosition()); // Reverse direction
  }
  
  // Move both motors one step at a time
  stepperA.run();
  stepperB.run();
}
