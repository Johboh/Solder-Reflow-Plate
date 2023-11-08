#include "remote.h"

#define FORCE_PUBLISH_INTERVAL_MS 5000
#define FORCE_PUBLISH_INTERVAL_WHEN_RUNNING_MS 750
#define PUBLISH_SETUP_EVERY_MS (60000 * 30) // 30 min

// TODO (johboh): Replace all of this with https://github.com/Johboh/HomeAssistantEntities library.

const char *HOME_ASSISTANT_INTERNAL_TEMPERATURE_CONFIG PROGMEM = "{ \
          \"unit_of_measurement\": \"°C\", \
          \"icon\": \"mdi:thermometer\", \
          \"name\": \"Solder reflow plate internal temperature\", \
          \"state_topic\": \"solder_reflow_plate/sensor/solder_reflow_plate_internal_temperature/state\", \
          \"availability_topic\": \"solder_reflow_plate/status\", \
          \"unique_id\": \"SolderReflowPlate_InternalTemperature\", \
          \"device\": { \
          \"identifiers\": \"58:cf:79:a4:ee:cc\", \
          \"name\": \"solder_reflow_plate\", \
          \"sw_version\": \"VSC, ElegantOTA 1.0\", \
          \"model\": \"esp32-s2\", \
          \"manufacturer\": \"espressif\" \
          } \
          }";

const char *HOME_ASSISTANT_BED_TEMPERATURE_CONFIG PROGMEM = "{ \
          \"unit_of_measurement\": \"°C\", \
          \"icon\": \"mdi:thermometer\", \
          \"name\": \"Solder reflow plate bed temperature\", \
          \"state_topic\": \"solder_reflow_plate/sensor/solder_reflow_plate_bed_temperature/state\", \
          \"availability_topic\": \"solder_reflow_plate/status\", \
          \"unique_id\": \"SolderReflowPlate_BedTemperature\", \
          \"device\": { \
          \"identifiers\": \"58:cf:79:a4:ee:cc\", \
          \"name\": \"solder_reflow_plate\", \
          \"sw_version\": \"VSC, ElegantOTA 1.0\", \
          \"model\": \"esp32-s2\", \
          \"manufacturer\": \"espressif\" \
          } \
          }";

const char *HOME_ASSISTANT_TARGET_TEMPERATURE_CONFIG PROGMEM = "{ \
          \"unit_of_measurement\": \"°C\", \
          \"icon\": \"mdi:thermometer\", \
          \"name\": \"Solder reflow plate target temperature\", \
          \"state_topic\": \"solder_reflow_plate/sensor/solder_reflow_plate_target_temperature/state\", \
          \"availability_topic\": \"solder_reflow_plate/status\", \
          \"unique_id\": \"SolderReflowPlate_TargetTemperature\", \
          \"device\": { \
          \"identifiers\": \"58:cf:79:a4:ee:cc\", \
          \"name\": \"solder_reflow_plate\", \
          \"sw_version\": \"VSC, ElegantOTA 1.0\", \
          \"model\": \"esp32-s2\", \
          \"manufacturer\": \"espressif\" \
          } \
          }";

const char *HOME_ASSISTANT_VOLTAGE_IN_CONFIG PROGMEM = "{ \
          \"unit_of_measurement\": \"V\", \
          \"icon\": \"mdi:power-plug\", \
          \"name\": \"Solder reflow plate voltage in\", \
          \"state_topic\": \"solder_reflow_plate/sensor/solder_reflow_plate_voltage_in/state\", \
          \"availability_topic\": \"solder_reflow_plate/status\", \
          \"unique_id\": \"SolderReflowPlate_VoltageIn\", \
          \"device\": { \
          \"identifiers\": \"58:cf:79:a4:ee:cc\", \
          \"name\": \"solder_reflow_plate\", \
          \"sw_version\": \"VSC, ElegantOTA 1.0\", \
          \"model\": \"esp32-s2\", \
          \"manufacturer\": \"espressif\" \
          } \
          }";

const char *HOME_ASSISTANT_DUTY_CYCLE_CONFIG PROGMEM = "{ \
          \"unit_of_measurement\": \"%\", \
          \"icon\": \"mdi:heating-coil\", \
          \"name\": \"Solder reflow plate duty cycle\", \
          \"state_topic\": \"solder_reflow_plate/sensor/solder_reflow_plate_duty_cycle/state\", \
          \"availability_topic\": \"solder_reflow_plate/status\", \
          \"unique_id\": \"SolderReflowPlate_DutyCycle\", \
          \"device\": { \
          \"identifiers\": \"58:cf:79:a4:ee:cc\", \
          \"name\": \"solder_reflow_plate\", \
          \"sw_version\": \"VSC, ElegantOTA 1.0\", \
          \"model\": \"esp32-s2\", \
          \"manufacturer\": \"espressif\" \
          } \
          }";

// TODO (johboh): Get available profiles from Profiles.h
const char *HOME_ASSISTANT_PROFILE_CONFIG PROGMEM = "{ \
          \"icon\": \"mdi:book-open-page-variant-outline\", \
          \"name\": \"Solder reflow plate profile\", \
          \"options\": [\"Sn42Bi58 80x60mm\",\"Sn42Bi58 95x95mm\"], \
          \"state_topic\": \"solder_reflow_plate/select/solder_reflow_plate_profile/state\", \
          \"command_topic\": \"solder_reflow_plate/select/solder_reflow_plate_profile/command\", \
          \"availability_topic\": \"solder_reflow_plate/status\", \
          \"unique_id\": \"SolderReflowPlate_Profile\", \
          \"retain\": true, \
          \"device\": { \
          \"identifiers\": \"58:cf:79:a4:ee:cc\", \
          \"name\": \"solder_reflow_plate\", \
          \"sw_version\": \"VSC, ElegantOTA 1.0\", \
          \"model\": \"esp32-s2\", \
          \"manufacturer\": \"espressif\" \
          } \
          }";

const char *HOME_ASSISTANT_STATE_CONFIG PROGMEM = "{ \
          \"icon\": \"mdi:progress-clock\", \
          \"name\": \"Solder reflow plate state\", \
          \"state_topic\": \"solder_reflow_plate/sensor/solder_reflow_plate_state/state\", \
          \"availability_topic\": \"solder_reflow_plate/status\", \
          \"unique_id\": \"SolderReflowPlate_State\", \
          \"device\": { \
          \"identifiers\": \"58:cf:79:a4:ee:cc\", \
          \"name\": \"solder_reflow_plate\", \
          \"sw_version\": \"VSC, ElegantOTA 1.0\", \
          \"model\": \"esp32-s2\", \
          \"manufacturer\": \"espressif\" \
          } \
          }";

const char *HOME_ASSISTANT_START_CONFIG PROGMEM = "{ \
          \"icon\": \"mdi:play-circle\", \
          \"name\": \"Solder reflow plate start\", \
          \"command_topic\": \"solder_reflow_plate/button/solder_reflow_plate_start/command\", \
          \"availability_topic\": \"solder_reflow_plate/status\", \
          \"unique_id\": \"SolderReflowPlate_Start\", \
          \"retain\": false, \
          \"device\": { \
          \"identifiers\": \"58:cf:79:a4:ee:cc\", \
          \"name\": \"solder_reflow_plate\", \
          \"sw_version\": \"VSC, ElegantOTA 1.0\", \
          \"model\": \"esp32-s2\", \
          \"manufacturer\": \"espressif\" \
          } \
          }";

const char *HOME_ASSISTANT_STOP_CONFIG PROGMEM = "{ \
          \"icon\": \"mdi:stop-circle\", \
          \"name\": \"Solder reflow plate stop\", \
          \"command_topic\": \"solder_reflow_plate/button/solder_reflow_plate_stop/command\", \
          \"availability_topic\": \"solder_reflow_plate/status\", \
          \"unique_id\": \"SolderReflowPlate_Stop\", \
          \"retain\": false, \
          \"device\": { \
          \"identifiers\": \"58:cf:79:a4:ee:cc\", \
          \"name\": \"solder_reflow_plate\", \
          \"sw_version\": \"VSC, ElegantOTA 1.0\", \
          \"model\": \"esp32-s2\", \
          \"manufacturer\": \"espressif\" \
          } \
          }";

Remote::Remote(Controller &controller, Thermocouple &thermocouple, Voltage &voltage, Logger &logger, String host,
               String username, String password)
    : _logger(logger), _voltage(voltage), _controller(controller), _thermocouple(thermocouple),
      _mqtt("solder_reflow_plate", host, 1883, username, password), _last_publish_ms(0), _host(host),
      _username(username), _password(password) {}

void Remote::setup() {
  _logger.setCallback([this](Logger::Severity severity, const char message[]) {
    switch (severity) {
    case Logger::Severity::Info:
      _mqtt.publishMessage("solder_reflow_plate/debug/info", message);
      break;
    case Logger::Severity::Warning:
      _mqtt.publishMessage("solder_reflow_plate/debug/waring", message);
      break;
    case Logger::Severity::Error:
      _mqtt.publishMessage("solder_reflow_plate/debug/error", message);
      break;
    }
  });
}

void Remote::publishHASetup() {
  _mqtt.publishMessageVerbose(
      "homeassistant/sensor/solder_reflow_plate/solder_reflow_plate_internal_temperature/config",
      HOME_ASSISTANT_INTERNAL_TEMPERATURE_CONFIG, true);

  _mqtt.publishMessageVerbose("homeassistant/sensor/solder_reflow_plate/solder_reflow_plate_bed_temperature/config",
                              HOME_ASSISTANT_BED_TEMPERATURE_CONFIG, true);

  _mqtt.publishMessageVerbose("homeassistant/sensor/solder_reflow_plate/solder_reflow_plate_target_temperature/config",
                              HOME_ASSISTANT_TARGET_TEMPERATURE_CONFIG, true);

  _mqtt.publishMessageVerbose("homeassistant/sensor/solder_reflow_plate/solder_reflow_plate_voltage_in/config",
                              HOME_ASSISTANT_VOLTAGE_IN_CONFIG, true);

  _mqtt.publishMessageVerbose("homeassistant/sensor/solder_reflow_plate/solder_reflow_plate_duty_cycle/config",
                              HOME_ASSISTANT_DUTY_CYCLE_CONFIG, true);

  _mqtt.publishMessageVerbose("homeassistant/sensor/solder_reflow_plate/solder_reflow_plate_state/config",
                              HOME_ASSISTANT_STATE_CONFIG, true);

  _mqtt.publishMessageVerbose("homeassistant/select/solder_reflow_plate/solder_reflow_plate_profile/config",
                              HOME_ASSISTANT_PROFILE_CONFIG, true);
  _mqtt.subscribe("solder_reflow_plate/select/solder_reflow_plate_profile/command",
                  [&](const char *topic, const char *message) {
                    String str(message);
                    _controller.selectProfile(str);
                  });

  _mqtt.publishMessageVerbose("homeassistant/button/solder_reflow_plate/solder_reflow_plate_start/config",
                              HOME_ASSISTANT_START_CONFIG, true);
  _mqtt.subscribe("solder_reflow_plate/button/solder_reflow_plate_start/command",
                  [&](const char *topic, const char *message) {
                    String str_message = String(message);
                    if (str_message == "PRESS") {
                      _controller.start();
                    }
                  });

  _mqtt.publishMessageVerbose("homeassistant/button/solder_reflow_plate/solder_reflow_plate_stop/config",
                              HOME_ASSISTANT_STOP_CONFIG, true);
  _mqtt.subscribe("solder_reflow_plate/button/solder_reflow_plate_stop/command",
                  [&](const char *topic, const char *message) {
                    String str_message = String(message);
                    if (str_message == "PRESS") {
                      _controller.stop();
                    }
                  });
}

void Remote::handle() {
  _mqtt.handle();

  if (!_mqtt.connected()) {
    _was_connected = false;
    return; // EARLY RETURN
  }

  if (!_was_connected) {
    publishHASetup();
    _was_connected = true;
  }

  unsigned long now = millis();
  unsigned long interval =
      _controller.inRunningState() ? FORCE_PUBLISH_INTERVAL_WHEN_RUNNING_MS : FORCE_PUBLISH_INTERVAL_MS;
  bool force_publish = now - _last_publish_ms > interval;

  if (force_publish) {
    String strval = String(_thermocouple.getAmbientTemperature());
    _mqtt.publishMessage("solder_reflow_plate/sensor/solder_reflow_plate_internal_temperature/state", strval);

    auto bed_temperature = _thermocouple.getBedTemperature();
    if (!isnan(bed_temperature)) {
      strval = String(bed_temperature);
      _mqtt.publishMessage("solder_reflow_plate/sensor/solder_reflow_plate_bed_temperature/state", strval);
    }

    strval = String(_controller.getTargetTemperature());
    _mqtt.publishMessage("solder_reflow_plate/sensor/solder_reflow_plate_target_temperature/state", strval);

    strval = String(_voltage.getVinVoltage());
    _mqtt.publishMessage("solder_reflow_plate/sensor/solder_reflow_plate_voltage_in/state", strval);

    strval = String(_voltage.getDutyCyclePercent() * 100.0);
    _mqtt.publishMessage("solder_reflow_plate/sensor/solder_reflow_plate_duty_cycle/state", strval);

    auto current_profile = _controller.getSelectedProfile();
    if (current_profile) {
      _mqtt.publishMessage("solder_reflow_plate/select/solder_reflow_plate_profile/state", current_profile->name);
    }

    switch (_controller.getCurrentState()) {
    case Controller::State::Idle:
      strval = String("idle");
      break;
    case Controller::State::NoProfileSelected:
      strval = String("no_profile_selected");
      break;
    case Controller::State::Error:
      strval = String("error");
      break;
    case Controller::State::Preheat:
      strval = String("preheating");
      break;
    case Controller::State::Soak:
      strval = String("soaking");
      break;
    case Controller::State::Reflow:
      strval = String("reflowing");
      break;
    default:
      strval = String("unknown");
      break;
    }
    _mqtt.publishMessage("solder_reflow_plate/sensor/solder_reflow_plate_state/state", strval);

    _last_publish_ms = now;
  }

  if (now - _last_publish_ha_setup_ms > PUBLISH_SETUP_EVERY_MS) {
    publishHASetup();
    _last_publish_ha_setup_ms = now;
  }
}