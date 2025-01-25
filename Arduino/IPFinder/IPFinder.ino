#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const char* ssid = "Xavier";      // Your WiFi network name
const char* password = "tacocat4642";      // Your WiFi password

int loops = 0;
WiFiServer server(80);

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

float currentScale = 1.0;
bool scaleIncreasing = false;
String currentPacket = "";

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
  WiFiClient client = server.available();
  loops++;
  
  unsigned long currentTime = millis();  // Track current time for animations
  
  if (client) {
    Serial.println("Client connected!");
    
    while (client.connected()) {
      String packet = client.readStringUntil('\n');
      packet.trim();  // Remove any whitespace or newline characters
      
      if (packet.length() == 0 || packet == "0") {
        currentPacket = "...";
      } else {
        currentPacket = packet;
      }

      client.println("SHello World1E " + String(loops));
      client.println("SHello World2E " + String(loops));
      Serial.println("Printed to client " + packet);
      
      // Update display while connected
      updateDisplay(currentTime);
      delay(100);
    }
    Serial.println("Client disconnected");
    client.stop();
  }

  // Update display when not connected
  updateDisplay(currentTime);
  delay(25);  // Control animation speed
}

// New function to handle display updates
void updateDisplay(unsigned long currentTime) {
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
}

void displayNumberAndText(String text, float scale) {
  // Clear is now handled in loop()
  
  // Display text
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);  // Use the defined constant
  
  // Center the text
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(text.c_str(), 0, 0, &x1, &y1, &w, &h);  // Use c_str() for compatibility
  int xpos = (SCREEN_WIDTH - w) / 2;
  display.setCursor(xpos, 10);  // Moved text up slightly
  display.println(text);
  
  // Calculate heart dimensions based on scale
  int baseRadius = 8;
  int radius = baseRadius * scale;
  int centerY = 32;
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
  
  display.display();  // Make sure this is called
}