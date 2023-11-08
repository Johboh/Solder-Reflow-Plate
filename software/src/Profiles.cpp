#include "Profiles.h"
#include "ProfileBuilder.h"
#include "ProfileStepBuilder.h"

Profiles::Profiles(Logger &logger) : _logger(logger) {
  Profile p0(ProfileBuilder("Sn42Bi58 80x60mm")
                 .addStep(ProfileStepBuilder(Profile::State::Preheat)
                              .setRampUpTime(60000)
                              .setAggressiveness(14)
                              .setTotalRuntime(80000)
                              .setTargetTemperatureC(108)
                              .setMaxDutyCyclePercent(0.5)
                              .build())
                 .addStep(ProfileStepBuilder(Profile::State::Soak)
                              .setRampUpTime(40000)
                              .setAggressiveness(8)
                              .setTotalRuntime(80000)
                              .setTargetTemperatureC(140)
                              .setMaxDutyCyclePercent(0.6)
                              .build())
                 .addStep(ProfileStepBuilder(Profile::State::Reflow)
                              .setRampUpTime(30000)
                              .setAggressiveness(2)
                              .setTotalRuntime(60000)
                              .setTargetTemperatureC(175)
                              .setMaxDutyCyclePercent(0.85)
                              .build())
                 .build());

  Profile p1(ProfileBuilder("Sn42Bi58 95x95mm")
                 .addStep(ProfileStepBuilder(Profile::State::Preheat)
                              .setRampUpTime(60000)
                              .setAggressiveness(12)
                              .setTotalRuntime(80000)
                              .setTargetTemperatureC(108)
                              .setMaxDutyCyclePercent(0.85)
                              .build())
                 .addStep(ProfileStepBuilder(Profile::State::Soak)
                              .setRampUpTime(40000)
                              .setAggressiveness(12)
                              .setTotalRuntime(80000)
                              .setTargetTemperatureC(140)
                              .setMaxDutyCyclePercent(0.9)
                              .build())
                 .addStep(ProfileStepBuilder(Profile::State::Reflow)
                              .setRampUpTime(20000)
                              .setAggressiveness(10)
                              .setTotalRuntime(40000)
                              .setTargetTemperatureC(170)
                              .setMaxDutyCyclePercent(1.0)
                              .build())
                 .build());
  _profiles.push_back(p0);
  _profiles.push_back(p1);
}

std::optional<Profile> Profiles::getProfile(String &name) {
  auto is_profile = [&](Profile profile) { return profile.name == name; };
  if (auto it = std::find_if(begin(_profiles), end(_profiles), is_profile); it != std::end(_profiles)) {
    return *it;
  } else {
    _logger.log(Logger::Severity::Error,
                String("Profile \"" + name +
                       "\" could not be found in list of profiles. Number of profiles: " + String(_profiles.size()))
                    .c_str());
    return std::nullopt;
  }
}

std::vector<String> Profiles::availableProfiles() const {
  std::vector<String> profile_names;

  for (const auto &profile : _profiles) {
    profile_names.push_back(profile.name);
  }

  return profile_names;
}