#include "StatusLeds.h"

StatusLeds::StatusLeds(uint8_t led_red_pin, uint8_t led_green_pin)
    : _led_red_pin(led_red_pin), _led_green_pin(led_green_pin) {}

void StatusLeds::setup() {
  pinMode(_led_red_pin, OUTPUT);
  pinMode(_led_green_pin, OUTPUT);
}

void StatusLeds::handle() {}

void StatusLeds::setRed(bool on) { digitalWrite(_led_red_pin, on); }
void StatusLeds::setGreen(bool on) { digitalWrite(_led_green_pin, on); }