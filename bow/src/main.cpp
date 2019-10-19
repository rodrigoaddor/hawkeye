#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

#define ssid "Hawkeye"
#define pass "hawkeyequiver"

#define host "http://192.168.4.1/?arrow="
#define port 80

const int pins[][2] = {
    {D1, 1},
    {D2, 2},
    {D6, 3},
    {D7, 4},
};

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < 4; i++) {
    Serial.print("Setting up pin ");
    Serial.println(pins[i][0]);
    pinMode(pins[i][0], INPUT_PULLUP);
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWifi connected");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  for (int i = 0; i < 4; i++) {
    int pin = pins[i][0];
    int arrow = pins[i][1];
    if (!digitalRead(pin)) {
      Serial.printf("Read pin %d, arrow %d\n", pin, arrow);

      for (int i = 0; i < 3; i++) {
        String req = host + String(arrow);
        Serial.printf("Sending request to %s\n", req.c_str());

        HTTPClient http;
        http.begin(req);
        int httpCode = http.GET();

        http.end();
        if (httpCode == 200) {
          Serial.println("Success, leaving");
          break;
        } else {
          Serial.printf("Error code %d, retrying\n", httpCode);
        }
      }

      while (!digitalRead(pin)) delay(100);
    }
  }
}