#include "StatusLeds.h"

#define BLINK_LED_EVERY_MS 500
//#define ENABLE_BLINK

StatusLeds::StatusLeds(uint8_t led_red_pin, uint8_t led_green_pin)
    : _led_red_pin(led_red_pin), _led_green_pin(led_green_pin) {}

void StatusLeds::setup() {
  pinMode(_led_red_pin, OUTPUT);
  pinMode(_led_green_pin, OUTPUT);
}

void StatusLeds::handle() {
#ifdef ENABLE_BLINK
  auto now = millis();
  if (now - _last_led_toggle_timestamp > BLINK_LED_EVERY_MS) {
    _last_led_toggle_timestamp = now;
    digitalWrite(_led_red_pin, _last_led_value);
    digitalWrite(_led_green_pin, !_last_led_value);
    _last_led_value = !_last_led_value;
  }
#endif
}

void StatusLeds::setRed(bool on) { digitalWrite(_led_red_pin, on); }
void StatusLeds::setGreen(bool on) { digitalWrite(_led_green_pin, on); }