#include "communication.h"

#include <stdlib.h>
#include <logging/log.h>

#include <i2c_funs.h>
#include <gpio.h>
#include <tca_interface.hpp>
#include <max_interface.hpp>

#define LED_CHANNELS_OFFSET     8

LOG_MODULE_REGISTER(communication);

void initialize_peripherals(void) 
{
    // init gpio and i2c
    dk_gpio_init();
	//configure_all_reset_pins();
    i2c_init();

    // init TCA chip
    tca_init();

    // init MAX chip
    configure_pin(PIN_JTAG_ROUTE_OUT, GPIO_OUTPUT);
    configure_pin(PIN_MAX_SD, GPIO_OUTPUT);
    enable_pin(PIN_MAX_SD);
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
        k_sleep(K_MSEC(10));
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
    LOG_DBG("pin %d OLD state: %d", pin, res);
    write_pin(pin, new_pin_state_pro);
    res = read_pin(pin);
    LOG_DBG("pin %d NEW state: %d", pin, res);

    pin++;
    res = read_pin(pin);
    LOG_DBG("pin %d OLD state: %d", pin, res);
    write_pin(pin, new_pin_state_ldo);
    res = read_pin(pin);
    LOG_DBG("pin %d NEW state: %d", pin, res);

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
    LOG_DBG("pin %d OLD state: %d", pin, res);
    write_pin(pin, new_pin_state);
    res = read_pin(pin);
    LOG_DBG("pin %d NEW state: %d", pin, res);
    
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
    uint8_t read_A0 = max_read_dir0();
    uint8_t read_A1 = max_read_dir1();
    uint8_t read_B0 = max_read_dir2();
    uint8_t read_B1 = max_read_dir3();
    LOG_INF("MAX TEST read: A0: %d, A1: %d, B0: %d, B1: %d", read_A0, read_A1, read_B0, read_B1);
}

uint8_t max_set_jtag(int8_t channel) 
{
    max_clear_all();
    if (channel == -1)
	{
        disable_pin(PIN_JTAG_ROUTE_OUT);
        LOG_INF("MAX all off");
    }
    else 
    {
        enable_pin(PIN_JTAG_ROUTE_OUT);
        uint8_t line = channel * 2;
        uint16_t swdclk_line = 1 << line;
        uint16_t swdio_line = 1 << (line + 1);
        max_set_A_switches(swdclk_line);
        max_set_B_switches(swdio_line);
        LOG_INF("MAX on CLK: %d, IO: %d", swdclk_line, swdio_line);
    }
    return 0;
}
