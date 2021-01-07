#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include <zephyr.h>

void initialize_peripherals(void);
void perform_i2c_scan(void);

void test_tca_chip(void);
uint8_t tca_set_power(uint8_t channel, char *state);
uint8_t tca_set_led(uint8_t channel, char *state);

uint8_t gpio_reset(uint8_t channel);

void test_ads_chip(void);
uint16_t adc_read_voltage(uint8_t target, uint8_t channel);

void test_max_chip(void);
uint8_t max_set_jtag(int8_t channel);

#endif  // _COMMUNICATION_H_
