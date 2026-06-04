# ESP32 WiFi Smart Obstacle Detection Car

## Project Overview

The ESP32 WiFi Smart Obstacle Detection Car is a WiFi-controlled robotic vehicle developed using an ESP32 microcontroller operating in Access Point (AP) mode.

The car hosts a web-based joystick interface that allows users to control movement directly from a smartphone without requiring a router or internet connection.

An HC-SR04 ultrasonic sensor continuously measures the distance to obstacles. When an object is detected within 15 cm, the ESP32 automatically stops the car, blocks forward movement, and activates a buzzer warning to prevent collisions.

## Features

- WiFi control through ESP32 Access Point
- Web-based joystick interface
- Real-time motor control
- Differential steering
- Obstacle detection
- Automatic emergency stop
- Buzzer warning system
- Collision prevention

## Hardware Components

- ESP32 DevKit (38-pin)
- L298N Motor Driver
- HC-SR04 Ultrasonic Sensor
- Active Buzzer
- 2 DC Gear Motors
- 2WD Robot Chassis
- Breadboard
- 1kΩ resistor
- 2kΩ resistor
- Battery Pack
- Power Bank

## System Architecture

Phone
↓
ESP32 Web Server
↓
L298N Motor Driver
↓
DC Motors

ESP32
↓
HC-SR04
↓
Obstacle Detection
↓
Buzzer

## Pin Configuration

| Component | ESP32 Pin |
|-----------|----------|
| IN1 | GPIO27 |
| IN2 | GPIO26 |
| IN3 | GPIO18 |
| IN4 | GPIO19 |
| TRIG | GPIO23 |
| ECHO | GPIO32 |
| Buzzer | GPIO25 |

## Obstacle Detection Logic

Distance = Duration × 0.034 / 2

Obstacle Threshold = 15 cm

When an obstacle is detected:
- Stop motors immediately
- Block forward movement
- Allow reverse movement
- Activate buzzer warning

## Results

- Control range: 15–30 meters
- Near-instant response time
- Reliable obstacle detection
- Successful collision prevention

## Programming Language

C++ using Arduino Framework

## Author

Mohamed Hamed
