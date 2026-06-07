// USB Mouse Jiggler for Raspberry Pi Pico
// Compatible with: Arduino Mbed OS RP2040 Boards
// Board setting: Tools → Board → Arduino Mbed OS RP2040 Boards → Raspberry Pi Pico

#include "PluggableUSBHID.h"
#include "USBMouse.h"

// Onboard LED on Raspberry Pi Pico is GPIO 25
#define LED_PIN 25

// Create USB Mouse device.
// Constructor blocks until USB is enumerated by the host (connect_blocking = true by default).
// The onboard LED will be OFF during this time.
USBMouse mouse;

unsigned long lastMoveTime = 0;
unsigned long nextMoveDelay = 0;

// Perform a jiggle: move a small random amount in a random direction.
// The OS registers real net cursor displacement to prevent sleep.
void performMouseJiggle() {
  // Random displacement: 10-40 pixels in each axis
  int16_t moveX = random(-40, 41);
  int16_t moveY = random(-40, 41);

  // Ensure at least some movement occurs
  if (moveX == 0 && moveY == 0) {
    moveX = 20;
  }

  mouse.move(moveX, moveY);
}

void setup() {
  pinMode(LED_PIN, OUTPUT);

  // USBMouse constructor already blocked until USB was enumerated,
  // so by the time we reach here the device is ready.

  // Startup blink pattern: 3 fast blinks to confirm device is running
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    digitalWrite(LED_PIN, LOW);
    delay(500);
  }

  digitalWrite(LED_PIN, HIGH);  // Solid LED when ready

  // Immediate test movement on startup: move 50px right then 50px left
  // so you can confirm the mouse is working right away
  delay(1000);
  mouse.move(50, 0);
  delay(500);
  mouse.move(-50, 0);

  // Seed random number generator
  randomSeed(analogRead(A0));  // Use floating analog pin for randomness

  // Set first random delay (5-10 seconds for quick first test, then 20-60 seconds after)
  nextMoveDelay = random(5000, 10001);  // milliseconds
  lastMoveTime = millis();
}

void loop() {
  // Check if it's time to move the mouse
  if (millis() - lastMoveTime >= nextMoveDelay) {

    // mouse.move() blocks internally until the report can be sent,
    // so no readiness check is needed.
    performMouseJiggle();

    // Blink LED to show movement happened (3 quick blinks)
    for (int i = 0; i < 3; i++) {
      digitalWrite(LED_PIN, LOW);
      delay(150);
      digitalWrite(LED_PIN, HIGH);
      delay(150);
    }

    // Set next random delay (20-60 seconds)
    nextMoveDelay = random(20000, 60001);
    lastMoveTime = millis();
  }

  delay(100);  // Check every 100ms
}