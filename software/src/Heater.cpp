#include "heater.h"

#define EVALUATE_EVERY_MS 100

Heater::Heater(Voltage &voltage, Thermocouple &thermocouple) : _voltage(voltage), _thermocouple(thermocouple) {}

void Heater::setup() { _voltage.setDutyCyclePercent(0.0); }

void Heater::handle() {
  if (_running) {
    auto now = millis();
    if (now - _last_evalulation_ms > EVALUATE_EVERY_MS) {
      evaulate();
      _last_evalulation_ms = now;
    }
  } else {
    _voltage.setDutyCyclePercent(0.0);
  }
}

void Heater::start() {
  _running = true;
  _last_evalulation_ms = 0;
}

void Heater::requestTemperature(float temperature, float max_duty_cycle_percent, uint8_t aggressiveness) {
  _aggressiveness = aggressiveness;
  _target_temperature = temperature;
  _max_duty_cycle_percent = max_duty_cycle_percent;
}

void Heater::stop() {
  _voltage.setDutyCyclePercent(0.0);
  _running = false;
}

// Not PID, but some kind of adjustment.
// TODO (johboh): Adjust with full heatbeds later on, and add i/d part.
void Heater::evaulate() {
  auto now = millis();
  auto duty = _voltage.getDutyCyclePercent();

  // If we are off/idle, turn fully off.
  if (_target_temperature <= 20) {
    _voltage.setDutyCyclePercent(0.0);
    return;
  }

  _thermocouple.update();
  float temperature = _thermocouple.getBedTemperature();
  if (isnan(temperature)) {
    // Oh no, no temperature! Try again soon.
    _last_evalulation_ms = millis() + EVALUATE_EVERY_MS / 2;
    return;
  }

  if (_target_temperature > temperature && duty < _max_duty_cycle_percent) {
    // How far away are we?
    float distance = _target_temperature - temperature;
    float new_duty_percent = distance * (_max_duty_cycle_percent / (float)_aggressiveness);
    new_duty_percent = min(new_duty_percent, _max_duty_cycle_percent);
    _voltage.setDutyCyclePercent(new_duty_percent);
  } else if (_target_temperature < temperature && duty > 0) {
    // We want to decrease.
    // TODO (johboh): Don't go fully off.
    _voltage.setDutyCyclePercent(0.0);
  }

  _last_evalulation_ms = millis();
}