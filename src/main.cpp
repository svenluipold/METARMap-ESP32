#include <Arduino.h>
#include "secrets.h"

#define LED_PIN 2   // Onboard-LED (oft GPIO2)


void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(LED_PIN, LOW);
  delay(500);
}
