/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);
  
  Wire.begin(9, 8);  // Set SDA to GPIO9, SCL to GPIO8
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();

  // Main countdown loop
  for (int i = 99; i >= 0; i--) {
    // Create smooth heart animation with 10 frames
    for (float scale = 1.0; scale >= 0.6; scale -= 0.04) {
      displayNumberAndHeart(i, scale);
      delay(25);  // Shorter delay for smoother animation
    }
    for (float scale = 0.6; scale <= 1.0; scale += 0.04) {
      displayNumberAndHeart(i, scale);
      delay(25);
    }
  }
}

void loop() {
  // You can add your main program logic here
}

void displayNumberAndHeart(int number, float scale) {
  display.clearDisplay();
  
  // Display number
  display.setTextSize(5);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println(String(number));
  
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