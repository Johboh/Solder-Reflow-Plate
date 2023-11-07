#ifndef __HEATER_H__
#define __HEATER_H__

#include "Thermocouple.h"
#include "Voltage.h"
#include <Arduino.h>

/**
 * Abstraction to control the heat bed.
 *
 * Pid references, not used currently:
 * https://newton.ex.ac.uk/teaching/CDHW/Feedback/Setup-PID.html
 *
 */
class Heater {
public:
  /**
   * @brief Construct a new Heater instance.
   *
   * @param voltage instance of [Voltage] class to use to read voltage in and control duty cycle.
   * @param thermocouple instance of [Thermocouple] class to read temperatures.
   */
  Heater(Voltage &voltage, Thermocouple &thermocouple);

  /**
   * @brief Call from the parent setup() function.
   */
  void setup();
  /**
   * @brief Call from the parent handle() function.
   */
  void handle();

  /**
   * @brief Start heating. After this call, continuous call [requestTemperature] with the desired temperature for the
   * heatbed.
   */
  void start();
  /**
   * @brief Call continuous to request a temperature for the heatbed. Must call [start] first.
   */
  void requestTemperature(float temperature, float max_duty_cycle_percent, uint8_t aggressiveness);
  /**
   * @brief Stop heating. Will cool down/turn off.
   */
  void stop();

private:
  void evaulate();

private:
  Voltage &_voltage;
  Thermocouple &_thermocouple;

private:
  bool _running = false;
  uint8_t _aggressiveness = 0;
  float _target_temperature = 0;
  float _max_duty_cycle_percent = 0;
  unsigned long _last_evalulation_ms = 0;
};

#endif //__HEATER_H__