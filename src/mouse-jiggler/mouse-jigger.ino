#include "Adafruit_TinyUSB.h"

// HID report descriptor for mouse
uint8_t const desc_hid_report[] = {
  TUD_HID_REPORT_DESC_MOUSE()
};

// Create HID device
Adafruit_USBD_HID usb_hid(desc_hid_report, sizeof(desc_hid_report), HID_ITF_PROTOCOL_MOUSE, 2, false);

unsigned long lastMoveTime = 0;
unsigned long nextMoveDelay = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  usb_hid.begin();
  
  // Wait for USB enumeration (up to 5 seconds)
  for (int i = 0; i < 50 && !TinyUSBDevice.mounted(); i++) {
    digitalWrite(LED_BUILTIN, i % 2);  // Blink while waiting
    delay(100);
  }
  
  digitalWrite(LED_BUILTIN, HIGH);  // Solid LED when ready
  
  // Seed random number generator
  randomSeed(analogRead(26));  // Use floating pin for randomness
  
  // Set first random delay (20-60 seconds)
  nextMoveDelay = random(20000, 60001);  // milliseconds
  lastMoveTime = millis();
}

void loop() {
  // Check if it's time to move the mouse
  if (millis() - lastMoveTime >= nextMoveDelay) {
    
    // Only move if USB is ready
    if (usb_hid.ready()) {
      
      // Random distance: 100-300 pixels
      int distance = random(100, 301);
      
      // Random direction (0-360 degrees)
      float angle = random(0, 360) * 3.14159 / 180.0;  // Convert to radians
      
      // Calculate x and y components
      int totalX = (int)(cos(angle) * distance);
      int totalY = (int)(sin(angle) * distance);
      
      // Move in small increments (max ±127 per report)
      while (totalX != 0 || totalY != 0) {
        int8_t moveX = 0;
        int8_t moveY = 0;
        
        // Move X
        if (totalX > 127) {
          moveX = 127;
          totalX -= 127;
        } else if (totalX < -127) {
          moveX = -127;
          totalX += 127;
        } else {
          moveX = totalX;
          totalX = 0;
        }
        
        // Move Y
        if (totalY > 127) {
          moveY = 127;
          totalY -= 127;
        } else if (totalY < -127) {
          moveY = -127;
          totalY += 127;
        } else {
          moveY = totalY;
          totalY = 0;
        }
        
        // Send movement
        usb_hid.mouseMove(0, moveX, moveY);
        delay(5);  // Small delay between movements
      }
      
      // Blink LED to show movement happened
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
      digitalWrite(LED_BUILTIN, HIGH);
    }
    
    // Set next random delay (20-60 seconds)
    nextMoveDelay = random(20000, 60001);
    lastMoveTime = millis();
  }
  
  delay(100);  // Check every 100ms
}