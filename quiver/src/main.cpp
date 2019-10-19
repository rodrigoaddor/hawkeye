#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <Servo.h>

#define ssid "Hawkeye"
#define pass "hawkeyequiver"

#define speed 50  // Lower = faster

#define debug 0  // Enables Serial communication

ESP8266WebServer server(80);
Servo servo;

int currentTip = 1;

void rotateArrow() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 4; j++) {
      digitalWrite(D5, j == 0);
      digitalWrite(D6, j == 1);
      digitalWrite(D7, j == 2);
      digitalWrite(D8, j == 3);
      delay(speed);
    }
  }
}

void rotateTip(int tip) {
  while (currentTip != tip) {
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 4; j++) {
        digitalWrite(D1, j == 0);
        digitalWrite(D2, j == 1);
        digitalWrite(D3, j == 2);
        digitalWrite(D4, j == 3);
        delay(speed);
      }
    }

    currentTip++;
    if (currentTip > 4) currentTip = 1;
  }
}

void triggerServo() {
  for (int pos = 0; pos <= 90; pos++) {
    servo.write(pos);
    delay(15);
  }
  for (int pos = 90; pos >= 0; pos--) {
    servo.write(pos);
    delay(15);
  }
}

void handleRoot() {
  int tip = server.arg(0).toInt();

  if (debug) Serial.printf("Selected tip %d\n", tip);

  server.send(200);

  rotateArrow();
  rotateTip(tip);
  triggerServo();
}

void setup() {
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
  pinMode(D10, OUTPUT);

  servo.attach(D10);

  delay(1000);
  if (debug) {
    Serial.begin(115200);
    Serial.println("Configuring access point...");
  }

  WiFi.softAP(ssid, pass);

  server.on("/", handleRoot);
  server.begin();
  if (debug) Serial.println("HTTP server started");
}

void loop() { server.handleClient(); }