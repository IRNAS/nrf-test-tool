#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

void initialize_peripherals(void);
void perform_i2c_scan(void);
void test_tca_chip(void);
int tca_set_power(int channel, char *state);

#endif  // _COMMUNICATION_H_
