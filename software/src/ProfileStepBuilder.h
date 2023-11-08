#ifndef __PROFILE_STEP_BUILDER_H__
#define __PROFILE_STEP_BUILDER_H__

#include "Profile.h"

class ProfileStepBuilder {
public:
  ProfileStepBuilder(Profile::State state) : _step({state, 0, 0, 0, 0.0, 0}) {}

  ProfileStepBuilder &setTargetTemperatureC(uint16_t target_temperature_c) {
    _step.target_temperature_c = target_temperature_c;
    return *this;
  }

  ProfileStepBuilder &setRampUpTime(unsigned long ramp_up_ms) {
    _step.ramp_up_ms = ramp_up_ms;
    return *this;
  }

  ProfileStepBuilder &setTotalRuntime(unsigned long total_runtime_ms) {
    _step.total_runtime_ms = total_runtime_ms;
    return *this;
  }

  ProfileStepBuilder &setMaxDutyCyclePercent(float max_duty_cycle_percent) {
    _step.max_duty_cycle_percent = max_duty_cycle_percent;
    return *this;
  }

  ProfileStepBuilder &setAggressiveness(uint8_t aggressiveness) {
    _step.aggressiveness = aggressiveness;
    return *this;
  }

  Profile::Step build() const { return _step; }

private:
  Profile::Step _step;
};

#endif // __PROFILE_STEP_BUILDER_H__