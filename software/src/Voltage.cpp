#include "Voltage.h"
#include "driver/ledc.h"
#include "esp_err.h"

#define FULL_ON_DUTY_CYCLE 1023.0f
#define ABSOLUTE_MAX_PWM_DUTY 1000.0f
#define READ_VIN_EVERY_MS 200
#define GATE_CHANNEL LEDC_CHANNEL_0
#define GATE_FREQUENCY 32768
#define VIN_FACTOR 0.00184842105263157894736842105263
#define MIN_VIN_VOLTAGE 9

Voltage::Voltage(uint8_t vin_pin, uint8_t gate_pin) : _vin_pin(vin_pin), _gate_pin(gate_pin), _current_duty_cycle(0) {}

void Voltage::setup() {
  ledc_timer_config_t ledc_timer = {
      .speed_mode = LEDC_LOW_SPEED_MODE,    // timer mode
      .duty_resolution = LEDC_TIMER_10_BIT, // resolution of PWM duty
      .timer_num = LEDC_TIMER_0,            // timer index
      .freq_hz = GATE_FREQUENCY,            // frequency of PWM signal
      .clk_cfg = LEDC_AUTO_CLK,             // Auto select the source clock
  };
  ledc_timer_config(&ledc_timer);

  ledc_channel_config_t ledc_channel = {.gpio_num = _gate_pin,
                                        .speed_mode = LEDC_LOW_SPEED_MODE,
                                        .channel = GATE_CHANNEL,
                                        .intr_type = LEDC_INTR_DISABLE,
                                        .timer_sel = LEDC_TIMER_0,
                                        .duty = 0,
                                        .hpoint = 0};
  ledc_channel.flags.output_invert = 0;
  ledc_channel_config(&ledc_channel);

  setDutyCyclePercent(0.0);
}

void Voltage::handle() {
  if (millis() - _last_vin_read_timestamp > READ_VIN_EVERY_MS) {
    update();
  }
}

float Voltage::getDutyCyclePercent() { return (float)_current_duty_cycle / FULL_ON_DUTY_CYCLE; }

void Voltage::setDutyCyclePercent(float duty_cycle_percent) {
  float duty_cycle = duty_cycle_percent * FULL_ON_DUTY_CYCLE;
  uint16_t max_allowed_duty = (uint16_t)min(duty_cycle, ABSOLUTE_MAX_PWM_DUTY);

  updatePwm(max_allowed_duty);
  delay(10);

  // Adjust to nearest safe VIN level.
  update();
  float vin = getVinVoltage();
  while (vin < MIN_VIN_VOLTAGE && max_allowed_duty > 0) {
    --max_allowed_duty;

    updatePwm(max_allowed_duty);
    delay(10);

    vin = getVinVoltage();
    update();
  }
  _current_duty_cycle = max_allowed_duty;
}

void Voltage::updatePwm(uint16_t duty_cycle) {
  ledc_set_duty(LEDC_LOW_SPEED_MODE, GATE_CHANNEL, duty_cycle);
  ledc_update_duty(LEDC_LOW_SPEED_MODE, GATE_CHANNEL);
}

void Voltage::readAdc() {
  unsigned long val = 0;
  // Some average value.
  for (uint8_t i = 0; i < 5; ++i) {
    val += analogRead(_vin_pin);
    delayMicroseconds(100);
  }
  _vin_voltage = (val / 5) * VIN_FACTOR;
}

void Voltage::update() {
  readAdc();
  _last_vin_read_timestamp = millis();
}