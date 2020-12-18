// TODO wrapper functions for drivers
#include "communication.h"

#include <zephyr.h>
#include <logging/log.h>

#include <tca_interface.hpp>
#include <i2c_functions.h>

LOG_MODULE_REGISTER(communication);

void initialize_peripherals(void) 
{
    i2c_init();
}

void perform_i2c_scan(void) 
{
    i2c_scan();
}

void test_tca_chip(void) 
{
    i2c_init();
    int err = test_connection();
    if (err) 
    {   
        LOG_ERR("TCA connection test failed!");
    }
}

/*
int res = read_reg(0x22, 0x04);
write_reg(0x22, 0x04, 0x01);
res = read_reg(0x22, 0x04);
*/

int tca_set_power(int channel, char *state)
{
    // TODO
    return 0;
}
