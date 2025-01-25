#include <WiFi.h>

const char* ssid = "heartfelt";
const char* password = "heartfelt";

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
  loops++;
  
  if (client) {
    Serial.println("Client connected!");
    
    char buffer[1024];

    while (client.connected()) {
      String packet = client.readStringUntil('\n');

      client.println("SHello World1E " + String(loops));
      client.println("SHello World2E " + String(loops));
      Serial.println("Printed to client " + packet);
      delay(100);
    }
    Serial.println("Client disconnected");
    client.stop();
  }
}