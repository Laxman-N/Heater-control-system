#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Pin Definitions
#define ONE_WIRE_BUS 4 // DS18B20 data wire is connected to GPIO4 on your ESP32
#define HEATER_LED_PIN 16 // Heater LED is connected to GPIO16
#define FEEDBACK_LED_PIN 17 // Feedback LED is connected to GPIO17
#define BUZZER_PIN 18 // Buzzer is connected to GPIO18

// State Machine Definitions
enum HeaterState {
IDLE,
HEATING,
STABILIZING,
TARGET_REACHED,
OVERHEAT
};

HeaterState currentState = IDLE;

// Temperature Thresholds (°C)
const float TARGET_TEMP = 35.0;
const float HEATING_THRESHOLD = 32.0;
const float OVERHEAT_THRESHOLD = 40.0;
const float STABILIZE_RANGE = 1.0;
const float COOLDOWN_THRESHOLD = 25.0;

// DS18B20 Sensor Setup
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Variables
float currentTempC = 0.0;
unsigned long lastLogTime = 0;
const unsigned long LOG_INTERVAL_MS = 2000; // Log every 2 seconds

// Function to read temperature from DS18B20 sensor
float readTemperature() {
sensors.requestTemperatures();
float temperatureC = sensors.getTempCByIndex(0);
return temperatureC;
}

// Function to update the state machine
void updateState() {
currentTempC = readTemperature();

switch (currentState) {
case IDLE:
digitalWrite(HEATER_LED_PIN, LOW);
digitalWrite(FEEDBACK_LED_PIN, LOW);
noTone(BUZZER_PIN);
if (currentTempC < HEATING_THRESHOLD) {
currentState = HEATING;
}
break;

case HEATING:
digitalWrite(HEATER_LED_PIN, HIGH);
digitalWrite(FEEDBACK_LED_PIN, HIGH);
noTone(BUZZER_PIN);
if (currentTempC >= TARGET_TEMP - STABILIZE_RANGE && currentTempC <= TARGET_TEMP + STABILIZE_RANGE) {
currentState = STABILIZING;
} else if (currentTempC >= OVERHEAT_THRESHOLD) {
currentState = OVERHEAT;
}
break;

case STABILIZING:
digitalWrite(HEATER_LED_PIN, LOW);
digitalWrite(FEEDBACK_LED_PIN, LOW);
noTone(BUZZER_PIN);
if (currentTempC < HEATING_THRESHOLD) {
currentState = HEATING;
} else if (currentTempC >= TARGET_TEMP - STABILIZE_RANGE && currentTempC <= TARGET_TEMP + STABILIZE_RANGE) {
currentState = TARGET_REACHED;
} else if (currentTempC >= OVERHEAT_THRESHOLD) {
currentState = OVERHEAT;
}
break;

case TARGET_REACHED:
digitalWrite(HEATER_LED_PIN, LOW);
digitalWrite(FEEDBACK_LED_PIN, HIGH);
noTone(BUZZER_PIN);
if (currentTempC < HEATING_THRESHOLD) {
currentState = HEATING;
} else if (currentTempC >= OVERHEAT_THRESHOLD) {
currentState = OVERHEAT;
}
break;

case OVERHEAT:
digitalWrite(HEATER_LED_PIN, LOW);
digitalWrite(FEEDBACK_LED_PIN, LOW);
tone(BUZZER_PIN, 1000);
if (currentTempC < COOLDOWN_THRESHOLD) {
currentState = IDLE;
}
break;
}
}

// Function to log data to the Serial Monitor
void logData() {
if (millis() - lastLogTime >= LOG_INTERVAL_MS) {
Serial.print("Current State: ");
switch (currentState) {
case IDLE: Serial.print("IDLE"); break;
case HEATING: Serial.print("HEATING"); break;
case STABILIZING: Serial.print("STABILIZING"); break;
case TARGET_REACHED: Serial.print("TARGET_REACHED"); break;
case OVERHEAT: Serial.print("OVERHEAT"); break;
}
Serial.print(" | Temperature: ");
Serial.print(currentTempC);
Serial.println(" °C");
lastLogTime = millis();
}
}

void setup() {
Serial.begin(115200);
pinMode(HEATER_LED_PIN, OUTPUT);
pinMode(FEEDBACK_LED_PIN, OUTPUT);
pinMode(BUZZER_PIN, OUTPUT);
// Start the DS18B20 sensor
sensors.begin();
currentState = IDLE;
Serial.println("Intelligent Heater Control System - Starting Up...");
Serial.println("System initialized. Current State: IDLE");
}

void loop() {
updateState();
logData();
}