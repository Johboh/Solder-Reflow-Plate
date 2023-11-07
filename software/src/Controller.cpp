#include "Controller.h"

Controller::Controller(Voltage &voltage, StatusLeds &status_leds, Thermocouple &thermocouple, Logger &logger)
    : _logger(logger), _voltage(voltage), _status_leds(status_leds), _thermocouple(thermocouple),
      _heater(_voltage, _thermocouple), _profiles(_logger) {}

void Controller::setup() {
  _voltage.setup();
  _status_leds.setup();
  _thermocouple.setup();
  _heater.setup();
  _heater.stop();

  // Up and running.
  _status_leds.setGreen(true);
  _status_leds.setRed(false);
}

void Controller::handle() {
  if (_current_profile != nullptr) {
    switch (_current_profile->getCurrentState()) {
    case Profile::State::Preheat:
      _current_state = State::Preheat;
      break;
    case Profile::State::Soak:
      _current_state = State::Soak;
      break;
    case Profile::State::Reflow:
      _current_state = State::Reflow;
      break;
    case Profile::State::Idle:
      _current_state = State::Idle;
      break;
    }
  }

  if (inRunningState() && _current_profile != nullptr) {
    float target_temperature = _current_profile->targetTemperature();
    float max_duty_cycle = _current_profile->getMaxDutyCycle();
    uint8_t aggressiveness = _current_profile->getAggressiveness();
    _heater.requestTemperature(target_temperature, max_duty_cycle, aggressiveness);
  } else {
    _heater.stop();
  }

  _voltage.handle();
  _status_leds.handle();
  _thermocouple.handle();
  _heater.handle();

  _status_leds.setRed(_voltage.getDutyCyclePercent() > 0.0);
}

bool Controller::selectProfile(String &profile_name) {
  _logger.log(Logger::Severity::Info, String("Trying to select profile <" + profile_name + ">").c_str());
  auto profile = _profiles.getProfile(profile_name);
  if (profile != nullptr) {
    _current_profile = profile;
    _logger.log(Logger::Severity::Info, String("Profile <" + profile_name + "> selected.").c_str());
    return true;
  } else {
    _logger.log(Logger::Severity::Error, String("Cannot select profile <" + profile_name + ">").c_str());
    _current_state = State::Error;
    return false;
  }
}

bool Controller::start() {
  _logger.log(Logger::Severity::Info, "Trying to start reflow.");
  if (_current_profile != nullptr) {
    _current_state = State::Idle;
    auto temperature = _thermocouple.getBedTemperature();
    if (isnan(temperature)) {
      _logger.log(Logger::Severity::Error, "No available bed temperature.");
      _current_state = State::Error;
      return false;
    } else {
      _current_profile->start(_thermocouple.getBedTemperature());
      _logger.log(Logger::Severity::Info, "Starting.");
    }
    _heater.start();
    return true;
  } else {
    _logger.log(Logger::Severity::Error, "Cannot start reflow. No profile selected.");
    _current_state = State::NoProfileSelected;
    return false;
  }
}

void Controller::stop() {
  _current_state = State::Idle;
  _voltage.setDutyCyclePercent(0.0);
  if (_current_profile != nullptr) {
    _current_profile->reset();
  }
  _heater.stop();
}

bool Controller::inRunningState() {
  return _current_profile != nullptr &&
         (_current_state == State::Preheat || _current_state == State::Soak || _current_state == State::Reflow);
}

uint16_t Controller::getTargetTemperature() {
  if (_current_profile != nullptr) {
    return _current_profile->targetTemperature();
  } else {
    return 0;
  }
}

void Controller::printDebug() {
  Serial.print("Internal temperature(C) = ");
  Serial.println(_thermocouple.getAmbientTemperature());

  double c = _thermocouple.getBedTemperature();
  if (isnan(c)) {
    Serial.println("Something wrong with thermocouple!");
  } else {
    Serial.print("Bed temperature(C) = ");
    Serial.println(c);
  }

  Serial.print("Vin(V) = ");
  Serial.println(_voltage.getVinVoltage());

  Serial.print("Current duty(%) = ");
  Serial.println(_voltage.getDutyCyclePercent() * 100.0);

  if (_current_profile != nullptr) {
    Serial.print("Current target(C) = ");
    Serial.println(_current_profile->targetTemperature());
  } else {
    Serial.println("No current target");
  }
}