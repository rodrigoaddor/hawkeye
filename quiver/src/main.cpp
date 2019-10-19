#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <Stepper.h>

#define ssid "Hawkeye"
#define pass "hawkeyequiver"
#define port 7412

ESP8266WebServer server(80);

Stepper arrowMotor(200, D1, D2, D3, D4);
int arrowMotorSteps = 0;

void handleRoot() {
  int arrow = server.arg(0).toInt();
  Serial.print("Selected arrow ");
  Serial.println(arrow);
  server.send(200);

  int steps = arrow * 50;
  arrowMotor.step(steps - arrowMotorSteps);
  arrowMotorSteps = steps;
}

void setup() {
  pinMode(LED_BUILTIN, INPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);

  arrowMotor.setSpeed(30);

  delay(1000);
  Serial.begin(115200);
  Serial.println("Configuring access point...");

  WiFi.softAP(ssid, pass);

  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() { server.handleClient(); }