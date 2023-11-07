#ifndef __STATUS_LEDS_H__
#define __STATUS_LEDS_H__

#include <Arduino.h>

/**
 * Control status leds.
 */
class StatusLeds {
public:
  /**
   * @brief Construct a new Status Leds instance.
   *
   * @param led_red_pin pin where red LED is connected. HIGH is on.
   * @param led_green_pin pin where green LED is connected. HIGH is on.
   */
  StatusLeds(uint8_t led_red_pin, uint8_t led_green_pin);

  /**
   * @brief Call from the parent setup() function.
   */
  void setup();
  /**
   * @brief Call from the parent handle() function.
   */
  void handle();

  /**
   * @brief Set the Red led to on.
   */
  void setRed(bool on);
  /**
   * @brief Set the Green led to on.
   */
  void setGreen(bool on);

private:
  const uint8_t _led_red_pin;
  const uint8_t _led_green_pin;
};

#endif //__STATUS_LEDS_H__