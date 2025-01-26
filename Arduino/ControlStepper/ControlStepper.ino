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
  // Phase 0: Fill chamber A and confirm it's full
  if (heartbeatPhase == 0) {
    if (stepperA.currentPosition() == 0 && stepperB.currentPosition() == 0) {
      stepperA.moveTo(-500);  // Start filling A
      heartbeatPhase = 1;
    }
  }
  // Phase 1: Wait for A to be completely full before starting next phase
  else if (heartbeatPhase == 1) {
    if (stepperA.currentPosition() == -500) {
      stepperA.moveTo(0);     // Start emptying A
      stepperB.moveTo(1000);  // Start filling B
      heartbeatPhase = 2;
    }
  }
  // Phase 2: Wait for both A to empty AND B to fill completely
  else if (heartbeatPhase == 2) {
    if (stepperA.currentPosition() == 0 && stepperB.currentPosition() == 1000) {
      stepperB.moveTo(0);     // Start emptying B
      heartbeatPhase = 3;
    }
  }
  // Phase 3: Wait for B to completely empty before starting next cycle
  else if (heartbeatPhase == 3) {
    if (stepperB.currentPosition() == 0) {
      heartbeatPhase = 0;     // Reset to start of cycle
    }
  }

  // Run both motors
  stepperA.run();
  stepperB.run();
} 