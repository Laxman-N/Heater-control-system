Intelligent Heater Control System
Project Overview
This project implements a state-driven, closed-loop temperature control system using an ESP32 microcontroller. The system continuously monitors the temperature from a DS18B20 sensor and controls a simulated heater (represented by an LED) to maintain a target temperature. The system provides real-time feedback through a serial monitor, LEDs, and an audible buzzer.

Features
Finite State Machine: The core logic is built around a state machine with the following states: IDLE, HEATING, STABILIZING, TARGET_REACHED, OVERHEAT, and SENSOR_ERROR.

Temperature Monitoring: Reads temperature from a DS18B20 digital temperature sensor.

Automated Heater Control: Turns a simulated heater (LED) on and off based on pre-defined temperature thresholds.

Visual & Audio Feedback: Uses LEDs to indicate the heating status and target temperature, and a buzzer for state transitions and alarms.

Serial Logging: Logs the current state, temperature, and heater status to the Serial Monitor.

Hardware Requirements
ESP32 DevKitC

DS18B20 Temperature Sensor

1x Red LED (for Heater Indicator)

1x Green LED (for Feedback Indicator)

1x Buzzer

1x 4.7kΩ Pull-up Resistor (crucial for DS18B20 communication)

2x 220Ω Resistors (for LEDs)

Breadboard and jumper wires

Setup and Wiring
To replicate this project, follow the wiring instructions below. Be sure to connect the 4.7kΩ pull-up resistor between the DS18B20's data pin and its power pin.

DS18B20 Sensor:

VCC -> 3.3V on ESP32

GND -> GND on ESP32

DQ (Data) -> GPIO4 on ESP32

4.7kΩ Resistor connects between DQ and VCC

Red LED (Heater):

Long Leg (Anode) -> 220Ω Resistor

Other end of Resistor -> GPIO16 on ESP32

Short Leg (Cathode) -> GND

Green LED (Feedback):

Long Leg (Anode) -> 220Ω Resistor

Other end of Resistor -> GPIO17 on ESP32

Short Leg (Cathode) -> GND

Buzzer:

Positive (+) pin -> GPIO18 on ESP32

Negative (-) pin -> GND

Code
The code for this project is contained within the sketch.ino file.

Wokwi Simulation
You can view and run a live simulation of this project on Wokwi.
(https://wokwi.com/projects/439194819403460609)
