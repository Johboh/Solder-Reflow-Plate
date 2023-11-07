#include "Profiles.h"

Profile::Step sn42bi58_95x_preheat = {
    .state = Profile::State::Preheat,
    .target_temperature_c = 108,
    .ramp_up_ms = 60000,
    .total_runtime_ms = 80000,
    .max_duty_cycle_percent = 0.85,
    .aggressiveness = 12,
};

Profile::Step sn42bi58_95x_soak = {
    .state = Profile::State::Soak,
    .target_temperature_c = 140,
    .ramp_up_ms = 40000,
    .total_runtime_ms = 80000,
    .max_duty_cycle_percent = 0.90,
    .aggressiveness = 12,
};

Profile::Step sn42bi58_95x_reflow = {
    .state = Profile::State::Reflow,
    .target_temperature_c = 170,
    .ramp_up_ms = 20000,
    .total_runtime_ms = 40000,
    .max_duty_cycle_percent = 1.0,
    .aggressiveness = 10,
};

String sn42bi58_95x_name = "Sn42Bi58 95x95mm";
Profile::Step sn42bi58_95x_steps[] = {sn42bi58_95x_preheat, sn42bi58_95x_soak, sn42bi58_95x_reflow};

Profile::Step sn42bi58_80x_preheat = {
    .state = Profile::State::Preheat,
    .target_temperature_c = 108,
    .ramp_up_ms = 60000,
    .total_runtime_ms = 80000,
    .max_duty_cycle_percent = 0.50,
    .aggressiveness = 14,
};

Profile::Step sn42bi58_80x_soak = {
    .state = Profile::State::Soak,
    .target_temperature_c = 140,
    .ramp_up_ms = 40000,
    .total_runtime_ms = 80000,
    .max_duty_cycle_percent = 0.6,
    .aggressiveness = 8,
};

Profile::Step sn42bi58_80x_reflow = {
    .state = Profile::State::Reflow,
    .target_temperature_c = 175,
    .ramp_up_ms = 30000,
    .total_runtime_ms = 60000,
    .max_duty_cycle_percent = 0.85,
    .aggressiveness = 2,
};

String sn42bi58_80x_name = "Sn42Bi58 80x60mm";
Profile::Step sn42bi58_80x_steps[] = {sn42bi58_80x_preheat, sn42bi58_80x_soak, sn42bi58_80x_reflow};

Profiles::Profiles(Logger &logger)
    : _logger(logger),
      _profiles({Profile(sn42bi58_95x_name, sn42bi58_95x_steps), Profile(sn42bi58_80x_name, sn42bi58_80x_steps)}) {}

Profile *Profiles::getProfile(String &name) {
  uint8_t number_of_profiles = getNumberOfProfiles();
  _logger.log(Logger::Severity::Info, String("Number of profiles available: " + String(number_of_profiles)).c_str());
  for (uint8_t i = 0; i < number_of_profiles; ++i) {
    Profile *profile = &_profiles[i];
    _logger.log(Logger::Severity::Info,
                String("Profile at position " + String(i) + " is <" + profile->getName() + ">").c_str());
    if (name == profile->getName()) {
      _logger.log(Logger::Severity::Info, String("Found profile <" + String(name) + ">").c_str());
      return profile;
    }
  }
  _logger.log(Logger::Severity::Error, "Profile could not be found in list of profiles.");
  return nullptr;
}
