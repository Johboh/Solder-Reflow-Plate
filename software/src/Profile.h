#ifndef __PROFILE_H__
#define __PROFILE_H__

#include <Arduino.h>

#define NUMBER_OF_STEPS 3

/**
 * A single solder profile.
 *
 * Source of profiles: https://www.compuphase.com/electronics/reflowsolderprofiles.htm
 */
class Profile {
public:
  enum class State {
    /**
     * The preheating zone should increase the temperature at a maximum rate of 3 °C/s.
     *  The purpose of preheating is to allow the solvents to evaporate and to activate the flux.
     */
    Preheat,
    /**
     * The soak zone brings the temperature of all components and board areas to an equal level.
     * Due to differences in thermal inertia, components do not heat up at the same speed.
     */
    Soak,
    /**
     * In the reflow zone, the temperature rises at a rate of approximately 2 °C to a
     * temperature above the melting point (the temperature above liquidus, or TAL).
     * The peak temperature for the reflow zone is at least 25 °C above TAL,
     * because solder both coalesces better with the copper and wets the pads and
     * component pins better when it is hotter —thereby creating better joints.
     * This is especially important for lead-free solders (SAC305), because this
     * solder is less effective at wetting than leaded solders.
     */
    Reflow,
    /**
     * No state/idle/Cooling.
     * The cooling zone ideally has a ramp of 4 °C/s (to cool down quickly, but avoid thermal stress).
     * Free air cooling is sufficient.
     */
    Idle,
  };

  struct Step {
    /**
     * What this step represent in the profile state.
     */
    Profile::State state;
    /**
     * Target temperature.
     */
    uint16_t target_temperature_c;
    /**
     * Time to ramp up to temperature, in milliseconds.
     */
    unsigned long ramp_up_ms;
    /**
     * How loing this step should run in total, in milliseconds.
     */
    unsigned long total_runtime_ms;
    /**
     * The maximum allowed duty cycle in percent. Float value between 0.0 and 1.0, where 1.0 is 100% duty (full on).
     * Any value below or above this value will be capped.
     */
    float max_duty_cycle_percent;
    /**
     * How agressive the heating should be the future away the target temperature is in relation to the current
     * temperature. You higher, you less agressive. 1 is the most aggressive. >20 is not agressive at all. Allowed
     * values are 1-100.
     * Nominal value is between 12 and 15.
     */
    uint8_t aggressiveness;
  };

  /**
   * @brief Create a new profile given a name and steps.
   *
   * @param name the name of the profile.
   * @param steps a list of steps for this profile (Preheat, Soak, Reflow)
   */
  Profile(String &name, Profile::Step steps[NUMBER_OF_STEPS]);

public:
  /**
   * @brief Given a state, return the step that has that state. Will return nullptr if no step could be found.
   */
  Profile::Step const *getStep(Profile::State state);

  /**
   * @brief Resets this profile. Will stop it from reporting any target temperature (>0).
   */
  void reset();
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
   * @brief Get the name of this profile.
   */
  String const &getName() { return _name; }
  /**
   * @brief Get the current state this profile is in.
   */
  Profile::State getCurrentState() { return _current_state; }

private:
  void calculateKM(float zero_time_temperature);

private:
  String const _name;
  Profile::Step const *_steps;

private:
  Profile::State _current_state = Profile::State::Idle;
  unsigned long _start_time_ms = 0;
  unsigned long _step_start_time_ms = 0;

  double _k = 0;
  double _m = 0;
};

#endif //__PROFILE_H__
