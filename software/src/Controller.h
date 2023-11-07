#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "Heater.h"
#include "Logger.h"
#include "Profile.h"
#include "Profiles.h"
#include "StatusLeds.h"
#include "Thermocouple.h"
#include "Voltage.h"

/**
 * Main controller for this board and heatbead.
 * Use the other classes for reading temperature, voltage and controlling voltage.
 * This is where all the buisness logic is (and in Heater class).
 *
 * The Controller will take care of calling setup/handle on child classes.
 */
class Controller {
public:
  /**
   * @brief Construct a new Controller instance.
   *
   * @param voltage the [Voltage] instance to use.
   * @param status_leds the [StatusLeds] instance to use.
   * @param thermocouple the [Thermocouple] instance to use.
   * @param logger the [Logger] instance to use.
   */
  Controller(Voltage &voltage, StatusLeds &status_leds, Thermocouple &thermocouple, Logger &logger);

  /**
   * @brief Call from the parent setup() function.
   */
  void setup();
  /**
   * @brief Call from the parent handle() function.
   */
  void handle();

  enum class State {
    NoProfileSelected,
    Idle,
    Preheat,
    Soak,
    Reflow,
    Error,
  };

  /**
   * @brief Select the profile to use, given its name. Will return false if the profile cannot be found.
   */
  bool selectProfile(String &profile_name);

  /**
   * @brief Get the selected proifle, or null if no proifle is seleted. Call [selectProfile] to select a profile.
   */
  Profile *getSelectedProfile() { return _current_profile; }

  /**
   * @brief Start the selected heating profile. Will fail and return false if no profile has been selected. Use
   * [selectProfile] to select a proifle.
   */
  bool start();

  /**
   * @brief Stop heating/cooldown.
   */
  void stop();

  /**
   * @brief Get the current state.
   */
  State getCurrentState() { return _current_state; }

  /**
   * @brief returns true if we are in some kind of heating state.
   */
  bool inRunningState();

  /**
   * @brief Get the current target temperature, specified by the profile. Returns 0 if there is no profile or if we are
   * not running.
   */
  uint16_t getTargetTemperature();

  /**
   * @brief Print debug information/status on Serial.
   */
  void printDebug();

private:
  Logger &_logger;
  Voltage &_voltage;
  StatusLeds &_status_leds;
  Thermocouple &_thermocouple;
  Heater _heater;

private:
  Profile *_current_profile;
  Profiles _profiles;
  State _current_state = State::NoProfileSelected;
};

#endif //__CONTROLLER_H__
