#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include <zephyr.h>

void initialize_peripherals(void);
void perform_i2c_scan(void);

void test_tca_chip(void);
uint8_t tca_set_power(uint8_t channel, char *state);
uint8_t tca_set_relay(uint8_t target, char* relay, char *state);
uint8_t tca_set_led(uint8_t target, char *state, char* color);
uint8_t tca_detect_device(uint8_t channel);
void init_tca_blink_work();

uint8_t gpio_reset(uint8_t channel);
uint8_t gpio_toggle_uart(void);

void test_adc_chip(void);
uint16_t adc_read_voltage(uint8_t target, uint8_t channel);
void adc_set_gain(uint8_t target, uint16_t gain);
uint16_t adc_get_gain(uint8_t target);

void test_max_chip(void);
uint8_t max_set_jtag(int8_t channel);

#endif  // _COMMUNICATION_H_
