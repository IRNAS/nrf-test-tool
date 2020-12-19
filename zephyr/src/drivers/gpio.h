#include <zephyr.h>
#include <drivers/gpio.h>

#ifndef _PINS_H_
#define _PINS_H_

#define PIN_BUTTON_1 16
#define PIN_BUTTON_2 17
#define PIN_BUTTON_3 18
#define PIN_BUTTON_4 19

#define PIN_NRF52_RESET_T0 3  // reset nrf52 on target 0 (T0)
#define PIN_NRF91_RESET_T0 4  // reset nrf91 on target 0 (T0)

#define PIN_NRF52_RESET_T1 5  // reset nrf52 on target 1 (T1)
#define PIN_NRF91_RESET_T1 6  // reset nrf91 on target 1 (T1)

#define PIN_NRF52_RESET_T2 7  // reset nrf52 on target 2 (T2)
#define PIN_NRF91_RESET_T2 8  // reset nrf91 on target 2 (T2)

#define PIN_NRF52_RESET_T3 9  // reset nrf52 on target 3 (T3)
#define PIN_NRF91_RESET_T3 10  // reset nrf91 on target 3 (T3)

#endif

int dk_gpio_init(void);
void configure_all_pins(void);

void button_1_pressed(void (*cb_func)(struct device *dev, struct gpio_callback *cb, u32_t pin));
void button_2_pressed(void (*cb_func)(struct device *dev, struct gpio_callback *cb, u32_t pin));
void button_3_pressed(void (*cb_func)(struct device *dev, struct gpio_callback *cb, u32_t pin));
void button_4_pressed(void (*cb_func)(struct device *dev, struct gpio_callback *cb, u32_t pin));

void enable_button_interrupt(uint8_t button);
void disable_button_interrupt(uint8_t button);

void reset_pin(uint8_t pin);
void enable_pin(uint8_t pin);
void disable_pin(uint8_t pin);