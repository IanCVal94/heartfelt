#include <WiFi.h>

const char* ssid = "MIT";      // Your WiFi network name
const char* password = "i%739nKGFT";      // Your WiFi password

int loops = 0;
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  initWiFi();
}

void initWiFi() {
  WiFi.mode(WIFI_STA);
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