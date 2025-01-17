#include "ProfileStateHandler.h"

ProfileStateHandler::ProfileStateHandler() {}

std::optional<Profile::Step> ProfileStateHandler::getStep(Profile::State state) {
  for (const auto &step : _profile.steps) {
    if (step.state == state) {
      return step;
    }
  }
  return std::nullopt;
}

void ProfileStateHandler::reset() {
  _current_state = Profile::State::Idle;
  _k = 0;
  _m = 0;
}

void ProfileStateHandler::setProfile(Profile profile) {
  _profile = profile;
  reset();
}

void ProfileStateHandler::start(float idle_temperature) {
  auto now = millis();
  _start_time_ms = now;
  _step_start_time_ms = now;
  _current_state = Profile::State::Preheat;

  calculateKM(idle_temperature);
}

uint16_t ProfileStateHandler::targetTemperature() {
  auto step = getStep(_current_state);
  if (step) {
    unsigned long x = millis() - _step_start_time_ms;

    // Check if we reached end of step.
    if (x > step->total_runtime_ms) {

      auto current_target_temperature = step->target_temperature_c;

      Serial.print("runtime exceeded, switching current state: ");
      switch (_current_state) {
      case Profile::State::Preheat:
        _current_state = Profile::State::Soak;
        Serial.println("Preheat -> Soak");
        break;
      case Profile::State::Soak:
        _current_state = Profile::State::Reflow;
        Serial.println("Soak -> Reflow");
        break;
      case Profile::State::Reflow:
        _current_state = Profile::State::Idle;
        Serial.println("Reflow -> Idle");
        break;
      default:
        Serial.println("Unknown!");
        break;
      }

      calculateKM(current_target_temperature);
      x = 0;
      _step_start_time_ms = millis();
    }

    // Once reached ramp up, we keep holding the requested temperature by clamping x to ramp up time.
    unsigned long max_x = min(x, step->ramp_up_ms);
    uint16_t target_temperature = _k * max_x + _m;

    return target_temperature;
  } else {
    return 0;
  }
}

float ProfileStateHandler::getMaxDutyCycle() {
  auto step = getStep(_current_state);
  if (step) {
    float val = min(1.0f, step->max_duty_cycle_percent);
    val = max(0.0f, val);
    return val;
  } else {
    return NAN;
  }
}

uint8_t ProfileStateHandler::getAggressiveness() {
  auto step = getStep(_current_state);
  if (step) {
    uint8_t val = min((uint8_t)100, step->aggressiveness);
    val = max((uint8_t)1, step->aggressiveness);
    return val;
  } else {
    return 255;
  }
}

void ProfileStateHandler::calculateKM(float zero_time_temperature) {
  auto step = getStep(_current_state);
  // Linear equation calculation. Ramp up to set temperature during ramp up time.
  if (step && !isnan(zero_time_temperature)) {
    _m = zero_time_temperature;
    _k = ((double)step->target_temperature_c - _m) / ((double)step->ramp_up_ms);
  } else {
    _k = 0;
    _m = 0;
  }
  Serial.println("Caulate KM for " + String(zero_time_temperature, 2) + ", k: " + String(_k, 10) +
                 ", m: " + String(_m, 10));
}