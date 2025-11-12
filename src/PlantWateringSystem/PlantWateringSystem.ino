/**********************************************************************************
 *  ESP32 Smart Plant Watering System using Sinric Pro & Capacitive Soil Sensor
 *  Author: Hasan Arman
 *  Description: Automatically monitors soil moisture and controls pump.
 **********************************************************************************/

#include <Arduino.h>
#include <WiFi.h>
#include <SinricPro.h>
#include <SinricProSwitch.h>
#include "CapacitiveSoilMoistureSensor.h"

// -----------------------------------------------------------------------------
// 🔐 Credentials (Replace with your own)
// -----------------------------------------------------------------------------
#define APP_KEY     "YOUR_APP_KEY"
#define APP_SECRET  "YOUR_APP_SECRET"

#define SOIL_DEVICE_ID  "YOUR_SOIL_SENSOR_DEVICE_ID"
#define PUMP_DEVICE_ID  "YOUR_PUMP_SWITCH_DEVICE_ID"

#define WIFI_SSID   "YOUR_WIFI"
#define WIFI_PASS   "YOUR_PASSWORD"

// -----------------------------------------------------------------------------
// ⚙️ Hardware Configuration
// -----------------------------------------------------------------------------
constexpr int RELAY_PIN = 16;   // Relay to control pump
constexpr int SOIL_PIN  = 17;   // Soil sensor analog pin

// -----------------------------------------------------------------------------
// 🌡️ Sensor Calibration (adjust to your sensor)
// -----------------------------------------------------------------------------
constexpr int VERY_DRY   = 2910;
constexpr int NEUTRAL    = 2098;
constexpr int VERY_WET   = 925;
constexpr int DRY_ALERT  = 2850;
constexpr int UNPLUGGED  = 3000;

// -----------------------------------------------------------------------------
// 🔧 Global Variables
// -----------------------------------------------------------------------------
int lastSoilValue = 0;
String lastSoilState = "";

CapacitiveSoilMoistureSensor &soilSensor = SinricPro[SOIL_DEVICE_ID];
SinricProSwitch &pumpSwitch = SinricPro[PUMP_DEVICE_ID];

// -----------------------------------------------------------------------------
// 💡 Pump Control Handler
// -----------------------------------------------------------------------------
bool onPowerState(const String& deviceId, bool &state) {
  if (deviceId == PUMP_DEVICE_ID) {
    digitalWrite(RELAY_PIN, state ? HIGH : LOW);
    Serial.printf("[Pump] %s\n", state ? "ON" : "OFF");
  }
  return true;
}

// -----------------------------------------------------------------------------
// 🌱 Soil Moisture Processing
// -----------------------------------------------------------------------------
void handleSoilMoisture() {
  if (!SinricPro.isConnected()) return;

  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate < 60000) return; // Update every 60 sec
  lastUpdate = millis();

  int rawValue = analogRead(SOIL_PIN);
  int moisturePercent = map(rawValue, VERY_WET, VERY_DRY, 100, 0);
  moisturePercent = constrain(moisturePercent, 0, 100);

  Serial.printf("[Sensor] ADC: %d | Moisture: %d%%\n", rawValue, moisturePercent);

  if (rawValue == lastSoilValue) return;

  String soilState = (rawValue > NEUTRAL) ? "Dry" : "Wet";
  if (soilState != lastSoilState) {
    soilSensor.sendModeEvent("modeInstance1", soilState, "PHYSICAL_INTERACTION");
    lastSoilState = soilState;
  }

  soilSensor.sendRangeValueEvent("rangeInstance1", moisturePercent);

  // 🔔 Notifications
  if (rawValue > DRY_ALERT) soilSensor.sendPushNotification("Soil is too dry! Please water your plants.");
  if (rawValue > UNPLUGGED) soilSensor.sendPushNotification("Soil sensor might be unplugged!");

  lastSoilValue = rawValue;
}

// -----------------------------------------------------------------------------
// 🌐 Wi-Fi Setup
// -----------------------------------------------------------------------------
void setupWiFi() {
  Serial.printf("[WiFi] Connecting to %s", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  WiFi.setSleep(false);
  WiFi.setAutoReconnect(true);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.printf("\n[WiFi] Connected! IP: %s\n", WiFi.localIP().toString().c_str());
}

// -----------------------------------------------------------------------------
// ☁️ Sinric Pro Setup
// -----------------------------------------------------------------------------
void setupSinricPro() {
  pumpSwitch.onPowerState(onPowerState);
  SinricPro.onConnected([] { Serial.println("[SinricPro] Connected"); });
  SinricPro.onDisconnected([] { Serial.println("[SinricPro] Disconnected"); });
  SinricPro.begin(APP_KEY, APP_SECRET);
}

// -----------------------------------------------------------------------------
// 🚀 Arduino Setup
// -----------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  pinMode(SOIL_PIN, INPUT);

  setupWiFi();
  setupSinricPro();
}

// -----------------------------------------------------------------------------
// 🔄 Main Loop
// -----------------------------------------------------------------------------
void loop() {
  SinricPro.handle();
  handleSoilMoisture();
}
