// TODO wrapper functions for drivers
#include "communication.h"

#include <stdlib.h>
#include <logging/log.h>

#include <peripherals.h>
#include <tca_interface.hpp>
#include <max_interface.hpp>
#include <gpio.h>

#define MAX_SD_PIN              2
#define JTAG_ROUTE_OUT          11
#define LED_CHANNELS_OFFSET     8

LOG_MODULE_REGISTER(communication);

void initialize_peripherals(void) 
{
    dk_gpio_init();
	//configure_all_reset_pins();
    i2c_init();

    tca_init();

    enable_pin(MAX_SD_PIN);
    max_clear_all();
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

    if (strcmp("on", state) == 0)  // state on, turn on power from ppk (LDO)
    {
      new_pin_state_pro = 0;
      new_pin_state_ldo = 1;
    }
    else if (strcmp("ppk", state) == 0)   // turn on power from nrf91 (PRO)
    {   
        new_pin_state_pro = 1;
        new_pin_state_ldo = 0;
    }
    else    // turn off both pins on current target
    {
        new_pin_state_pro = 0;
        new_pin_state_ldo = 0;
    }

    res = read_pin(pin);
    LOG_INF("pin %d OLD state: %d", pin, res);
    write_pin(pin, new_pin_state_pro);
    res = read_pin(pin);
    LOG_INF("pin %d NEW state: %d", pin, res);

    pin++;
    res = read_pin(pin);
    LOG_INF("pin %d OLD state: %d", pin, res);
    write_pin(pin, new_pin_state_ldo);
    res = read_pin(pin);
    LOG_INF("pin %d NEW state: %d", pin, res);

    return 0;
}

uint8_t tca_set_led(uint8_t channel, char *state)
{
    int res;
    uint8_t pin = channel + LED_CHANNELS_OFFSET;
    uint8_t new_pin_state;

    if (strcmp("on", state) == 0)  // turn on pin on current channel
    {
      new_pin_state = 1;
    }
    else    // turn off pin on current channel
    {
        new_pin_state = 0;
    }

    res = read_pin(pin);
    LOG_INF("pin %d OLD state: %d", pin, res);
    write_pin(pin, new_pin_state);
    res = read_pin(pin);
    LOG_INF("pin %d NEW state: %d", pin, res);
    
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
    uint8_t read_A0 = max_read_dir0();  // should be 0
    uint8_t read_A1 = max_read_dir1();  // 
    uint8_t read_B0 = max_read_dir2();  // 
    uint8_t read_B1 = max_read_dir3();  // 
    LOG_INF("MAX TEST read: A0: %d, A1: %d, A2: %d, A3: %d", read_A0, read_A1, read_B0, read_B1);
}

uint8_t max_set_jtag(int8_t channel) 
{
    max_set_A_switches(0);
    max_set_B_switches(0);
    if (channel == -1)
	{
        LOG_INF("MAX all off");
    }
    else 
    {
        enable_pin(JTAG_ROUTE_OUT);
        uint16_t swdclk_line = 1 << channel;
        uint16_t swdio_line = 1 << (channel + 1);
        max_set_A_switches(swdclk_line);
        max_set_B_switches(swdio_line);
        LOG_INF("MAX on CLK: %d, IO: %d", swdclk_line, swdio_line);
    }
    return 0;
}

// TODO check this
/* MAX14661 functions */

uint8_t max_set_A_switches(uint16_t switch_config)
{
    uint8_t swA0 = switch_config & 0xFF;  //lower byte configures A08-A01
    uint8_t swA1 = (switch_config & 0xFF00) >> 8;  //upper byte configures A16-A09
    max_set_A(swA0, swA1);  // control is done using shadow registers - this allows for simultaneous updates of the switches
    return 0;
}

uint8_t max_set_B_switches(uint16_t switch_config)
{
    uint8_t swB0 = switch_config & 0xFF;  //lower byte configures B08-B01
    uint8_t swB1 = (switch_config & 0xFF00) >> 8;  //upper byte configures B16-B09
    max_set_B(swB0, swB1);
    return 0;
}
