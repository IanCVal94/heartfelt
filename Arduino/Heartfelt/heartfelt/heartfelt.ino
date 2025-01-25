#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Add these global variables at the top
const char* ssid = "MIT";
const char* password = "i%739nKGFT";
WiFiServer server(80);
String currentBPM = "";

void setup() {
  Serial.begin(115200);
  
  Wire.begin(9, 8);  // Set SDA to GPIO9, SCL to GPIO8
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  // Initialize WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  server.begin();
  
  display.clearDisplay();
}

void loop() {
  WiFiClient client = server.available();
  
  if (client) {
    while (client.connected()) {
      String packet = client.readStringUntil('\n');
      currentBPM = packet;
      client.println("SACK"); // Acknowledge receipt
      break;
    }
    client.stop();
  }

  // Display animation
  if (currentBPM == "") {
    // Loading animation when no BPM
    static float scale = 1.0;
    static bool growing = false;
    
    displayNumberAndHeart(-1, scale); // -1 indicates loading state
    
    if (growing) {
      scale += 0.04;
      if (scale >= 1.0) {
        growing = false;
      }
    } else {
      scale -= 0.04;
      if (scale <= 0.6) {
        growing = true;
      }
    }
    delay(25);
  } else {
    // Display BPM with pulsing heart
    static float scale = 1.0;
    static bool growing = false;
    
    displayNumberAndHeart(currentBPM.toInt(), scale);
    
    if (growing) {
      scale += 0.04;
      if (scale >= 1.0) {
        growing = false;
      }
    } else {
      scale -= 0.04;
      if (scale <= 0.6) {
        growing = true;
      }
    }
    delay(25);
  }
}

void displayNumberAndHeart(int number, float scale) {
  display.clearDisplay();
  
  // Display number or "---" if loading
  display.setTextSize(5);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  if (number == -1) {
    display.println("---");
  } else {
    display.println(String(number));
  }
  
  // Calculate heart dimensions based on scale
  int baseRadius = 8;  // Keep the same large size
  int radius = baseRadius * scale;
  int centerY = 32;
  int leftX = 95;      // Brought circles closer together
  int rightX = 105;    // Reduced separation between circles
  
  // Adjusted triangle calculations to match circles perfectly
  int triangleTop = centerY + (radius * 0.5);
  int triangleHeight = 24 * scale;
  int triangleWidth = (rightX - leftX) + (2 * radius);
  
  // Display heart with current scale
  display.fillCircle(leftX, centerY, radius, WHITE);
  display.fillCircle(rightX, centerY, radius, WHITE);
  display.fillTriangle(
    leftX - radius, triangleTop,
    rightX + radius, triangleTop,
    (leftX + rightX) / 2, centerY + triangleHeight,
    WHITE
  );
  
  display.display();
}