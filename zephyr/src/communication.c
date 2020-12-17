// TODO wrapper functions for drivers
#include "communication.h"

#include <zephyr.h>
#include <logging/log.h>

#include <tca_interface.hpp>
#include <i2c_functions.h>

LOG_MODULE_REGISTER(communication);

void i2c_testing(void) 
{
    i2c_init();
    i2c_scan();
}

void tca_testing(void) 
{
    int res = read_reg(0x49, 0x00);
    /*
    int err = test_connection();
    if (err) 
    {
        LOG_ERR("TCA connection test failed!");
    }
    */
}