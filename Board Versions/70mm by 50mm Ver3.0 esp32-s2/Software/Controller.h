#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

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
  Controller(Voltage &voltage, StatusLeds &status_leds, Thermocouple &thermocouple);

  void setup();
  void handle();

  void printDebug();

private:
  Voltage &_voltage;
  StatusLeds &_status_leds;
  Thermocouple &_thermocouple;
};

#endif //__CONTROLLER_H__
