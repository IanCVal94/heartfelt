#include <WiFi.h>

const char* ssid = "MIT";
const char* password = "i%739nKGFT";

WiFiServer server(80);
int loops = 0;
void setup() {
  Serial.begin(115200);
  
  // Create Access Point
  WiFi.softAP(ssid, password);
  Serial.println("Access Point Started2");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());
  
  server.begin();
}


void loop() {
  WiFiClient client = server.available();
}