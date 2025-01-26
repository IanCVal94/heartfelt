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

// Define step sizes for each stepper
// backstroke multiplier is because the backstroke is less than the forward stroke due to pressure assistance
const float BACKSTROKE_MULTIPLIER_A = 0.99;
const float BACKSTROKE_MULTIPLIER_B = 1;
const int STEPPER_A_STEPS = 500;
const int STEPPER_B_STEPS = 1000;
const int STEPPER_A_START = STEPPER_A_STEPS * (1-BACKSTROKE_MULTIPLIER_A);
const int STEPPER_B_START = STEPPER_B_STEPS * (1-BACKSTROKE_MULTIPLIER_B);

void setup() {
  // Configure microstepping for full-step mode
  pinMode(MS1_PIN, OUTPUT);
  pinMode(MS2_PIN, OUTPUT);
  pinMode(MS3_PIN, OUTPUT);
  digitalWrite(MS1_PIN, LOW);
  digitalWrite(MS2_PIN, LOW);
  digitalWrite(MS3_PIN, LOW);

  // Set max speed and acceleration
  stepperA.setMaxSpeed(8000);
  stepperA.setAcceleration(4000);
  stepperB.setMaxSpeed(12000);
  stepperB.setAcceleration(7000);

  Serial.begin(9600);
}

// Track which phase of the heartbeat we're in
int heartbeatPhase = 0;

void loop() {
  // Phase 0: Start filling A
  if (heartbeatPhase == 0) {
    stepperA.move(-STEPPER_A_STEPS);  // Move relative to current position
    heartbeatPhase = 1;
  }
  // Phase 1: Wait for A to be completely full before starting next phase
  else if (heartbeatPhase == 1) {
    if (stepperA.distanceToGo() == 0) {  // Check if movement is complete
      stepperA.move(STEPPER_A_STEPS * BACKSTROKE_MULTIPLIER_A);  // Return with multiplier
      stepperB.move(STEPPER_B_STEPS);  // Start filling B
      heartbeatPhase = 2;
    }
  }
  // Phase 2: Wait for both A to empty AND B to fill completely
  else if (heartbeatPhase == 2) {
    if (stepperA.distanceToGo() == 0 && stepperB.distanceToGo() == 0) {
      stepperB.move(-STEPPER_B_STEPS * BACKSTROKE_MULTIPLIER_B);  // Return with multiplier
      heartbeatPhase = 3;
    }
  }
  // Phase 3: Wait for B to completely empty before starting next cycle
  else if (heartbeatPhase == 3) {
    if (stepperB.distanceToGo() == 0) {
      heartbeatPhase = 0;  // Reset to start of cycle
    }
  }

  // Run both motors
  stepperA.run();
  stepperB.run();
}