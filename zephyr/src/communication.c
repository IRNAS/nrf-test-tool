// TODO wrapper functions for drivers
#include "communication.h"

#include <stdlib.h>
#include <logging/log.h>

#include <tca_interface.hpp>
#include <i2c_functions.h>

LOG_MODULE_REGISTER(communication);

void initialize_peripherals(void) 
{
    i2c_init();
    tca_init();
}

void perform_i2c_scan(void) 
{
    i2c_scan();
}

void test_tca_chip(void) 
{
    uint8_t result = test_connection();
    LOG_INF("Connection test result: %x", result);

    for (int i = 0; i < 15; i++) 
    {
        if (i == 3 || i == 7 || i == 11) 
        {
            continue;
        }
        int res = read_bank(i);
        LOG_INF("%d result: %x", i, res);
        k_msleep(10);
    }
}

uint8_t tca_set_power(uint8_t target, char *state)
{
    int res;
    uint8_t pin = target * 2;   // each target has two pins
    uint8_t new_pin_state_pro;
    uint8_t new_pin_state_ldo;

    if (strcmp("off", state) == 0)  // turn off both pins on current target
    {
      new_pin_state_pro = 0;
      new_pin_state_ldo = 0;
    }
    else if (strcmp("on", state) == 0)   // turn on power from nrf91 (LDO)
    {   
        new_pin_state_pro = 1;
        new_pin_state_ldo = 0;
    }
    else    // state on, turn on power from ppk (PRO)
    {
        new_pin_state_pro = 0;
        new_pin_state_ldo = 1;
    }

    res = read_pin(pin);
    LOG_INF("pin %d state: %d", pin, res);
    write_pin(pin, new_pin_state_pro);
    res = read_pin(pin);
    LOG_INF("pin %d state: %d", pin, res);

    pin++;
    res = read_pin(pin);
    LOG_INF("pin %d state: %d", pin, res);
    write_pin(pin, new_pin_state_ldo);
    res = read_pin(pin);
    LOG_INF("pin %d state: %d", pin, res);

    return 0;
}
