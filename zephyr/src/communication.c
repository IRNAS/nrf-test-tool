#include "communication.h"

#include <stdlib.h>
#include <logging/log.h>

#include <i2c_funs.h>
#include <gpio.h>
#include <tca_interface.hpp>
#include <max_interface.hpp>
#include <ads1015_interface.hpp>

#define LED_CHANNELS_OFFSET     8
#define DEVICE_CHANNELS_OFFSET 8

LOG_MODULE_REGISTER(communication);

static int color[4] = {-1};

void initialize_peripherals(void) 
{
    // init gpio and i2c
    dk_gpio_init();
	configure_all_reset_pins();
    i2c_init();

    // init TCA chip
    tca_init();

    // init ADC chips
    ads_init();

    // init MAX chip
    configure_pin(PIN_JTAG_ROUTE_OUT, GPIO_OUTPUT);
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

    for (int i = 0; i < 24; i++)  // read all 24 pins
    {
        // if (i == 3 || i == 7 || i == 11) 
        // {
        //     continue;
        // }
        int res = read_pin(i);
        LOG_INF("%d result: %x", i, res);
        k_sleep(K_MSEC(10));
    }
}

uint8_t tca_set_relay(uint8_t target, char *relay, char *state)
{
    int res;
    uint8_t pin = 23 - target * 2;  // skip first 16 pins
    // pin 16 is idc4
    // we need idc1 (22 - test_6, 23 - test_5)
    // we need idc2 (20 - test_6, 21 - test_5)
    uint8_t new_pin_state;

    if (strcmp("on", state) == 0)  // state on, turn on power from ppk (LDO)
    {
      new_pin_state = 1;
    }
    else if (strcmp("off", state) == 0)   // turn on power from nrf91 (PRO)
    {   
        new_pin_state = 0;
    }

    if (strcmp("battery", relay) == 0)
    {
        pin = pin;  // battery pin is odd
    }
    else if (strcmp("charge", relay) == 0)
    {
        pin = pin - 1;  // battery pin is even
    }

    res = read_pin(pin);
    LOG_DBG("pin %d OLD state: %d", pin, res);
    write_pin(pin, new_pin_state);
    res = read_pin(pin);
    LOG_DBG("pin %d NEW state: %d", pin, res);

    return 0;
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



void toggle_led(uint8_t target, uint8_t new_pin_state)
{
    uint8_t pin;
    uint8_t off_pin;
    int res;
    LOG_DBG("color %d on target %d\n", color[target], target);

    if (color[target] == 1 || color[target] == 2) 
    {
        if (color[target] == 1)  // red
        {
            pin = target * 2 + LED_CHANNELS_OFFSET;
            off_pin = pin + 1;
            write_pin(off_pin, 1);
            if (new_pin_state == 1)
            {
                set_red_led(target, "on");
            }
            if (new_pin_state == 0)
            {
                set_red_led(target, "off");
            }
        }
        if (color[target] == 2)  // green
        {
            pin = target * 2 + 1 + LED_CHANNELS_OFFSET;
            off_pin = pin - 1;
            // polarity of output pins is inversed
            if (new_pin_state == 1)
            {
                new_pin_state = 0;
            }
            else if (new_pin_state == 0)
            {
                new_pin_state = 1;
            }
            write_pin(pin, new_pin_state);
            set_red_led(target, "off");
            // if (new_pin_state == 1)
            // {
            //     set_red_led(target, "off");
            // }
        }
        // write_pin(off_pin, 0);
    }
    else if (color[target] == 3)
    {
        pin = target * 2 + 1 + LED_CHANNELS_OFFSET;
        res = read_pin(pin);
        // polarity of output pins is inversed
        if (new_pin_state == 1)
        {
            set_red_led(target, "on");
        }
        if (new_pin_state == 0)
        {
            set_red_led(target, "off");
        }
        // LOG_DBG("pin %d OLD state: %d", pin, res);
        if (new_pin_state == 1)
        {
            new_pin_state = 0;
        }
        else if (new_pin_state == 0)
        {
            new_pin_state = 1;
        }
        write_pin(pin, new_pin_state);
    }   
}

bool blinking[4] = {false};
bool led_state[4] = {false};

struct k_delayed_work work_toggle_led_0;

void work_handler_toggle_led_0(struct k_work *work)
{
    // LOG_DBG("blinking 0 setting %d", blinking[0]);
    if (blinking[0] == true) 
    {
        // LOG_DBG("blinking led");
        if (led_state[0] == false)
        {
            toggle_led(0, 1);
            // write_pin(8, 1);  // if pin is off toggle it on
            // write_pin(9, 1);  // if pin is off toggle it on
            led_state[0] = !led_state[0];
        }
        else if (led_state[0] == true)
        {
            toggle_led(0, 0);
            // write_pin(8, 0);  // if pin is on toggle it off
            // write_pin(9, 0);  // if pin is on toggle it off
            led_state[0] = !led_state[0];
        }

        k_delayed_work_submit(&work_toggle_led_0, K_MSEC(500));
    }
}


struct k_delayed_work work_toggle_led_1;

void work_handler_toggle_led_1(struct k_work *work)
{
    if (blinking[1] == true) 
    {
        if (led_state[1] == false)
        {
            toggle_led(1, 1);
            // write_pin(10, 1);  // if pin is off toggle it on
            // write_pin(11, 1);  // if pin is off toggle it on
            led_state[1] = !led_state[1];
        }
        else if (led_state[1] == true)
        {
            toggle_led(1, 0);
            // write_pin(10, 0);  // if pin is on toggle it off
            // write_pin(11, 0);  // if pin is on toggle it off
            led_state[1] = !led_state[1];
        }

        k_delayed_work_submit(&work_toggle_led_1, K_MSEC(500));
    }
}


struct k_delayed_work work_toggle_led_2;

void work_handler_toggle_led_2(struct k_work *work)
{
    if (blinking[2] == true) 
    {
        if (led_state[2] == false)
        {
            toggle_led(2, 1);
            // write_pin(12, 1);  // if pin is off toggle it on
            // write_pin(13, 1);  // if pin is off toggle it on
            led_state[2] = !led_state[2];
        }
        else if (led_state[2] == true)
        {
            toggle_led(2, 0);
            // write_pin(12, 0);  // if pin is on toggle it off
            // write_pin(13, 0);  // if pin is on toggle it off
            led_state[2] = !led_state[2];
        }

        k_delayed_work_submit(&work_toggle_led_2, K_MSEC(500));
    }
}


struct k_delayed_work work_toggle_led_3;

void work_handler_toggle_led_3(struct k_work *work)
{
    if (blinking[3] == true) 
    {
        if (led_state[3] == false)
        {
            toggle_led(3, 1);
            // write_pin(14, 1);  // if pin is off toggle it on
            // write_pin(15, 1);  // if pin is off toggle it on
            led_state[3] = !led_state[3];
        }
        else if (led_state[3] == true)
        {
            toggle_led(3, 0);
            // write_pin(14, 0);  // if pin is on toggle it off
            // write_pin(15, 0);  // if pin is on toggle it off
            led_state[3] = !led_state[3];
        }

        k_delayed_work_submit(&work_toggle_led_3, K_MSEC(500));
    }
}

void init_tca_blink_work()
{
    k_delayed_work_init(&work_toggle_led_0, work_handler_toggle_led_0);
    k_delayed_work_init(&work_toggle_led_1, work_handler_toggle_led_1);
    k_delayed_work_init(&work_toggle_led_2, work_handler_toggle_led_2);
    k_delayed_work_init(&work_toggle_led_3, work_handler_toggle_led_3);
}

uint8_t tca_read_bank(uint8_t bank)
{
    uint8_t bank_state = read_bank(bank);
    LOG_DBG("read bank %d state: %d\n", bank, bank_state);

    return bank_state;
}

uint8_t tca_read_bank_outputs(uint8_t bank)
{
    uint8_t bank_outputs = read_bank_outputs(bank);
    LOG_DBG("read bank outputs %d state: %d\n", bank, bank_outputs);
    return bank_outputs;
}

uint8_t tca_read_bank_direction(uint8_t bank)
{
    uint8_t bank_state = read_bank_direction(bank);
    LOG_DBG("read bank %d direction: %d", bank, bank_state);

    return bank_state;
}

uint8_t tca_set_led(uint8_t target, char *state, char *_color)
{
    // int res;
    // uint8_t pin = channel + LED_CHANNELS_OFFSET;
    uint8_t new_pin_state;

    if (_color != NULL)
    {
        if (strcmp("red", _color) == 0)
        {
            color[target] = 1;
            LOG_DBG("setting color: %d on target: %d", color[target], target);
        }
        if (strcmp("green", _color) == 0) 
        {
            color[target] = 2;
        }
        if (strcmp("orange", _color) == 0)
        {
            color[target] = 3;
        }
    }


    // LOG_DBG("setting color: %d on target: %d", color[target], target);

    if (strcmp("blink", state) == 0) 
    {
        // LOG_DBG("received blink on target %d",target);
        blinking[target] = true;
        if (target == 0)
        {
            // LOG_DBG("submitting work on target %d",target);
            // LOG_DBG("blinking 0 setting %d", blinking[0]);
            k_delayed_work_submit(&work_toggle_led_0, K_NO_WAIT);
        }
        else if (target == 1)
        {
            k_delayed_work_submit(&work_toggle_led_1, K_NO_WAIT);
        }
        else if (target == 2)
        {
            k_delayed_work_submit(&work_toggle_led_2, K_NO_WAIT);
        }
        else if (target == 3)
        {
            k_delayed_work_submit(&work_toggle_led_3, K_NO_WAIT);
        }
    }

    else
    {
        blinking[target] = false;
        if (strcmp("on", state) == 0)  // turn on pin on current channel
        {
            new_pin_state = 1;
        }
        else    // turn off pin on current channel
        {
            new_pin_state = 0;
        }

        toggle_led(target, new_pin_state);        
    }
    
    return 0;
}

uint8_t tca_detect_device(uint8_t target)
{
    int res;
    uint8_t pin = (target * 2) + DEVICE_CHANNELS_OFFSET;

    res = read_pin(pin);
    LOG_DBG("read pin %d state: %d", pin, res);

    return res;
}

uint8_t gpio_reset(uint8_t channel)  
{
    uint8_t pin = channel + 3;  // gpio pins start with number 3 and end with 10
    enable_pin(pin);
    k_sleep(K_MSEC(100));
    disable_pin(pin);
    return 0;
}

uint8_t set_red_led(uint8_t target, char *state)  
{
    uint8_t pin = (target * 2) + 4;  // gpio pins start with number 3 and end with 10
    if (strcmp("on", state) == 0) 
    {
        enable_pin(pin);
        return 0;
    }   
    if (strcmp("off", state) == 0)
    {
        disable_pin(pin);
        return 0;
    }
    return -1;
}

uint8_t toggle_uart(char *state)
{
    if (strcmp("on", state) == 0) 
    {
        enable_uart();
        return 0;
    }   
    if (strcmp("off", state) == 0)
    {
        disable_uart();
        return 0;
    }
    return -1;
}

void test_adc_chip(void)
{
    int16_t readings[4][4] = {{-1, -1, -1, -1}, {-1, -1, -1, -1}, {-1, -1, -1, -1}, {-1, -1, -1, -1}};

    for (uint8_t target = 0; target < 4; target++) 
    {
        for (uint8_t channel = 0; channel < 4; channel++) 
        {
            readings[target][channel] = ads_read_ADC_single_ended(target, channel);
        }
    }


    LOG_INF("ADC TEST TARGET 0 read: A0: %d, A1: %d, A2: %d, A3: %d", readings[0][0], readings[0][1], readings[0][2], readings[0][3]);
    LOG_INF("ADC TEST TARGET 1 read: A0: %d, A1: %d, A2: %d, A3: %d", readings[1][0], readings[1][1], readings[1][2], readings[1][3]);
    LOG_INF("ADC TEST TARGET 2 read: A0: %d, A1: %d, A2: %d, A3: %d", readings[2][0], readings[2][1], readings[2][2], readings[2][3]);
    LOG_INF("ADC TEST TARGET 3 read: A0: %d, A1: %d, A2: %d, A3: %d", readings[3][0], readings[3][1], readings[3][2], readings[3][3]);
}

uint16_t adc_read_voltage(uint8_t target, uint8_t channel)
{
    uint16_t digital_value = ads_read_ADC_single_ended(target, channel);
    return ads_convert_to_analog(target, digital_value);
}

void adc_set_gain(uint8_t target, uint16_t gain)
{
    ads_set_gain(target, gain);
}

uint16_t adc_get_gain(uint8_t target)
{
    return ads_get_gain(target);
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
