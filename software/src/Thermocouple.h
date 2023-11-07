#ifndef __THERMOCOUPLE_H__
#define __THERMOCOUPLE_H__

#include "Adafruit_MAX31855.h"
#include <Arduino.h>

/**
 * Read Thermocouple for internal and bed temperature.
 */
class Thermocouple {
public:
  /**
   * @brief Construct a new Thermocouple instance.
   *
   * @param cs_pin chip select pin of MAX31855
   * @param sck_pin clock pin of MAX31855
   * @param so_pin Data out pin of MAX31855 (that is, master input pin of host controller)
   */
  Thermocouple(uint8_t cs_pin, uint8_t sck_pin, uint8_t so_pin);

  /**
   * @brief Call from the parent setup() function.
   */
  void setup();
  /**
   * @brief Call from the parent handle() function.
   */
  void handle();

  /**
   * @brief Force update temperatures. This will block until done.
   */
  void update();

  /**
   * @brief Get current internal/ambient temperature, i.e. the temperature on the main board itself.
   */
  double getAmbientTemperature() { return _ambient_temperature; }
  /**
   * @brief Get the bed temperature. Must check that value is not NAN before using (!isnan(value))
   */
  double getBedTemperature() { return _bed_temperature; }

private:
  Adafruit_MAX31855 _max31855;
  unsigned long _last_read_timestamp = 0;
  double _ambient_temperature = NAN;
  double _bed_temperature = NAN;
};

#endif //__THERMOCOUPLE_H__
