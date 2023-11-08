#ifndef __PROFILE_H__
#define __PROFILE_H__

#include <Arduino.h>
#include <vector>

/**
 * A single solder profile.
 *
 * Source of profiles: https://www.compuphase.com/electronics/reflowsolderprofiles.htm
 */
struct Profile {
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

  String name;
  std::vector<Step> steps;

  bool operator<(const Profile &other) const { return name < other.name; }
};

#endif //__PROFILE_H__
