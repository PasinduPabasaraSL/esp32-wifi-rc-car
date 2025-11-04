# ESP32 RC Car Receiver — ESP-NOW Controlled Motion Driver

[![Platform: ESP32](https://img.shields.io/badge/platform-ESP32-orange.svg)]()  
[![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)  
[![Status: Stable](https://img.shields.io/badge/status-stable-green.svg)]()

**Smart RC Car Receiver built for ESP-NOW control.**  
Receives direction commands from a joystick transmitter (`F`, `B`, `L`, `R`, `S`) and drives motors using an L298N/L293D or similar H-bridge motor driver. Designed for smooth motion, reliable stops, and minimal delay.

---

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Hardware](#hardware)
- [Wiring](#wiring)
- [Setup](#setup)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)
- [License](#license)

---

## Overview
This RC Car receiver is the companion to the **ESP32 Vector Joystick Transmitter**.  
It listens for commands over **ESP-NOW**, decodes the direction, and drives motors accordingly.  
Each command corresponds to a simple state:

| Command | Action |
|----------|---------|
| `F` | Move Forward |
| `B` | Move Backward |
| `L` | Turn Left |
| `R` | Turn Right |
| `S` | Stop Motors |

The receiver is designed for **fast response** and **graceful stop behavior** with non-blocking motor control and simple update logic.

---

## Features
- Real-time wireless control using **ESP-NOW**
- Compatible with **joystick transmitter**
- Supports most H-bridge motor drivers (L298N, L293D, BTS7960)
- Smooth stop handling with minimal delay
- Configurable motor pin assignments
- LED or Serial feedback for debugging
- Low-power idle mode when stopped

---

## Hardware

### Required Components
| Component | Quantity | Notes |
|------------|-----------|-------|
| ESP32 Dev Board | 1 | Receiver |
| L298N or L293D Motor Driver | 1 | Dual-channel motor driver |
| DC Motors | 2 | Left and Right |
| Power Source | 1 | 7–12V (depending on motors) |
| Jumper Wires | — | For connections |

---

## Wiring

| Motor Driver Pin | Connects To | Description |
|------------------|-------------|--------------|
| IN1 | ESP32 GPIO 26 | Left Motor Forward |
| IN2 | ESP32 GPIO 27 | Left Motor Backward |
| IN3 | ESP32 GPIO 14 | Right Motor Forward |
| IN4 | ESP32 GPIO 12 | Right Motor Backward |
| ENA | 5V PWM (optional) | Speed control left |
| ENB | 5V PWM (optional) | Speed control right |
| +12V | Motor Power | Depends on motor rating |
| GND | ESP32 GND | Common ground |

Optional:
- Status LED on GPIO 2 (lights when connected or command received).

---

## Setup

1. Clone the repository:
```bash
git clone https://github.com/PasinduPabasaraSL/esp32-wifi-rc-car.git
cd esp32-wifi-rc-car
```

2. Open in **PlatformIO** or **Arduino IDE**.

3. Update this line with the transmitter’s MAC address:
```c
uint8_t controllerMac[] = { 0x20, 0xE7, 0xC8, 0x68, 0xB8, 0x30 };
```

4. Flash the sketch to your ESP32 receiver board.

5. Open Serial Monitor (115200 baud). You should see:
```
ESP-NOW Receiver Ready
Paired with controller: 20:E7:C8:68:B8:30
```

### ESP-NOW Setup
```c
esp_now_init();
esp_now_register_recv_cb(onReceive);
esp_now_add_peer(&peerInfo);
```

---

## Testing

1. Power both transmitter and receiver boards.
2. Verify they are in range (same Wi-Fi channel and power source).
3. Move the joystick — observe motor direction changes:
   - Forward → both motors spin forward
   - Backward → both spin backward
   - Left → left motor stops, right motor spins
   - Right → right motor stops, left motor spins
   - Release → stop motors
4. Serial Monitor prints the received command:
```
Received: F
Received: S
Received: R
```
---

## License
Released under the **MIT License**.  
You are free to use, modify, and distribute this project for personal or educational use.

---

## Credits

Created by **Pasindu Pabasara (RootCipher)**  
Part of the **Dynamic RC Car Obstacle Race Track** system.  
Includes ESP-NOW joystick transmitter + receiver pair for smooth wireless control.
