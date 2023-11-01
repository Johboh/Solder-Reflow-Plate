#include "Controller.h"
#include "Logger.h"
#include "Ota.h"
#include "StatusLeds.h"
#include "Thermocouple.h"
#include "Voltage.h"
#include "credentials.h"
#include "remote.h"
#include <Arduino.h>

// Bulit in Visual Studio Code using Ardunio extension v0.4.11
// Using esp arduino core 2.0.2 (https://github.com/espressif/arduino-esp32).
// Newer versions will probably work, but in case of any issues, try using this version.

// Using libraries
// https://github.com/knolleary/pubsubclient
// https://github.com/ayushsharma82/ElegantOTA
// https://github.com/adafruit/Adafruit-MAX31855-library

#define OTA_HTTP_PORT 81
// Update at http://10.20.1.133:81/update

// WIFI/MQTT
// credentials.h should define the following:
// const char user_hostname[] = "";
// const char user_wifi_ssid[] = "";
// const char user_wifi_password[] = "";
// const char mqtt_host[] = "";
// const char mqtt_username[] = "";
// const char mqtt_password[] = "";

// pin definitions
#define GATE_PIN 10
#define LED_RED_PIN 8
#define LED_GREEN_PIN 9
#define VIN_MEASURE_PIN 1
#define MAX31855_CS_PIN 38
#define MAX31855_SO_PIN 37
#define MAX31855_SCK_PIN 36

// Intervals
#define DEBUG_PRINTOUTS_EVERY_MS 1000

// members
Ota _ota(OTA_HTTP_PORT, user_hostname);
Voltage _voltage(VIN_MEASURE_PIN, GATE_PIN);
unsigned long _last_debug_printout_timestamp = 0;
StatusLeds _status_leds(LED_RED_PIN, LED_GREEN_PIN);
Thermocouple _thermocouple(MAX31855_CS_PIN, MAX31855_SCK_PIN, MAX31855_SO_PIN);
Logger _logger;
Controller _controller(_voltage, _status_leds, _thermocouple, _logger);
Remote _remote(_controller, _thermocouple, _voltage, _logger, mqtt_host, mqtt_username, mqtt_password);

void setupSerial() {
  Serial.begin(115200);
  Serial.println("");
  Serial.println("Booting...");
}

void setupWifi() {
  Serial.println("Setting up Wifi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(user_wifi_ssid, user_wifi_password);
  WiFi.setHostname(user_hostname);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  Serial.println("have wifi");
  Serial.print("IP number: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  setupSerial();
  setupWifi();
  _ota.setup();
  _remote.setup();
  _controller.setup();
}

void loop() {
  _ota.handle();
  _remote.handle();
  _controller.handle();

  // Debug printouts for now.
  auto now = millis();
  if (now - _last_debug_printout_timestamp > DEBUG_PRINTOUTS_EVERY_MS) {
    _last_debug_printout_timestamp = now;
    _controller.printDebug();
  }
}
