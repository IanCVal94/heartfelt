#include <AccelStepper.h>

// Define stepper motor and pins
#define STEP_PIN_A 1
#define DIR_PIN_A 0
#define STEP_PIN_B 3
#define DIR_PIN_B 2
#define MS1_PIN 21
#define MS2_PIN 20
#define MS3_PIN 19

// Create two instances of AccelStepper
AccelStepper stepperA(AccelStepper::DRIVER, STEP_PIN_A, DIR_PIN_A);
AccelStepper stepperB(AccelStepper::DRIVER, STEP_PIN_B, DIR_PIN_B);

void setup() {
  // Configure microstepping for full-step mode
  pinMode(MS1_PIN, OUTPUT);
  pinMode(MS2_PIN, OUTPUT);
  pinMode(MS3_PIN, OUTPUT);
  digitalWrite(MS1_PIN, LOW);
  digitalWrite(MS2_PIN, LOW);
  digitalWrite(MS3_PIN, LOW);

  // Set max speed and acceleration for faster motion
  stepperA.setMaxSpeed(12000);
  stepperA.setAcceleration(12000);
  stepperB.setMaxSpeed(12000);
  stepperB.setAcceleration(12000);

  // Optional debugging
  Serial.begin(9600);x
}

void loop() {
  // Check if Motor B has reached its current target
  if (stepperB.distanceToGo() == 0) {
    // Toggle direction for Motor B (oscillates between 0 and 600)
    int nextTargetB = (stepperB.currentPosition() == 0) ? 1000 : 0;
    stepperB.moveTo(nextTargetB);

    // Set Motor A to be half and inverted
    int nextTargetA = -nextTargetB / 2;
    stepperA.moveTo(nextTargetA);

    // Optional: Debugging information
    Serial.print("Motor B Target: ");
    Serial.print(nextTargetB);
    Serial.print(", Motor A Target: ");
    Serial.println(nextTargetA);
  }

  // Run both motors
  stepperA.run();
  stepperB.run();
}