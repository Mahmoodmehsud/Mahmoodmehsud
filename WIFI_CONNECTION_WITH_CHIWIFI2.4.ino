
#include <WiFi.h>

// Replace with your network credentials (STATION)
const char* ssid = "CHIWifi2.4";
const char* password = "L3tme1n@CHI";

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  initWiFi();
  Serial.println("Connected to : CHIWifi2.4");
  Serial.print("Signal Strength: ");
  Serial.println(WiFi.RSSI());
}

void loop() {
  // put your main code here, to run repeatedly:
}
