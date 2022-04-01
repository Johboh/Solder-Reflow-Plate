#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "Logger.h"
#include "Profile.h"
#include "Profiles.h"
#include "StatusLeds.h"
#include "Thermocouple.h"
#include "Voltage.h"

/**
 * Main controller for this board and heatbead.
 * Use the other classes for reading temperature, voltage and controlling voltage.
 * This is where all the buisness logic is.
 *
 * The Controller will take care of calling setup/handle on child classes.
 */
class Controller {
public:
  Controller(Voltage &voltage, StatusLeds &status_leds, Thermocouple &thermocouple, Logger &logger);

  void setup();
  void handle();

  enum class State {
    NoProfileSelected,
    Idle,
    Preheat,
    Soak,
    Reflow,
    Cooling,
    Error,
  };

  bool selectProfile(String &profile_name);
  Profile *getSelectedProfile() { return _current_profile; }
  bool start();
  void stop();
  State getCurrentState() { return _current_state; }
  bool inRunningState();

  // Debug and temporary functions
  void handleSerialInput();
  void printDebug();

private:
  Logger &_logger;
  Voltage &_voltage;
  StatusLeds &_status_leds;
  Thermocouple &_thermocouple;

private:
  Profile *_current_profile;
  Profiles _profiles;
  unsigned long _last_duty_change_timestamp = 0;
  State _current_state = State::NoProfileSelected;
};

#endif //__CONTROLLER_H__