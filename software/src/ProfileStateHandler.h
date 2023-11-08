#ifndef __PROFILE_STATE_HANDLER_H__
#define __PROFILE_STATE_HANDLER_H__

#include "Profile.h"
#include <Arduino.h>

class ProfileStateHandler {
public:
  ProfileStateHandler();

public:
  /**
   * @brief Given a state, return the step that has that state. Will return nullptr if no step could be found.
   */
  std::optional<Profile::Step> getStep(Profile::State state);

  /**
   * @brief Resets this profile. Will stop it from reporting any target temperature (>0).
   */
  void reset();

  /**
   * @brief Set the Profile to use.
   * Will reset on call.
   */
  void setProfile(Profile profile);

  /**
   * @brief Start this profile, given the current (idle) temperature. After this, continuous call [targetTemperature].
   */
  void start(float idle_temperature);

  /**
   * @brief Returns the current temperature the heat bed should be set to (i.e. the target). Must call [start()] before
   * first call.
   */
  uint16_t targetTemperature();

  /**
   * @brief Get the Max allowed duty cycle.
   *
   * @return float percent between 0.0 and 1.0, where 1.0 is 100% (full duty). Must call [start()] before first call,
   * otherwise will return NAN.
   */
  float getMaxDutyCycle();

  /**
   * @brief See Profile definition for description.
   *
   * @return uint8_t aggressiveness. 255 if no valid profile.  Must call [start()] before first call.
   */
  uint8_t getAggressiveness();

  /**
   * @brief Get the current state this profile is in.
   */
  Profile::State getCurrentState() { return _current_state; }

private:
  void calculateKM(float zero_time_temperature);

private:
  Profile _profile;

private:
  Profile::State _current_state = Profile::State::Idle;
  unsigned long _start_time_ms = 0;
  unsigned long _step_start_time_ms = 0;

  double _k = 0;
  double _m = 0;
};

#endif // __PROFILE_STATE_HANDLER_H__
