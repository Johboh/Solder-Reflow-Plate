#include "Adafruit_MAX31855.h"
#include "wifi_credentials.h"
#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ElegantOTA.h>
#include <SPI.h>

// Using libraries
// https://github.com/ayushsharma82/ElegantOTA
// https://github.com/adafruit/Adafruit-MAX31855-library

// Update at http://192.168.1.163:81/update

// WIFI
// See wifi_credentials.h where the following should be defined:
// const char user_hostname[] = "";
// const char user_wifi_ssid[] = "";
// const char user_wifi_passsword[] = "";

// PINS
#define LED1_PIN 8
#define LED2_PIN 9
#define MAX31855_CS_PIN 38
#define MAX31855_SCK_PIN 36
#define MAX31855_SO_PIN 37
#define GATE_PIN 10
#define VIN_MEASURE_PIN 1

// INTERVALS
#define BLINK_LED_EVERY_MS 500
#define READ_THERMOCOUPLE_EVERY_MS 1000

WebServer _elegant_ota_server(81);
bool _last_led_value = false;
unsigned long _last_led_toggle_timestamp = 0;
unsigned long _last_thermocouple_timestamp = 0;
Adafruit_MAX31855 _thermocouple(MAX31855_SCK_PIN, MAX31855_CS_PIN, MAX31855_SO_PIN); // TODO(johboh): Move to own class

void setupSerial() {
  Serial.begin(115200);
  Serial.println("");
  Serial.println("Booting...");
}

void setupWifi() {
  Serial.println("Setting up Wifi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(user_wifi_ssid, user_wifi_passsword);
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

void setupOta() {
  ArduinoOTA.onStart([]() { Serial.println("OTA: Start"); });
  ArduinoOTA.onEnd([]() { Serial.println("\nOTA: End"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("OTA: Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("OTA: Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)
      Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR)
      Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR)
      Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR)
      Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR)
      Serial.println("End Failed");
  });
  ArduinoOTA.begin();

  ElegantOTA.begin(&_elegant_ota_server);
  ElegantOTA.setID(user_hostname);
  _elegant_ota_server.begin();
}

void setupThermocouple() {
  Serial.print("Setting up max31855... ");
  int8_t retries = 3;
  while (!_thermocouple.begin()) {
    Serial.print("Failed!");
    if (--retries < 0) {
      break;
    }
    Serial.println("Retrying setup of max31855... ");
    delay(200);
  }
  if (retries > 0) {
    Serial.print("Success!");
  }
}

void setup() {
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  setupSerial();
  setupWifi();
  setupOta();
  setupThermocouple();
}

void loop() {
  ArduinoOTA.handle();
  _elegant_ota_server.handleClient();

  auto now = millis();
  if (now - _last_led_toggle_timestamp > BLINK_LED_EVERY_MS) {
    _last_led_toggle_timestamp = now;
    digitalWrite(LED1_PIN, _last_led_value);
    digitalWrite(LED2_PIN, !_last_led_value);
    _last_led_value = !_last_led_value;
  }

  if (now - _last_thermocouple_timestamp > READ_THERMOCOUPLE_EVERY_MS) {
    _last_thermocouple_timestamp = now;
    Serial.print("Internal Temp = ");
    Serial.println(_thermocouple.readInternal());

    double c = _thermocouple.readCelsius();
    if (isnan(c)) {
      Serial.println("Something wrong with thermocouple!");
    } else {
      Serial.print("C = ");
      Serial.println(c);
    }
  }
}