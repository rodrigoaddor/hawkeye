#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

#define ssid "Hawkeye"
#define pass "hawkeyequiver"
#define port 7412

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
      Serial.print("Read pin ");
      Serial.println(pin);

      Serial.print("Sending ");
      Serial.println(host + String(arrow));

      HTTPClient http;
      http.begin(host + String(arrow));
      int httpCode = http.GET();

      if (httpCode == 200)
        Serial.println("Success");
      else
        Serial.println("Error");

      http.end();

      while (!digitalRead(pin)) delay(100);
    }
  }
}

/*
void loop() {
  Serial.print("connecting to ");
  Serial.print(host);
  Serial.print(':');
  Serial.println(port);

  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    delay(5000);
    return;
  }

  Serial.println("sending data to server");
  if (client.connected()) {
    client.println("hello from ESP8266");
  }

  // wait for data to be available
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      delay(60000);
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  Serial.println("receiving from remote server");
  // not testing 'client.connected()' since we do not need to send data here
  while (client.available()) {
    char ch = static_cast<char>(client.read());
    Serial.print(ch);
  }

  Serial.println();
  Serial.println("closing connection");
  client.stop();

  delay(300000);
}*/