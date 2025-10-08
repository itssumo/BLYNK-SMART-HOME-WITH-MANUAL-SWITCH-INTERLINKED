# ESP8266 Smart Home System

**Author:** Sumo  
**Platform:** NodeMCU (ESP8266)  
**App Integration:** Blynk IoT  
**Features:** 4-Channel Relay Control + Fan Speed (PWM) + Offline Mode + Local/Virtual Sync  

---

## Overview

This project is a **Smart Home Automation System** built using the **ESP8266 NodeMCU**.  
It allows you to **control 4 appliances** (via relays) and **1 DC fan** (via PWM) both **remotely** using the Blynk IoT app and **locally** using physical switches.

The **real and virtual switches are synchronized** — you can toggle a load using either the app or a physical switch, and both stay updated automatically.  
The system even works in **offline mode**, allowing manual control if Wi-Fi or internet is disconnected.

---

##  Features

- ✅ Control **4 appliances** using relays  
- ✅ Control **fan speed** using PWM (0–100%)  
- ✅ **Two-way sync** between physical switches and Blynk virtual buttons  
- ✅ **Offline operation** – switches still work if Wi-Fi is lost  
- ✅ Real-time feedback to app  
- ✅ Clean modular structure for easy expansion  

---

##  Working Principle

| Function | Description |
|-----------|-------------|
| **Physical Switches (D5–D8)** | Connected to input pins using `INPUT_PULLUP`. Pressing a switch toggles the corresponding relay and updates the Blynk app. |
| **Relays (D0, D2, D3, D4)** | Each relay is active-LOW. Controlled by app or switches. |
| **PWM Pin (D1)** | Drives a small DC fan; speed controlled via Blynk slider (V4). |
| **Sync Logic** | When either a local or app control changes state, the system updates both sides to stay consistent. |
| **Offline Mode** | If Wi-Fi or Blynk is not connected, relays still respond to physical switches. |

---

## 🪛 Hardware Setup

**Components Used**
- NodeMCU (ESP8266)
- 4-Channel Relay Module (5 V, Active-LOW)
- 4 Push Buttons / Switches (connected to GND)
- DC Fan or Motor (for PWM test)
- Power Supply 5 V (common GND)

**Pin Configuration**

| Component | NodeMCU Pin | Function |
|------------|--------------|-----------|
| Relay 1 | D0 | Output |
| Relay 2 | D2 | Output |
| Relay 3 | D3 | Output |
| Relay 4 | D4 | Output |
| Switch 1 | D5 | Input (active-LOW) |
| Switch 2 | D6 | Input (active-LOW) |
| Switch 3 | D7 | Input (active-LOW) |
| Switch 4 | D8 | Input (active-LOW) |
| Fan PWM | D1 | PWM Output (0–255) |

---

## Blynk Configuration

1. Create a new project on **Blynk IoT**.
2. Add the following widgets:
   | Widget | Virtual Pin | Function |
   |---------|--------------|-----------|
   | Button | V0 | Relay 1 |
   | Button | V1 | Relay 2 |
   | Button | V2 | Relay 3 |
   | Button | V3 | Relay 4 |
   | Slider | V4 | Fan Speed (0–100%) |
3. Set each Button to **Switch Mode**.
4. Copy your **Blynk Template ID** and **Auth Token** into the sketch.

---

## 🧩 Software & Libraries

- **Arduino IDE** or **PlatformIO**
- Install libraries:
  - `ESP8266WiFi.h`
  - `BlynkSimpleEsp8266.h`
- Blynk app (Android/iOS)

---

##  Usage

1. Flash the sketch to NodeMCU.
2. Connect NodeMCU to your Wi-Fi.
3. Open the Blynk app and use the buttons/sliders to control your appliances.
4. You can also toggle switches manually — both states remain in sync.

---

##  Optional Enhancements

- Add **EEPROM** or **LittleFS** to save last relay state after power loss.  
- Add **DHT11/DHT22 sensor** to display temperature & humidity in the app.  
- Integrate **voice control** via Alexa/Google Assistant using IFTTT.  
- Use **Blynk Timer** for scheduled automation (night light, fan, etc.).  

---

---

## ❤️ Acknowledgments

Created and developed by **itssumo** as a self-designed Smart Home system during engineering coursework.  
Improved and optimized for public sharing so juniors can learn real-world IoT logic — combining **hardware control**, **network synchronization**, and **cloud feedback** in a single system.

> “Logic first, syntax later — it’s the approach that matters.”
