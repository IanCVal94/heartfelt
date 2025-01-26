#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <AccelStepper.h>

const char* ssid = "Xavier";      // Your WiFi network name
const char* password = "tacocat4642";      // Your WiFi password

// Define stepper motor and pins
#define STEP_PIN_A 1
#define DIR_PIN_A 0
#define STEP_PIN_B 3
#define DIR_PIN_B 2

// Track which phase of the heartbeat we're in
int heartbeatPhase =  0;

// Create two instances of AccelStepper
AccelStepper stepperA(AccelStepper::DRIVER, STEP_PIN_A, DIR_PIN_A);
AccelStepper stepperB(AccelStepper::DRIVER, STEP_PIN_B, DIR_PIN_B);


int loops = 0;
WiFiServer server(80);
WiFiClient client;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Global variables for heart animation
float currentScale = 1.0;
bool scaleIncreasing = false;
unsigned long lastAnimationUpdate = 0;
const unsigned long ANIMATION_INTERVAL = 100;  // 25ms between updates

// Modify these variables after the existing global variables
#define DEFAULT_BPM 60
#define PULSE_STEPS 50  // Number of steps for each pulse (adjust based on testing)
unsigned long lastPulseTime = 0;
bool pulsing = false;
int currentPosition = 0;

// Add this with the other global variables
int currentBPM = DEFAULT_BPM;  // Global BPM variable

// Add these global variables
String currentPacket = "";
bool newPacketAvailable = false;

void setup() {
  Serial.begin(115200);
  Wire.begin(9, 8);  // Set SDA to GPIO9, SCL to GPIO8

  // Initialize display first
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  } else {
    Serial.println(F("SSD1306 initialization successful"));
  }
  
  // Add these lines after display initialization
  delay(2000);
  // Clear display and draw heart once (it won't change)
  display.clearDisplay();
  drawHeart(1.0);  // Draw at full scale
  displayBPM(currentBPM);  // Add initial BPM display
  display.display();
  
  // Set text properties for any debug text you might want to show
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  // Set max speed and acceleration
  stepperA.setMaxSpeed(8000);
  stepperA.setAcceleration(4000);
  stepperB.setMaxSpeed(12000);
  stepperB.setAcceleration(7000);

  initWiFi();
}

void initWiFi() {
  // Disconnect any existing WiFi connection first
  WiFi.disconnect(true);  // true = disable WiFi at the same time
  delay(1000);  // Give it some time to disconnect fully

  // Clear any previous WiFi config
  WiFi.mode(WIFI_STA);
  WiFi.persistent(false);  // Don't save WiFi settings in flash

  // Now attempt to connect
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());

  server.begin();
}


void loop() {
  client = server.available();

  if (client) {
    Serial.println("Client connected!");
    
    while (client.connected()) {
      // Check for new data without blocking
      if (client.available()) {
        currentPacket = client.readStringUntil('\n');
        newPacketAvailable = true;
      }
      
      moveMotors();

      // Process packet after motors have moved
      if (newPacketAvailable) {
        Serial.println("Received: " + currentPacket);
        currentBPM = currentPacket.toInt();
        newPacketAvailable = false;
      }
    }
    Serial.println("Client disconnected");
    client.stop();
  }
}

void drawHeart(float scale) {
    int leftX = 95;
    int rightX = 105;
    int centerY = 25;
    int baseRadius = 8;
    int radius = baseRadius * scale;
    
    int triangleTop = centerY + (radius * 0.5);
    int triangleHeight = 24 * scale;
    
    display.fillCircle(leftX, centerY, radius, SSD1306_WHITE);
    display.fillCircle(rightX, centerY, radius, SSD1306_WHITE);
    display.fillTriangle(
        leftX - radius, triangleTop,
        rightX + radius, triangleTop,
        (leftX + rightX) / 2, centerY + triangleHeight,
        SSD1306_WHITE
    );
}

// Heart Animations - Disabled for now for performance reasons
// void updateHeart() {
//     unsigned long currentTime = millis();
    
//     if (currentTime - lastAnimationUpdate >= ANIMATION_INTERVAL) {
//         // Update heart animation scale
//         if (scaleIncreasing) {
//             currentScale += 0.04;
//             if (currentScale >= 1.0) {
//                 currentScale = 1.0;
//                 scaleIncreasing = false;
//             }
//         } else {
//             currentScale -= 0.04;
//             if (currentScale <= 0.6) {
//                 currentScale = 0.6;
//                 scaleIncreasing = true;
//             }
//         }  
        
//         // Update display
//         display.clearDisplay();
//         drawHeart(currentScale);
//         display.display();
        
//         lastAnimationUpdate = currentTime;
//     }
// }

void moveMotors() {
  // Phase 0: Fill chamber A and confirm it's full
  if (heartbeatPhase == 0) {
    if (stepperA.currentPosition() == 0 && stepperB.currentPosition() == 0) {
      stepperA.moveTo(-500);  // Start filling A
      heartbeatPhase = 1;
      displayBPM(currentBPM);
    }
  }
  // Phase 1: Wait for A to be completely full before starting next phase
  else if (heartbeatPhase == 1) {
    if (stepperA.currentPosition() == -500) {
      stepperA.moveTo(0);     // Start emptying A
      stepperB.moveTo(1000);  // Start filling B
      heartbeatPhase = 2;
      displayBPM(currentBPM);
    }
  }
  // Phase 2: Wait for both A to empty AND B to fill completely
  else if (heartbeatPhase == 2) {
    if (stepperA.currentPosition() == 0 && stepperB.currentPosition() == 1000) {
      stepperB.moveTo(0);     // Start emptying B
      heartbeatPhase = 3;
      displayBPM(currentBPM);
    }
  }
  // Phase 3: Wait for B to completely empty before starting next cycle
  else if (heartbeatPhase == 3) {
    if (stepperB.currentPosition() == 0) {
      heartbeatPhase = 0;     // Reset to start of cycle
      displayBPM(currentBPM);
    }
  }

  // Run both motors
  stepperA.run();
  stepperB.run();
}


void displayBPM(int bpm) {
    // Only update if BPM has changed
    static int lastDisplayedBPM = -1;
    if (bpm == lastDisplayedBPM) {
        return;
    }
    
    // Position for BPM text (adjust these coordinates as needed)
    const int BPM_X = 5;
    const int BPM_Y = 5;
    
    // Clear only the BPM area (not the whole display)
    display.fillRect(BPM_X, BPM_Y, 70, 64, SSD1306_BLACK);
    
    display.setTextSize(4);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(BPM_X, BPM_Y);
    
    if (bpm == 0) {
        display.print("OFF");
    } else {
        display.print(bpm);
    }
    
    display.display();  // Update the display
    lastDisplayedBPM = bpm;  // Store the new value
}