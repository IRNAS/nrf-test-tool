#include "gpio.h"

#include <zephyr.h>
#include <drivers/gpio.h>
#include <logging/log.h>

LOG_MODULE_REGISTER(pins);

struct device *gpio_dev;

static struct gpio_callback button_1_cb;
static struct gpio_callback button_2_cb;
static struct gpio_callback button_3_cb;
static struct gpio_callback button_4_cb;

// call this first, sets up gpio device to be used in other calls to config or set pins
int dk_gpio_init(void)
{
    int err = 0;

    gpio_dev = device_get_binding(DT_LABEL(DT_NODELABEL(gpio0)));
    if (!gpio_dev)
    {
        LOG_ERR("Cannot bind gpio device");
        return -ENODEV;
    }
    
    configure_pin(UART_RX_ENABLE, GPIO_OUTPUT);
    enable_pin(UART_RX_ENABLE);
    configure_pin(UART_TX_ENABLE, GPIO_OUTPUT);
    enable_pin(UART_TX_ENABLE);

    return err;
}

void configure_pin(uint8_t pin, uint32_t direction) 
{
    gpio_pin_configure(gpio_dev, pin, direction);
}

void enable_pin(uint8_t pin)
{
    gpio_pin_set(gpio_dev, pin, 1);
}

void disable_pin(uint8_t pin)
{
    gpio_pin_set(gpio_dev, pin, 0);
}

// trigger reset on pin
// void reset_pin(uint8_t pin) 
// {
//     gpio_pin_set(gpio_dev, pin, 1);
//     k_sleep(K_MSEC(10));
//     gpio_pin_set(gpio_dev, pin, 0);
//     k_sleep(K_MSEC(20));
// }

// pin configurations
void configure_all_reset_pins(void)
{
    // configure target 0 pins
    configure_pin(PIN_NRF52_RESET_T0, GPIO_OUTPUT);
    configure_pin(PIN_NRF91_RESET_T0, GPIO_OUTPUT);

    // configure target 1 pins
    configure_pin(PIN_NRF52_RESET_T1, GPIO_OUTPUT);
    configure_pin(PIN_NRF91_RESET_T1, GPIO_OUTPUT);

    // configure target 2 pins
    configure_pin(PIN_NRF52_RESET_T2, GPIO_OUTPUT);
    configure_pin(PIN_NRF91_RESET_T2, GPIO_OUTPUT);

    // configure target 3 pins
    configure_pin(PIN_NRF52_RESET_T3, GPIO_OUTPUT);
    configure_pin(PIN_NRF91_RESET_T3, GPIO_OUTPUT);
}

struct k_delayed_work work_enable_button_1_interrupt;

void work_handler_enable_button_1_interput(struct k_work *work)
{
    enable_button_interrupt(PIN_BUTTON_1);
}

void enable_button_1_delayed_interrupt(k_timeout_t delay)
{
    k_delayed_work_submit(&work_enable_button_1_interrupt, delay);
}

// button 1 callback
void button_1_pressed(void (*cb_func)(struct device *dev, struct gpio_callback *cb, u32_t pin))
{
    LOG_INF("INIT BUTTON 1 INTERRUPT");
    gpio_pin_configure(gpio_dev, PIN_BUTTON_1, GPIO_INPUT | GPIO_ACTIVE_HIGH);

    gpio_init_callback(&button_1_cb, cb_func, BIT(PIN_BUTTON_1));
    gpio_add_callback(gpio_dev, &button_1_cb);

    enable_button_interrupt(PIN_BUTTON_1);

    k_delayed_work_init(&work_enable_button_1_interrupt, work_handler_enable_button_1_interput);
}


struct k_delayed_work work_enable_button_2_interrupt;

void work_handler_enable_button_2_interput(struct k_work *work)
{
    enable_button_interrupt(PIN_BUTTON_2);
}

void enable_button_2_delayed_interrupt(k_timeout_t delay)
{
    k_delayed_work_submit(&work_enable_button_2_interrupt, delay);
}

// button 2 callback
void button_2_pressed(void (*cb_func)(struct device *dev, struct gpio_callback *cb, u32_t pin))
{
    gpio_pin_configure(gpio_dev, PIN_BUTTON_1, GPIO_INPUT | GPIO_ACTIVE_HIGH);

    gpio_init_callback(&button_2_cb, cb_func, BIT(PIN_BUTTON_2));
    gpio_add_callback(gpio_dev, &button_2_cb);

    enable_button_interrupt(PIN_BUTTON_2);

    k_delayed_work_init(&work_enable_button_2_interrupt, work_handler_enable_button_2_interput);
}

// button 3 callback
void button_3_pressed(void (*cb_func)(struct device *dev, struct gpio_callback *cb, u32_t pin))
{
    gpio_pin_configure(gpio_dev, PIN_BUTTON_3, GPIO_INPUT | GPIO_ACTIVE_HIGH);

    gpio_init_callback(&button_3_cb, cb_func, BIT(PIN_BUTTON_3));
    gpio_add_callback(gpio_dev, &button_3_cb);

    enable_button_interrupt(PIN_BUTTON_3);
}

// button 4 callback
void button_4_pressed(void (*cb_func)(struct device *dev, struct gpio_callback *cb, u32_t pin))
{
    gpio_pin_configure(gpio_dev, PIN_BUTTON_4, GPIO_INPUT | GPIO_ACTIVE_HIGH);

    gpio_init_callback(&button_4_cb, cb_func, BIT(PIN_BUTTON_4));
    gpio_add_callback(gpio_dev, &button_4_cb);

    enable_button_interrupt(PIN_BUTTON_4);
}

/* configure button interrupts */
void enable_button_interrupt(uint8_t button)
{
    gpio_pin_interrupt_configure(gpio_dev, button, GPIO_INT_LEVEL_ACTIVE);
}

void disable_button_interrupt(uint8_t button)
{
    gpio_pin_interrupt_configure(gpio_dev, button, GPIO_INT_DISABLE);
}