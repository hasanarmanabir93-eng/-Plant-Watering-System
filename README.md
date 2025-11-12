# ESP32 Smart Plant Watering System

An automatic plant watering system using **ESP32**, **Sinric Pro**, and a **capacitive soil moisture sensor**.  
Monitors soil moisture and controls a water pump automatically or via IoT dashboard.

## Features
- Real-time soil moisture monitoring (0–100%)
- Automatic pump control based on moisture levels
- Remote control via Sinric Pro app (ON/OFF)
- Push notifications for dry soil or sensor unplugged
- Professional, modular code structure

## Hardware Required
- ESP32 Development Board
- Capacitive Soil Moisture Sensor
- Relay Module (for water pump)
- Submersible Water Pump
- Jumper Wires & Power Supply

## Circuit Diagram
![Circuit Diagram](IMG/circuit.png)

| Component | ESP32 Pin |
|-----------|-----------|
| Soil Sensor | GPIO 17 |
| Relay / Pump | GPIO 16 |

## Software / Libraries
- Arduino IDE
- SinricPro Library
- WiFi.h
- CapacitiveSoilMoistureSensor.h (custom header)

## Installation
1. Clone the repository:
```bash
git clone https://github.com/hasanarmanabir93-eng/ESP32-Plant-Watering-System.git
