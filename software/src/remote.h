#ifndef __REMOTE_H__
#define __REMOTE_H__

#include "Controller.h"
#include "Logger.h"
#include "Thermocouple.h"
#include "Voltage.h"
#include <Arduino.h>
#include <MQTTRemote.h>

/**
 * Handles all remote/external communication.
 * In this case, responsible for communicating with MQTT server for
 * interating into Home Assistant (https://www.home-assistant.io/)
 */
class Remote {
public:
  /**
   * @brief Create new Remote instance for handling MQTT (Home Assistant) integration, read/write.
   *
   * @param controller the [Controller] instance to use for starting/stopping/status of reflow.
   * @param thermocouple the [Thermocouple] instance to use for reading current temperatures.
   * @param voltage the [Voltage] instance to use for reading current duty cycle.
   * @param logger this [Remote] class subscribes to and forward log messages to MQTT.
   * @param host MQTT hostname.
   * @param usenrame MQTT username.
   * @param password MQTT password.
   */
  Remote(Controller &controller, Thermocouple &thermocouple, Voltage &voltage, Logger &logger, String host,
         String username, String password);

  /**
   * Call from the parent setup() function.
   */
  void setup();
  /**
   * @brief Call from the parent handle() function.
   */
  void handle();

private:
  void publishHASetup();

private:
  Logger &_logger;
  Voltage &_voltage;
  Controller &_controller;
  Thermocouple &_thermocouple;
  unsigned long _last_publish_ms = 0;
  unsigned long _last_publish_ha_setup_ms = 0;

  String _host;
  String _username;
  String _password;

  MQTTRemote _mqtt;
  bool _was_connected = false;
  unsigned long _retry_to_connect_at_ms = 0;
};

#endif //__REMOTE_H__