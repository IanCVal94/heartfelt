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
const float BACKSTROKE_MULTIPLIER_A = 1;
const float BACKSTROKE_MULTIPLIER_B = 1;
const int STEPPER_A_STEPS = 500;
const int STEPPER_B_STEPS = 1000;
const int STEPPER_A_START = STEPPER_A_STEPS * (1-BACKSTROKE_MULTIPLIER_A);
const int STEPPER_B_START = STEPPER_B_STEPS * (1-BACKSTROKE_MULTIPLIER_B);

// Add speed control variable (at the top with other constants)
float speedMultiplier = 1.0;

// Add speed setter function
void setSpeed(float newSpeed) {
  speedMultiplier = newSpeed;
  stepperA.setMaxSpeed(8000 * speedMultiplier);
  stepperA.setAcceleration(8000 * speedMultiplier);
  stepperB.setMaxSpeed(10000 * speedMultiplier);
  stepperB.setAcceleration(10000 * speedMultiplier);
}

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
  stepperA.setAcceleration(8000);
  stepperB.setMaxSpeed(10000);
  stepperB.setAcceleration(10000);

  // Update speed settings to use multiplier
  setSpeed(0.5);  // Initialize with default speed

  Serial.begin(9600);
}

// Track which phase of the heartbeat we're in
int heartbeatPhase = 0;

// Move this from loop() to its own function
void moveMotors() {
  // Phase 0: Start filling A
  if (heartbeatPhase == 0) {
    stepperA.move(-STEPPER_A_STEPS);
    heartbeatPhase = 1;
  }
  // Phase 1: Wait for A to be completely full before starting next phase
  else if (heartbeatPhase == 1) {
    if (stepperA.distanceToGo() == 0) {
      stepperA.move(STEPPER_A_STEPS * BACKSTROKE_MULTIPLIER_A);
      stepperB.move(STEPPER_B_STEPS);
      heartbeatPhase = 2;
    }
  }
  // Phase 2: Wait for both A to empty AND B to fill completely
  else if (heartbeatPhase == 2) {
    if (stepperA.distanceToGo() == 0 && stepperB.distanceToGo() == 0) {
      stepperB.move(-STEPPER_B_STEPS * BACKSTROKE_MULTIPLIER_B);
      heartbeatPhase = 3;
    }
  }
  // Phase 3: Wait for B to completely empty before starting next cycle
  else if (heartbeatPhase == 3) {
    if (stepperB.distanceToGo() == 0) {
      heartbeatPhase = 0;
    }
  }

  // Run both motors
  stepperA.run();
  stepperB.run();
}

void loop() {
  moveMotors();
}