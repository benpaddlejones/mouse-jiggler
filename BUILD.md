# Build Instructions - USB Mouse Jiggler

Complete step-by-step instructions for building and programming your USB Mouse Jiggler.

> **⚡ Quick Start:** Don't want to build from source? Download the pre-built firmware:  
> **[Download usb-jiggler.uf2 (v1.0.0)](https://github.com/benpaddlejones/mouse-jiggler/releases/download/v1.0.0/usb-jiggler.uf2)**  
> See [README.md](README.md) for flashing instructions.

## Table of Contents

1. [Prerequisites](#prerequisites)
2. [Hardware Setup](#hardware-setup)
3. [Software Installation](#software-installation)
4. [Building the Firmware](#building-the-firmware)
5. [Uploading to Device](#uploading-to-device)
6. [Testing](#testing)
7. [Troubleshooting](#troubleshooting)
8. [Customization](#customization)

---

## Prerequisites

### Required Hardware
- Raspberry Pi Pico, Pico W, or Pico 2
- Micro USB to USB-A cable
- Computer (Windows, Mac, or Linux)

### Required Software
- Arduino IDE (version 1.8.19 or later, or 2.x recommended)
- Internet connection for downloading dependencies

### Estimated Time
- First-time setup: 15-20 minutes
- Subsequent builds: 2-3 minutes

---

## Hardware Setup

### Step 1: Inspect Your Hardware

1. **Remove the Pico from packaging**
2. **Locate the BOOTSEL button** on the Pico (small white button near the USB port)
3. **Check the USB cable** - ensure it's a data cable, not just a charging cable
4. **Do not connect the Pico yet** - we'll do this during the upload process

### Step 2: Physical Preparation

- No soldering or assembly required
- The Pico can be used as-is for this project
- Optional: Place the Pico in an enclosure after programming

---

## Software Installation

### Step 1: Install Arduino IDE

#### Option A: Arduino IDE 2.x (Recommended)

1. Go to [https://www.arduino.cc/en/software](https://www.arduino.cc/en/software)
2. Download Arduino IDE 2.3.x for your operating system:
   - Windows: Download `.exe` or `.msi` installer
   - Mac: Download `.dmg` file
   - Linux: Download `.AppImage` or use package manager
3. Install Arduino IDE following the installer prompts
4. Launch Arduino IDE

#### Option B: Arduino IDE 1.8.x (Legacy)

1. Go to [https://www.arduino.cc/en/software](https://www.arduino.cc/en/software)
2. Scroll down to "Legacy IDE (1.8.X)"
3. Download and install for your OS
4. Launch Arduino IDE

### Step 2: Add Raspberry Pi Pico Board Support

1. **Open Arduino IDE**

2. **Add Board Manager URL**:
   - Click **File → Preferences** (or **Arduino IDE → Settings** on Mac)
   - Find "Additional Boards Manager URLs" field
   - Paste this URL:
     ```
     https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
     ```
   - If there are other URLs already there, click the button next to the field and add it on a new line
   - Click **OK**

   ![Board Manager URL](https://docs.arduino.cc/static/4e20f58141a9e235bb66ac0dfdfdf130/a6d36/board-manager-preferences.png)

3. **Install Pico Board Package**:
   - Click **Tools → Board → Boards Manager**
   - In the search box, type: `pico`
   - Find **"Raspberry Pi Pico/RP2040"** by Earle F. Philhower, III
   - Click **Install**
   - Wait for installation to complete (may take 5-10 minutes, ~500MB download)
   - Status will show "INSTALLED" when complete

   ![Boards Manager](https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2021/05/Boards-Manager-Install-Raspberry-Pi-Pico-RP2040.png)

4. **Verify Installation**:
   - Click **Tools → Board**
   - You should see **"Raspberry Pi RP2040 Boards"** in the menu
   - Hover over it to see board options

### Step 3: Install Adafruit TinyUSB Library

1. **Open Library Manager**:
   - Click **Sketch → Include Library → Manage Libraries...**
   - Or click **Tools → Manage Libraries...**

2. **Search and Install**:
   - In the search box, type: `Adafruit TinyUSB`
   - Find **"Adafruit TinyUSB Library"** by Adafruit
   - Click **Install**
   - If prompted to install dependencies, click **Install All**
   - Wait for "INSTALLED" status

   ![Library Manager](https://cdn-learn.adafruit.com/assets/assets/000/093/689/medium800/adafruit_products_Arduino_Library_Manager.png)

3. **Close Library Manager** when complete

---

## Building the Firmware

### Step 1: Create New Sketch

1. **Open a new sketch**:
   - Click **File → New** (or press `Ctrl+N` / `Cmd+N`)
   - Arduino IDE will create a new sketch with empty `setup()` and `loop()` functions

2. **Delete the default code** (everything in the window)

### Step 2: Copy the Firmware Code

Copy and paste this complete code into Arduino IDE:

```cpp
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
```

### Step 3: Save the Sketch

1. Click **File → Save** (or press `Ctrl+S` / `Cmd+S`)
2. Name it: `mouse-jiggler` (Arduino will create a folder with this name)
3. Choose a location you'll remember (e.g., Documents/Arduino/)
4. Click **Save**
5. Arduino will create a folder structure: `mouse-jiggler/mouse-jiggler.ino`

### Step 4: Configure Board Settings

⚠️ **CRITICAL STEP** - These settings must be correct:

1. **Select Board**:
   - Click **Tools → Board → Raspberry Pi RP2040 Boards**
   - Choose your board:
     - **Raspberry Pi Pico** (for original Pico or Pico W)
     - **Raspberry Pi Pico 2** (for Pico 2 with RP2350)

2. **Select USB Stack** ⚠️ **MOST IMPORTANT**:
   - Click **Tools → USB Stack**
   - Select **"Adafruit TinyUSB"**
   - ❌ Do NOT use "Pico SDK" for this project

3. **Other Settings** (optional, defaults are fine):
   - **Flash Size**: Leave as default (2MB)
   - **CPU Speed**: Leave as default (133 MHz)
   - **Optimize**: Leave as default (-Os Small)

### Step 5: Verify/Compile the Code

1. Click the **Verify** button (✓ checkmark icon) in the top-left toolbar
2. Watch the output panel at the bottom
3. Wait for compilation to complete (10-30 seconds)
4. You should see:
   ```
   Sketch uses XXXX bytes (X%) of program storage space.
   Global variables use XXXX bytes (X%) of dynamic memory.
   Done compiling.
   ```
5. If you see errors, see [Troubleshooting](#troubleshooting) section

---

## Uploading to Device

### Step 1: Put Pico in Bootloader Mode

1. **Unplug the Pico** if it's connected
2. **Hold down the BOOTSEL button** on the Pico (small white button near USB connector)
3. **While holding BOOTSEL**, plug the USB cable into your computer
4. **Keep holding BOOTSEL** for 2-3 seconds
5. **Release BOOTSEL button**
6. The Pico should appear as a USB drive named **"RPI-RP2"** or **"RP2350"**

   ![BOOTSEL Button Location](https://cdn.shopify.com/s/files/1/0609/6011/2892/files/pico_pinout_1024x1024.png)

### Step 2: Select Port (IDE 2.x) or Skip (IDE 1.x)

**For Arduino IDE 2.x:**
1. Click **Tools → Port**
2. Look for the port showing your Pico
   - May show as "UF2 Board" or "RP2 Boot"
3. Select it

**For Arduino IDE 1.8.x:**
- No port selection needed when in BOOTSEL mode
- Skip to next step

### Step 3: Upload the Firmware

1. Click the **Upload** button (→ arrow icon) in the top-left toolbar
2. Watch the output panel for progress
3. You'll see:
   ```
   Resetting /dev/ttyACMX
   Converting to uf2, output size: XXXXX, start address: 0x10000000
   Flashing XXXXX bytes to 0x10000000
   Wrote XXXXX bytes to /media/.../RPI-RP2/NEW.UF2
   ```
4. The Pico will automatically disconnect and reboot
5. Wait for "Done uploading" message

### Step 4: Verify Upload Success

After upload completes:
1. **The Pico will reboot automatically**
2. **The RPI-RP2 drive will disappear**
3. **The onboard LED should blink briefly then stay solid**
4. **A new HID device should appear** in your system's device manager

---

## Testing

### Step 1: Visual Check

1. **Check the LED**:
   - Should be **solid green** (device ready)
   - If blinking rapidly: Still initializing (wait 5-10 seconds)
   - If off: Problem with upload or power

### Step 2: Device Recognition

**Windows:**
1. Open **Device Manager** (`devmgmt.msc`)
2. Expand **Mice and other pointing devices**
3. You should see a new **HID-compliant mouse** device

**Mac:**
1. Click **  → About This Mac → System Report**
2. Select **USB** in the sidebar
3. Look for a new **USB HID Device**

**Linux:**
```bash
lsusb
```
Look for a new USB HID device in the list

### Step 3: Wait for First Movement

1. **Do not move your mouse**
2. **Watch the cursor** (place it in the middle of the screen)
3. **Wait up to 60 seconds** (first movement is random between 20-60 seconds)
4. **Cursor should move** automatically
5. **LED will blink briefly** when movement occurs

### Step 4: Verify Random Behavior

- Watch for 2-3 movements (may take up to 3 minutes total)
- Each movement should:
  - Occur at different time intervals (20-60 seconds apart)
  - Move different distances (100-300 pixels)
  - Move in different directions

✅ **If all tests pass, your Mouse Jiggler is working correctly!**

---

## Troubleshooting

### Compilation Errors

#### Error: "Adafruit_TinyUSB.h: No such file or directory"
**Solution**: Install Adafruit TinyUSB Library
- Go back to [Step 3: Install Adafruit TinyUSB Library](#step-3-install-adafruit-tinyusb-library)
- Make sure it says "INSTALLED" in Library Manager

#### Error: "TUD_HID_REPORT_DESC_MOUSE was not declared"
**Solution**: Check USB Stack setting
- Go to **Tools → USB Stack**
- Make sure **"Adafruit TinyUSB"** is selected (not "Pico SDK")

#### Error: "Fatal Python error: initfsencoding"
**Solution**: Corrupted board package installation
1. Close Arduino IDE
2. Delete folder: `C:\Users\[YourUsername]\AppData\Local\Arduino15\packages\rp2040`
3. Reopen Arduino IDE
4. Reinstall Pico board package from Boards Manager

### Upload Errors

#### Error: "No device found on [port]"
**Solution**: Pico not in bootloader mode
- Unplug Pico
- Hold BOOTSEL button
- Plug in while holding BOOTSEL
- Wait 2 seconds
- Release BOOTSEL
- Try upload again

#### Error: "Access denied" or "Permission denied"
**Solution**: Port access issue
- **Windows**: Close any serial monitors or programs using COM ports
- **Linux**: Add user to dialout group:
  ```bash
  sudo usermod -a -G dialout $USER
  ```
  Then log out and log back in
- **Mac**: Check System Settings → Privacy & Security

### Runtime Issues

#### LED Not Turning On
**Causes and solutions:**
1. **USB cable issue**: Try a different cable (must be data cable, not charge-only)
2. **USB port issue**: Try a different USB port on your computer
3. **Power issue**: Try a powered USB hub if using laptop
4. **Upload failed**: Re-upload firmware following all steps

#### LED Blinking Continuously
**Cause**: Device cannot enumerate as USB device
**Solutions:**
1. Unplug and replug the Pico
2. Try a different USB port
3. Verify USB Stack is set to "Adafruit TinyUSB"
4. Re-upload firmware

#### Mouse Not Moving
**Checks:**
1. **Wait longer**: First movement can take up to 60 seconds
2. **Check LED**: Should be solid (not blinking) when ready
3. **Check Device Manager**: Verify new HID mouse device appears
4. **Check code**: Verify you copied the complete firmware code
5. **Re-upload**: Try uploading firmware again

#### Device Not Recognized
**Solutions:**
1. **Try different USB port**: Some ports may not provide enough power
2. **Try different computer**: Verify Pico hardware is working
3. **Check cable**: Use a known-good USB data cable
4. **Reset Pico**: Unplug for 10 seconds, then plug back in

### Still Having Issues?

1. **Verify all dependencies installed**:
   - Boards Manager shows "INSTALLED" for Raspberry Pi Pico
   - Library Manager shows "INSTALLED" for Adafruit TinyUSB

2. **Verify settings**:
   - Tools → Board → Raspberry Pi Pico (or Pico 2)
   - Tools → USB Stack → **Adafruit TinyUSB** ✓

3. **Test with blink sketch first**:
   ```cpp
   void setup() {
     pinMode(LED_BUILTIN, OUTPUT);
   }
   void loop() {
     digitalWrite(LED_BUILTIN, HIGH);
     delay(1000);
     digitalWrite(LED_BUILTIN, LOW);
     delay(1000);
   }
   ```
   - Use **Tools → USB Stack → Pico SDK** for this test
   - If blink works, hardware is okay
   - Switch back to Adafruit TinyUSB for mouse jiggler

---

## Customization

### Changing Movement Timing

To adjust how often the mouse moves, modify this line:

```cpp
// Change from 20-60 seconds to something else
nextMoveDelay = random(20000, 60001);  // values in milliseconds
```

**Examples:**
- 10-30 seconds: `random(10000, 30001)`
- 30-90 seconds: `random(30000, 90001)`
- 1-2 minutes: `random(60000, 120001)`

### Changing Movement Distance

To adjust how far the mouse moves, modify this line:

```cpp
// Change from 100-300 pixels to something else
int distance = random(100, 301);
```

**Examples:**
- Small movements (50-150 pixels): `random(50, 151)`
- Large movements (200-500 pixels): `random(200, 501)`
- Tiny movements (10-50 pixels): `random(10, 51)`

### Fixed Movement Pattern

To make the mouse move in a predictable pattern instead of randomly:

```cpp
void loop() {
  if (millis() - lastMoveTime >= 30000) {  // Fixed 30 seconds
    if (usb_hid.ready()) {
      // Move in a circle pattern
      usb_hid.mouseMove(0, 50, 0);   // Right
      delay(100);
      usb_hid.mouseMove(0, 0, 50);   // Down
      delay(100);
      usb_hid.mouseMove(0, -50, 0);  // Left
      delay(100);
      usb_hid.mouseMove(0, 0, -50);  // Up
      
      // Blink LED
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
      digitalWrite(LED_BUILTIN, HIGH);
    }
    lastMoveTime = millis();
  }
  delay(100);
}
```

### Adjusting LED Behavior

To disable LED blinking on movement:

```cpp
// Comment out or remove these lines in loop():
digitalWrite(LED_BUILTIN, LOW);
delay(100);
digitalWrite(LED_BUILTIN, HIGH);
```

To make LED blink continuously (always visible):

```cpp
void loop() {
  // Add at start of loop
  digitalWrite(LED_BUILTIN, (millis() / 500) % 2);  // Blink every 500ms
  
  // ... rest of your code
}
```

### Export Compiled Binary

After making your changes and verifying the code compiles:

1. Click **Sketch → Export Compiled Binary**
2. Wait for compilation to finish
3. Find the `.uf2` file in your sketch folder
   - Look in the same folder as your `.ino` file
   - The file will be named something like: `mouse-jiggler.ino.uf2` or `mouse-jiggler.ino.raspberrypi.uf2`
4. You can distribute this file or use it to flash multiple Picos
5. To flash: Put Pico in bootloader mode and copy the `.uf2` file to the RPI-RP2 drive

---

## Additional Resources

### Official Documentation

- **[Raspberry Pi Pico Documentation](https://www.raspberrypi.com/documentation/microcontrollers/raspberry-pi-pico.html)** - Official hardware and software documentation
- **[Arduino-Pico GitHub](https://github.com/earlephilhower/arduino-pico)** - Board support package documentation
- **[Adafruit TinyUSB Library](https://github.com/adafruit/Adafruit_TinyUSB_Arduino)** - Library documentation and examples
- **[RP2040 Datasheet](https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf)** - Detailed chip specifications

### Community Resources

- **[Raspberry Pi Forums](https://forums.raspberrypi.com/viewforum.php?f=145)** - Official Pico discussion forum
- **[Arduino Forums](https://forum.arduino.cc/)** - General Arduino help and discussion
- **[Reddit r/raspberry_pi](https://www.reddit.com/r/raspberry_pi/)** - Community discussions and projects

### Related Projects

- **[Pre-built Firmware v1.0.0](https://github.com/benpaddlejones/mouse-jiggler/releases/tag/v1.0.0)** - Download ready-to-use UF2 file
- **[Project Repository](https://github.com/benpaddlejones/mouse-jiggler)** - Source code and updates

---

## Version History

### v1.0.0 (Current)
- Initial release
- Random movement every 20-60 seconds
- Random direction and distance (100-300 pixels)
- LED status indicator
- Support for Pico, Pico W, and Pico 2

### Future Plans
- Configurable timing via button presses
- Multiple movement patterns
- Web-based configuration (for Pico W)

---

## Need Help?

If you're stuck or have questions:

1. **Check the [Troubleshooting](#troubleshooting) section** - Most common issues are covered
2. **Read the [README](README.md)** - Quick start guide and testing instructions
3. **Visit the [Releases Page](https://github.com/benpaddlejones/mouse-jiggler/releases/tag/v1.0.0)** - For pre-built firmware
4. **[Open an Issue](https://github.com/benpaddlejones/mouse-jiggler/issues)** - For bugs or feature requests
5. **[Start a Discussion](https://github.com/benpaddlejones/mouse-jiggler/discussions)** - For general questions

---

**Happy building! 🎉**

*If you found this guide helpful, please consider starring the repository on GitHub!*