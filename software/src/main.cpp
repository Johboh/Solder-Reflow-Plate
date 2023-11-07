#include "Controller.h"
#include "Logger.h"
#include "StatusLeds.h"
#include "Thermocouple.h"
#include "Voltage.h"
#include "credentials.h" // See credentials.h.template!
#include "remote.h"
#include <Arduino.h>
#include <OtaHelper.h>
#include <WiFiHelper.h>

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
OtaHelper _ota_helper(user_hostname);
WiFiHelper _wifi_helper(user_wifi_ssid, user_wifi_password, user_hostname);

Logger _logger;
Voltage _voltage(VIN_MEASURE_PIN, GATE_PIN);
unsigned long _last_debug_printout_timestamp = 0;
StatusLeds _status_leds(LED_RED_PIN, LED_GREEN_PIN);
Thermocouple _thermocouple(MAX31855_CS_PIN, MAX31855_SCK_PIN, MAX31855_SO_PIN);
Controller _controller(_voltage, _status_leds, _thermocouple, _logger);
Remote _remote(_controller, _thermocouple, _voltage, _logger, mqtt_host, mqtt_username, mqtt_password);

void setupSerial() {
  Serial.begin(115200);
  Serial.println("");
  Serial.println("Booting...");
}

void setup() {
  setupSerial();
  _wifi_helper.connect();
  _ota_helper.setup();
  _remote.setup();
  _controller.setup();
}

void loop() {
  _wifi_helper.handle();
  _ota_helper.handle();
  _remote.handle();
  _controller.handle();

  // Debug printouts for now.
  auto now = millis();
  if (now - _last_debug_printout_timestamp > DEBUG_PRINTOUTS_EVERY_MS) {
    _last_debug_printout_timestamp = now;
    _controller.printDebug();
  }
}
