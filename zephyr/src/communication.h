#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include <zephyr.h>

void initialize_peripherals(void);
void perform_i2c_scan(void);
void test_tca_chip(void);
uint8_t tca_set_power(uint8_t channel, char *state);

uint8_t testing_tca(void);

#endif  // _COMMUNICATION_H_
