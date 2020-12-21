// TODO wrapper functions for drivers
#include "communication.h"

#include <stdlib.h>
#include <logging/log.h>

#include <peripherals.h>
#include <tca_interface.hpp>
#include <max_interface.hpp>
#include <gpio.h>

LOG_MODULE_REGISTER(communication);

void initialize_peripherals(void) 
{
    dk_gpio_init();
	configure_all_reset_pins();
    i2c_init();

    tca_init();
    //max_clear_all();  // TEST
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

uint8_t gpio_reset(uint8_t channel)  
{
    uint8_t pin = channel + 3;  // gpio pins start with number 3 and end with 10
    enable_pin(pin);
    k_msleep(100);
    disable_pin(pin);
    return 0;
}

void test_max_chip(void)  
{
    max_clear_all();

    uint8_t read_A0 = max_read_dir0();  // should be 0
    uint8_t read_A1 = max_read_dir1();  // 
    uint8_t read_B0 = max_read_dir2();  // 
    uint8_t read_B1 = max_read_dir3();  // 

    if (read_A0 == read_A1 == read_B0 == read_B1 == 0) 
    {
        LOG_INF("MAX test OK");
    }
    else 
    {
        LOG_INF("MAX test ERR");
    }
}

uint8_t max_set_jtag(uint8_t channel) 
{
    if (channel == -1)  // TODO
	{

    }
    else 
    {

    }
    return 0;
}

// TODO check this
/* MAX14661 functions */

int max_set_A_switches(uint16_t switch_config)
{
    uint8_t swA0 = switch_config & 0xFF;  //lower byte configures A08-A01
    uint8_t swA1 = (switch_config & 0xFF00) >> 8;  //upper byte configures A16-A09
    max_set_A(swA0, swA1);  // control is done using shadow registers - this allows for simultaneous updates of the switches

    uint8_t read_A0 = max_read_dir0();  // read switch states to make sure setting was successful
    uint8_t read_A1 = max_read_dir1();  // read switch states to make sure setting was successful

    if (read_A0 == swA0 && read_A1 == swA1) {
        return 0;
    }
    else {
        return -1;
    }
}

int max_set_B_switches(uint16_t switch_config)
{
    uint8_t swB0 = switch_config & 0xFF;  //lower byte configures B08-B01
    uint8_t swB1 = (switch_config & 0xFF00) >> 8;  //upper byte configures B16-B09
    max_set_A(swB0, swB1);

    uint8_t read_B0 = max_read_dir2();  // read switch states to make sure setting was successful
    uint8_t read_B1 = max_read_dir3();  // read switch states to make sure setting was successful

    if (read_B0 == swB0 && read_B1 == swB1) {
        return 0;
    }
    else {
        return -1;
    }
}
