# USB Mouse Jiggler for Raspberry Pi Pico

A USB HID mouse jiggler that prevents computers from going to sleep by randomly moving the mouse cursor every 20-60 seconds.

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Raspberry%20Pi%20Pico-red.svg)](https://www.raspberrypi.com/documentation/microcontrollers/raspberry-pi-pico.html)
[![Version](https://img.shields.io/badge/version-1.1.0-green.svg)](https://github.com/benpaddlejones/mouse-jiggler/releases/tag/v1.1.0)
[![Release](https://img.shields.io/github/v/release/benpaddlejones/mouse-jiggler)](https://github.com/benpaddlejones/mouse-jiggler/releases/latest)

**Quick Links:** [Download v1.1.0](https://github.com/benpaddlejones/mouse-jiggler/releases/download/v1.1.0/usb-jiggler.uf2) • [Build Guide](BUILD.md) • [Releases](https://github.com/benpaddlejones/mouse-jiggler/releases) • [Report Issue](https://github.com/benpaddlejones/mouse-jiggler/issues)

## ✨ Features

- 🎲 Random movement every 20-60 seconds (first move after just 5-10 seconds)
- 📏 Moves up to 40 pixels per axis in random directions
- 🚀 Startup self-test: confirms the mouse works the moment it's plugged in
- 🛡️ Hardware watchdog auto-recovers the device if a USB send ever hangs
- 💡 LED status patterns for startup, ready, and movement
- 🔌 Plug and play - no software installation needed
- ✅ Compatible with Raspberry Pi Pico (RP2040) and Pico 2 (RP2350)
- 🖥️ Works on Windows, macOS, and Linux

## 📦 Download

**Get the latest firmware:** [Download usb-jiggler.uf2 (v1.1.0)](https://github.com/benpaddlejones/mouse-jiggler/releases/download/v1.1.0/usb-jiggler.uf2)

Or browse all releases: [Releases Page](https://github.com/benpaddlejones/mouse-jiggler/releases)

## 🚀 How to Flash

### What You Need

- Raspberry Pi Pico (RP2040) or Pico 2 (RP2350)
- USB cable (data-capable, not charge-only)
- Computer (Windows, macOS, or Linux)

### Installation Steps

1. **Download the firmware**
   - Go to the [v1.1.0 release](https://github.com/benpaddlejones/mouse-jiggler/releases/tag/v1.1.0)
   - Download [`usb-jiggler.uf2`](https://github.com/benpaddlejones/mouse-jiggler/releases/download/v1.1.0/usb-jiggler.uf2)

2. **Put Pico in bootloader mode**
   - Unplug your Pico from the computer
   - Hold down the **BOOTSEL** button (white button on the board)
   - While holding BOOTSEL, plug the USB cable into your computer
   - Keep holding for 2 seconds, then release
   - A drive named **RPI-RP2** should appear

3. **Flash the firmware**
   - Drag and drop `usb-jiggler.uf2` onto the **RPI-RP2** drive
   - The Pico will automatically reboot
   - The RPI-RP2 drive will disappear

4. **Verify it's working**
   - The LED stays **off** while the Pico connects to USB
   - Then it gives **3 slow blinks** and goes **solid** (ready)
   - The cursor does a quick **right-then-left self-test move** right away
   - The first random move happens after 5-10 seconds, then every 20-60 seconds
   - The LED gives **3 quick blinks** each time the mouse moves

### Visual Guide

```
Step 1: Hold BOOTSEL          Step 2: Plug in USB          Step 3: Drag UF2 file
    ┌─────────┐                   ┌─────────┐                  ┌─────────────┐
    │  PICO   │                   │  PICO   │ ──USB──>         │  RPI-RP2    │
    │  [🔘]   │ <── Hold          │         │                  │             │
    └─────────┘                   └─────────┘                  │ Drop file   │
                                                               │ here        │
                                                               └─────────────┘
```

## 💡 LED Indicators

| LED Pattern | Meaning |
|------------|---------|
| **Off** | USB device connecting (waiting for host) |
| **3 slow blinks** (~0.5s each) | Device enumerated and starting up |
| **Solid ON** | Ready and waiting |
| **3 quick blinks** (~0.15s each) | Mouse movement occurred |
| **Stuck off** | Error - try reflashing or another USB port |

## 🎯 Usage

### Normal Operation

- Plug the Pico into any USB port
- Wait for the 3 startup blinks and solid LED (device ready)
- The cursor does a quick self-test move, then jiggles randomly every 20-60 seconds
- No configuration needed!

### Stopping the Jiggler

- **Temporary:** Unplug the USB cable
- **Permanent:** Flash different firmware or hold BOOTSEL and delete the firmware

## 🛠️ Troubleshooting

### Pico Not Recognized

**Problem:** RPI-RP2 drive doesn't appear

**Solutions:**
- Use a data-capable USB cable (not charge-only)
- Try a different USB port
- Hold BOOTSEL for full 2 seconds before releasing
- Try on a different computer

### Mouse Not Moving

**Problem:** LED is solid but mouse doesn't move

**Solutions:**
- Wait the full 60 seconds (first delay is random)
- Check Device Manager (Windows) or System Information (Mac) for new HID mouse device
- Try unplugging and replugging the Pico
- Reflash the firmware

### LED Not Lighting Up

**Problem:** No LED activity at all

**Solutions:**
- Check USB cable is working (try with another device)
- Try a different USB port
- The Pico may be damaged - try another Pico

### Computer Security Software Blocking

**Problem:** Antivirus/security software alerts

**Solutions:**
- This is a legitimate HID device, not malware
- Add exception in your security software
- Use only on systems you own or have permission to use

## 📋 Supported Hardware

| Board | Chip | Status |
|-------|------|--------|
| Raspberry Pi Pico | RP2040 | ✅ Fully Tested |
| Raspberry Pi Pico W | RP2040 | ✅ Fully Tested |
| Raspberry Pi Pico 2 | RP2350 | ✅ Fully Tested |

## 📖 Technical Details

### Movement Behavior

| Parameter | Value |
|-----------|-------|
| **First Movement** | 5-10 seconds after startup (random) |
| **Movement Interval** | 20-60 seconds (random) thereafter |
| **Movement Distance** | -40 to +40 pixels per axis (random) |
| **Movement Direction** | Random on each axis |
| **Movement Speed** | Single instantaneous move |
| **Startup Self-Test** | +50px right, then -50px left |
| **Watchdog Timeout** | 5 seconds (auto-reset on hang) |

### Hardware Specifications

| Specification | Value |
|--------------|-------|
| **USB Device Class** | HID Mouse |
| **Power Draw** | ~50mA typical |
| **Supported Boards** | Pico, Pico W, Pico 2 |
| **USB Connection** | Micro USB |

### Software Compatibility

| Operating System | Minimum Version |
|-----------------|-----------------|
| **Windows** | Windows 10+ |
| **macOS** | macOS 10.14+ (Mojave) |
| **Linux** | Kernel 4.4+ |
| **Chrome OS** | Version 89+ |

## ❓ Frequently Asked Questions

### Can I customize the movement timing?
Yes! See [BUILD.md](BUILD.md) for instructions on modifying the firmware to change timing, distance, and movement patterns.

### Will this prevent sleep/lock on my system?
Yes, the random mouse movements simulate user activity and prevent most systems from entering sleep mode or activating screen locks.

### Is this detectable by system administrators?
Yes, the device appears as a standard USB HID mouse and movements are visible on screen. Always get permission before use in managed environments.

### Does this work with Pico W?
Yes! The Pico W is fully supported. The wireless capabilities are not used, but the device functions identically to the standard Pico.

### Can I use this with a KVM switch?
Yes, the device should work with most KVM switches, though some switches may require re-initialization when switching between computers.

### What's the difference between this and software-based jigglers?
This is a hardware device that appears as a real USB mouse, making it harder to detect than software solutions. It works regardless of what's running on your computer.

## 🔨 Building from Source

Want to modify or build the firmware yourself?

See [BUILD.md](BUILD.md) for detailed compilation instructions.

**Quick build:**
```bash
# Requirements
- Arduino IDE with "Arduino Mbed OS RP2040 Boards" board support
  (no extra USB libraries needed — native USBMouse HID is built in)

# Build steps
1. Open src/mouse-jiggler/mouse-jiggler.ino in Arduino IDE
2. Select Tools → Board → Arduino Mbed OS RP2040 Boards → Raspberry Pi Pico
3. Click Sketch → Export Compiled Binary
```

## ⚠️ Important Notes

### Legal & Ethical Use

- ✅ Use only on systems you own
- ✅ Get permission before use in corporate environments
- ✅ Respect your organization's security policies
- ❌ Do not use to circumvent legitimate security measures
- ❌ Do not use on systems without authorization

### Security Considerations

- This device appears as a standard USB mouse
- Some security software may flag unrecognized HID devices
- Movement is visible on screen and may be detected
- Not suitable for high-security environments

## � Version Notes

### v1.1.0

- 🎲 Random mouse jiggle every 20-60 seconds, with a faster 5-10 second first move
- 📏 Random movement of -40 to +40 pixels per axis
- 🚀 Startup self-test move (+50px right, -50px left) to confirm the device works immediately
- 💡 LED status patterns: off while connecting, 3 slow startup blinks, solid when ready, 3 quick blinks on each move
- 🛡️ Hardware watchdog (5s timeout) automatically resets and re-enumerates the device if a USB report send ever hangs (e.g. after host sleep/suspend)
- 🔧 Built on the native **Arduino Mbed OS RP2040** USB HID stack (`PluggableUSBHID` / `USBMouse`) — no external USB libraries required

## �📄 License

This project is released under the MIT License. See [LICENSE](LICENSE) for details.

```
MIT License - Copyright (c) 2025 benpaddlejones
```

## 🙏 Acknowledgments

- **[Arduino Mbed OS RP2040 Boards](https://github.com/arduino/ArduinoCore-mbed)** for the board core and built-in USB HID (`PluggableUSBHID` / `USBMouse`)
- **[Arduino](https://www.arduino.cc/)** for the IDE and toolchain
- **[Raspberry Pi Foundation](https://www.raspberrypi.org/)** for the RP2040 and RP2350

## � Documentation

- **[BUILD.md](BUILD.md)** - Complete build instructions from source
- **[Release v1.1.0](https://github.com/benpaddlejones/mouse-jiggler/releases/tag/v1.1.0)** - Pre-built firmware and release notes
- **[All Releases](https://github.com/benpaddlejones/mouse-jiggler/releases)** - Version history

## �📞 Support

- 🐛 [Report a Bug](https://github.com/benpaddlejones/mouse-jiggler/issues/new?labels=bug)
- 💡 [Request a Feature](https://github.com/benpaddlejones/mouse-jiggler/issues/new?labels=enhancement)
- 💬 [Ask a Question](https://github.com/benpaddlejones/mouse-jiggler/discussions)
- 📖 [Build from Source](BUILD.md)

## 🌟 Star History

If you find this project useful, please consider giving it a star! ⭐

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

---

**Made with ❤️ for the Raspberry Pi Pico community**

*Keep your computer awake, the simple way!*