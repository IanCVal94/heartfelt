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
#define MS1_PIN 21  // Add microstepping control pins
#define MS2_PIN 20
#define MS3_PIN 19



int loops = 0;
WiFiServer server(80);

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

float currentScale = 1.0;
bool scaleIncreasing = false;
String currentPacket = "";

// Modify these variables after the existing global variables
#define DEFAULT_BPM 60
#define PULSE_STEPS 50  // Number of steps for each pulse (adjust based on testing)
unsigned long lastPulseTime = 0;
bool pulsing = false;
int currentPosition = 0;

// Add this with the other global variables
int currentBPM = DEFAULT_BPM;  // Global BPM variable


// Add at the top with other globals
unsigned long lastLoopUpdate = 0;
const unsigned long LOOP_INTERVAL = 25;  // 25ms interval if needed

// Add timer interrupt variables
volatile int currentPeriodA = 1000;  // Period for stepper A
volatile int currentPeriodB = 1000;  // Period for stepper B
volatile bool stepPinStateA = false;
volatile bool stepPinStateB = false;
volatile long stepCountA = 0;
volatile long stepCountB = 0;
volatile bool directionA = true;  // true = forward, false = backward
volatile bool directionB = true;

// Timer interrupt handlers
void ARDUINO_ISR_ATTR onTimer0() {
  stepPinStateA = !stepPinStateA;
  digitalWrite(STEP_PIN_A, stepPinStateA);
  
  if (stepPinStateA) {  // Count only on rising edge
    if (directionA) stepCountA++; else stepCountA--;
    if (abs(stepCountA) >= PULSE_STEPS) {
      directionA = !directionA;
      digitalWrite(DIR_PIN_A, directionA);
      stepCountA = 0;
    }
  }
}

void ARDUINO_ISR_ATTR onTimer1() {
  stepPinStateB = !stepPinStateB;
  digitalWrite(STEP_PIN_B, stepPinStateB);
  
  if (stepPinStateB) {  // Count only on rising edge
    if (directionB) stepCountB++; else stepCountB--;
    if (abs(stepCountB) >= PULSE_STEPS) {
      directionB = !directionB;
      digitalWrite(DIR_PIN_B, directionB);
      stepCountB = 0;
    }
  }
}

void setupTimers() {
  // Configure pins
  pinMode(STEP_PIN_A, OUTPUT);
  pinMode(DIR_PIN_A, OUTPUT);
  pinMode(STEP_PIN_B, OUTPUT);
  pinMode(DIR_PIN_B, OUTPUT);
  pinMode(MS1_PIN, OUTPUT);
  pinMode(MS2_PIN, OUTPUT);
  pinMode(MS3_PIN, OUTPUT);
  
  // Configure microstepping
  digitalWrite(MS1_PIN, LOW);
  digitalWrite(MS2_PIN, LOW);
  digitalWrite(MS3_PIN, LOW);
  
  // Set initial directions
  digitalWrite(DIR_PIN_A, directionA);
  digitalWrite(DIR_PIN_B, directionB);
  
  // Configure timer interrupts - using 1MHz base frequency
  hw_timer_t *timer0 = timerBegin(1000000);  // 1MHz timer frequency
  hw_timer_t *timer1 = timerBegin(1000000);  // 1MHz timer frequency
  
  // Attach interrupt handlers
  timerAttachInterrupt(timer0, &onTimer0);
  timerAttachInterrupt(timer1, &onTimer1);
  
  // Set alarm to trigger at specified intervals (in microseconds)
  // true for auto-reload, 0 for unlimited repeats
  timerAlarm(timer0, currentPeriodA, true, 0);
  timerAlarm(timer1, currentPeriodB, true, 0);
}

void setup() {
  Serial.begin(115200);
  Wire.begin(9, 8);  // Set SDA to GPIO9, SCL to GPIO8

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();
  initWiFi();

  setupTimers();  // Add this line to setup()
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
  unsigned long currentTime = millis();
  
  // Only process updates every LOOP_INTERVAL milliseconds
  if (currentTime - lastLoopUpdate >= LOOP_INTERVAL) {
    lastLoopUpdate = currentTime;
    
    WiFiClient client = server.available();
    loops++;
    
    updateHeartAnimation(currentTime);

    
    if (client) {
      Serial.println("Client connected!");
      
      while (client.connected()) {
        currentTime = millis();
        if (currentTime - lastLoopUpdate >= LOOP_INTERVAL) {
          lastLoopUpdate = currentTime;
          
          if (client.available()) {
            String packet = client.readStringUntil('\n');
            packet.trim();
            
            if (packet.length() == 0 || packet == "0") {
              currentPacket = "...";
            } else {
              currentPacket = packet;
              // Update the BPM when we receive a valid packet
              currentBPM = packet.toInt();
            }

            client.println("SHello World1E " + String(loops));
            client.println("SHello World2E " + String(loops));
            Serial.println("Printed to client " + packet);
          }
          
          updateHeartAnimation(currentTime);
        }
      }
      Serial.println("Client disconnected");
      client.stop();
    }
  }
}

void updateHeartAnimation(unsigned long currentTime) {
  static unsigned long lastAnimationUpdate = 0;
  const unsigned long ANIMATION_INTERVAL = 25;
  
  if (currentTime - lastAnimationUpdate >= ANIMATION_INTERVAL) {
    // Update heart animation scale
    if (scaleIncreasing) {
      currentScale += 0.04;
      if (currentScale >= 1.0) {
        currentScale = 1.0;
        scaleIncreasing = false;
      }
    } else {
      currentScale -= 0.04;
      if (currentScale <= 0.6) {
        currentScale = 0.6;
        scaleIncreasing = true;
      }
    }
    lastAnimationUpdate = currentTime;
    
    // Update display
    display.clearDisplay();
    
    if (currentPacket == "..." || currentPacket == "") {
      // Loading animation
      static unsigned long lastDotUpdate = 0;
      static int dots = 0;
      
      if (currentTime - lastDotUpdate >= 500) {
        dots = (dots + 1) % 4;
        lastDotUpdate = currentTime;
      }
      
      String loadingText = "LOAD";
      for (int i = 0; i < dots; i++) {
        loadingText += ".";
      }
      displayNumberAndText(loadingText, currentScale);
    } else {
      displayNumberAndText(currentPacket, currentScale);
    }
  }
}

void displayNumberAndText(String text, float scale) {
  // Display text
  display.setTextSize(5);
  display.setTextColor(SSD1306_WHITE);
  
  // Left align the text with some padding
  display.setCursor(5, 10);  // 5 pixels from left edge
  display.println(text);
  
  // Only draw the heart if we're not in the loading state
  if (!text.startsWith("LOAD")) {
    // Calculate heart dimensions based on scale
    int baseRadius = 8;
    int radius = baseRadius * scale;
    int centerY = 25;  // Moved up from 32
    int leftX = 95;
    int rightX = 105;
    
    int triangleTop = centerY + (radius * 0.5);
    int triangleHeight = 24 * scale;
    int triangleWidth = (rightX - leftX) + (2 * radius);
    
    display.fillCircle(leftX, centerY, radius, SSD1306_WHITE);
    display.fillCircle(rightX, centerY, radius, SSD1306_WHITE);
    display.fillTriangle(
      leftX - radius, triangleTop,
      rightX + radius, triangleTop,
      (leftX + rightX) / 2, centerY + triangleHeight,
      SSD1306_WHITE
    );
  }
  
  display.display();
}

