/*
 * Copyright (c) 2015 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <sys/printk.h>
#include <shell/shell.h>
#include <shell/shell_uart.h>
#include <version.h>
#include <logging/log.h>
#include <stdlib.h>

#include "communication.h"
#include <gpio.h>

LOG_MODULE_REGISTER(app);

static int cmd_demo_ping(const struct shell *shell, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	shell_print(shell, "pong");

	return 0;
}

static int cmd_demo_params(const struct shell *shell, size_t argc, char **argv)
{
	shell_print(shell, "argc = %d", argc);
	for (size_t cnt = 0; cnt < argc; cnt++) {
		shell_print(shell, "  argv[%d] = %s", cnt, argv[cnt]);
	}

	return 0;
}

static int cmd_demo_hexdump(const struct shell *shell, size_t argc, char **argv)
{
	shell_print(shell, "argc = %d", argc);
	for (size_t cnt = 0; cnt < argc; cnt++) {
		shell_print(shell, "argv[%d]", cnt);
		shell_hexdump(shell, argv[cnt], strlen(argv[cnt]));
	}

	return 0;
}

static int cmd_version(const struct shell *shell, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	shell_print(shell, "Zephyr version %s", KERNEL_VERSION_STRING);

	return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sub_demo,
	SHELL_CMD(hexdump, NULL, "Hexdump params command.", cmd_demo_hexdump),
	SHELL_CMD(params, NULL, "Print params command.", cmd_demo_params),
	SHELL_CMD(ping, NULL, "Ping command.", cmd_demo_ping),
	SHELL_SUBCMD_SET_END /* Array terminated. */
);
SHELL_CMD_REGISTER(demo, &sub_demo, "Demo commands", NULL);

SHELL_CMD_ARG_REGISTER(version, NULL, "Show kernel version", cmd_version, 1, 0);

// DOCS: http://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/zephyr/reference/shell/index.html?highlight=shell_cmd_arg_register

/* ------------------------------------ CUSTOM COMMANDS ------------------------------------ */
#define MAX_CHANNEL_NUM 	7	// we have 8 channels (0-7)
#define MAX_TARGET_NUM		3	// we have 4 targets (0-3)
#define MAX_ADC_NUM			15	// we have 16 ADC channels (0-15)

static int cmd_power(const struct shell *shell, size_t argc, char **argv)
{
	char *after_num = NULL;
	int target = strtol(argv[1], &after_num, 10);
	if (target < 0 || target > MAX_TARGET_NUM) 
	{
		shell_print(shell, "wrong parameter <target> \nERROR");
		return -1;
	}
	if (strcmp("off", argv[2]) != 0 && strcmp("ppk", argv[2]) != 0 && strcmp("on", argv[2]) != 0) 
	{
		shell_print(shell, "wrong parameter <state> \nERROR");
		return -1;
	}
	char *state = argv[2];
	shell_print(shell, "Received command: power state: %s for target: %d", state, target);
	int err = tca_set_power(target, state);		// TODO return OK or ERR
	shell_print(shell, "OK");
	return 0;
}
SHELL_CMD_ARG_REGISTER(power, NULL, "parameters: <target num (0-3)> <state (on, ppk or off)> \nERROR", cmd_power, 3, 0);

static int cmd_relay(const struct shell *shell, size_t argc, char **argv)
{
	char *after_num = NULL;
	int target = strtol(argv[1], &after_num, 10);
	if (target < 0 || target > MAX_TARGET_NUM) 
	{
		shell_print(shell, "wrong parameter <target> \nERROR");
		return -1;
	}
	if (strcmp("off", argv[2]) != 0 && strcmp("on", argv[2]) != 0) 
	{
		shell_print(shell, "wrong parameter <state> \nERROR");
		return -1;
	}
	if (strcmp("battery", argv[3]) != 0 && strcmp("charge", argv[3]) != 0)
	{
		shell_print(shell, "wrong parameter <relay> \nERROR");
		return -1;
	}
	char *state = argv[2];
	char *relay = argv[3];
	shell_print(shell, "Received command: relay state: %s on relay: %s on target: %d", state, relay, target);
	int err = tca_set_relay(target, relay, state);		// TODO return OK or ERR
	shell_print(shell, "OK");
	return 0;
}
SHELL_CMD_ARG_REGISTER(relay, NULL, "parameters: <target num (0-3)> <state (on, off)> <relay (battery, charge)> \nERROR", cmd_relay, 4, 0);

static int cmd_jtag(const struct shell *shell, size_t argc, char **argv)
{

	char *after_num = NULL;
	int channel = strtol(argv[1], &after_num, 10);
	if (channel < -1 || channel > MAX_CHANNEL_NUM) 
	{
		shell_print(shell, "ERROR: wrong parameter <channel>");
		return -1;
	}
  
	if (channel == -1) 
	{
		shell_print(shell, "Received command: jtag all channels off.", channel);
	}
	else 
	{
		shell_print(shell, "Received command: jtag channel %d on.", channel);
	}
	int err = max_set_jtag(channel);	// TODO return OK or ERR
	shell_print(shell, "OK");
	return 0;
}
SHELL_CMD_ARG_REGISTER(jtag, NULL, "parameters: <channel num (0-7) or -1 (off)>", cmd_jtag, 2, 0);

static int cmd_bank(const struct shell *shell, size_t argc, char **argv)
{

	char *after_num = NULL;
	int bank = strtol(argv[1], &after_num, 10);
	if (bank < -1 || bank > 2) 
	{
		shell_print(shell, "ERROR: wrong parameter <bank>");
		return -1;
	}
  
	else 
	{
		shell_print(shell, "Received command: read bank %d on.", bank);
	}
	shell_print(shell, "Bank direction: \n");
	int err = tca_read_bank_direction(bank);	// TODO return OK or ERR

	shell_print(shell, "Bank inputs: \n");
	err = tca_read_bank(bank);

	shell_print(shell, "Bank outputs: \n");
	err = tca_read_bank_outputs(bank);

	shell_print(shell, "OK");
	return 0;
}
SHELL_CMD_ARG_REGISTER(bank, NULL, "parameters: <bank (0, 1, 2)>", cmd_bank, 2, 0);

static int cmd_uart(const struct shell *shell, size_t argc, char **argv)
{
	if (strcmp("off", argv[1]) != 0 && strcmp("on", argv[1])) 
	{
		shell_print(shell, "wrong parameter <state> \nERROR");
		return -1;
	}
	char *state = argv[1];
	shell_print(shell, "Received command: uart enable %s", state);
	int err = toggle_uart(state);
	shell_print(shell, "OK");
	return 0;
}
SHELL_CMD_ARG_REGISTER(uart, NULL, "parameters: <state (on, off)>", cmd_uart, 2, 0);

static int cmd_reset(const struct shell *shell, size_t argc, char **argv)
{
	char *after_num = NULL;
	int channel = strtol(argv[1], &after_num, 10);
	if (channel < 0 || channel > MAX_CHANNEL_NUM) 
	{
		shell_print(shell, "ERROR: wrong parameter <channel>");
		return -1;
	}
	shell_print(shell, "Received command: reset channel %d.", channel);
	int err = gpio_reset(channel);		// TODO return OK or ERR
	shell_print(shell, "OK");
	return 0;
}
SHELL_CMD_ARG_REGISTER(reset, NULL, "parameters: <channel num (0-7)>", cmd_reset, 2, 0);

static int cmd_red_led(const struct shell *shell, size_t argc, char **argv)
{
	char *after_num = NULL;
	int target = strtol(argv[1], &after_num, 10);
	if (target < 0 || target > MAX_CHANNEL_NUM) 
	{
		shell_print(shell, "ERROR: wrong parameter <target>");
		return -1;
	}
	if (strcmp("off", argv[2]) != 0 && strcmp("on", argv[2]) != 0) 
	{
		shell_print(shell, "wrong parameter <state> \nERROR");
		return -1;
	}
	char *state = argv[2];
	shell_print(shell, "Received command: led_red %s on target %d.", state, target);
	int err = set_red_led(target, state);		// TODO return OK or ERR
	shell_print(shell, "OK");
	return 0;
}
SHELL_CMD_ARG_REGISTER(set_red_led, NULL, "parameters: <target num (0-3) state (on,off)>", cmd_red_led, 3, 0);

static int cmd_adc(const struct shell *shell, size_t argc, char **argv)
{
	char *after_num = NULL;
	int channel = strtol(argv[1], &after_num, 10);
	if (channel < 0 || channel > MAX_ADC_NUM) 
	{
		shell_print(shell, "ERROR: wrong parameter <channel>");
		return -1;
	}
	shell_print(shell, "Received command: adc channel %d.", channel);
	uint8_t target = (uint8_t)(channel/4);
	channel = channel % 4;
	uint16_t value = adc_read_voltage(target, channel);

	shell_print(shell, "Read analog value (mV): %d", value);
	shell_print(shell, "OK");
	return 0;
}
SHELL_CMD_ARG_REGISTER(adc, NULL, "parameters: <adc channel num (0-15)>", cmd_adc, 2, 0);

static int cmd_gain(const struct shell *shell, size_t argc, char **argv)
{
	char *after_num = NULL;
	int target = strtol(argv[1], &after_num, 10);
	if (target < 0 || target > MAX_TARGET_NUM) 
	{
		shell_print(shell, "ERROR: wrong parameter <target>");
		return -1;
	}
	uint16_t gain = strtol(argv[2], &after_num, 0);
	adc_set_gain(target, gain);

	uint16_t read_gain = adc_get_gain(target);
	shell_print(shell, "Read adc %u gain: %x", read_gain, gain);
	if (read_gain != gain)
	{
		shell_print(shell, "ERROR");
	}
	else
	{
		shell_print(shell, "OK");
	}
	return 0;
}
SHELL_CMD_ARG_REGISTER(gain, NULL, "parameters: <adc target (0-3), gain (0x000, 0x0200, 0x0400, 0x0600, 0x0800, 0x0A00)>", cmd_gain, 3, 0);

static int cmd_led(const struct shell *shell, size_t argc, char **argv)
{
	char *after_num = NULL;
	int target = strtol(argv[1], &after_num, 10);
	if (target < 0 || target > MAX_TARGET_NUM) 
	{
		shell_print(shell, "ERROR: wrong parameter <target>");
		return -1;
	}
	if (strcmp("off", argv[2]) != 0 && strcmp("on", argv[2]) != 0 && strcmp("blink", argv[2]) != 0) 
	{
		shell_print(shell, "wrong parameter <state> \nERROR");
		return -1;
	}
	if (argc > 3) 
	{
		if (strcmp("green", argv[3]) != 0 && strcmp("red", argv[3]) != 0 && strcmp("orange", argv[3]) != 0) 
		{
			shell_print(shell, "wrong parameter <color> \nERROR");
			return -1;
		}
	}
	
	char *state = argv[2];
	char *color;
	if (argc > 3) 
	{
		color = argv[3];
	}
	else 
	{
		color = NULL;
	}
	shell_print(shell, "Received command: led state: %s for target: %d", state, target);
	int err = tca_set_led(target, state, color);		// TODO return OK or ERR
	shell_print(shell, "OK");
	return 0;
}
SHELL_CMD_ARG_REGISTER(led, NULL, "parameters: <channel num (0-7)> <state (on, off, blink)> <color (red, green, orange)>", cmd_led, 3, 1);

// read both device pins on target
static int cmd_detect(const struct shell *shell, size_t argc, char **argv)
{
	char *after_num = NULL;
	int target = strtol(argv[1], &after_num, 10);
	if (target < 0 || target > MAX_TARGET_NUM) 
	{
		shell_print(shell, "wrong parameter <target> \nERROR");
		return -1;
	}
	shell_print(shell, "Received command: detect devices on target: %d", target);
	// uint8_t channel = (3 - target) * 2;  // detect board if present - pins are reversed so its 3-target
	int err = tca_detect_device(target);		// TODO return OK or ERR
	int ret;
	if (err == 0)
	{
		ret = 1;  // print 1 if device is present
	}
	else if (err == 1)
	{
		ret = 0;  // print 0 if device is not present
	}
	else
	{
		ret = err;  // else return error
	}
	shell_print(shell, "Detected board: %d", ret);
	shell_print(shell, "OK");
	return 0;
}
SHELL_CMD_ARG_REGISTER(detect, NULL, "parameters: <target num (0-3)> \nERROR", cmd_detect, 2, 0);

static int i2c_scan(const struct shell *shell, size_t argc, char **argv)
{
	perform_i2c_scan();
	shell_print(shell, "OK");
	return 0;
}
SHELL_CMD_REGISTER(scan, NULL, "testing i2c (init and scan)", i2c_scan);

static int test_tca(const struct shell *shell, size_t argc, char **argv)
{
	test_tca_chip();
	return 0;
}
SHELL_CMD_REGISTER(tca, NULL, "testing tca", test_tca);

static int test_max(const struct shell *shell, size_t argc, char **argv)
{
	test_max_chip();
	return 0;
}
SHELL_CMD_REGISTER(max, NULL, "testing max", test_max);

static int test_adc(const struct shell *shell, size_t argc, char **argv)
{
	test_adc_chip();
	return 0;
}
SHELL_CMD_REGISTER(adc_test, NULL, "testing adc", test_adc);

/* INTERRUPTS */	// TODO
void button_1_interrupt_handler(struct device *dev, struct gpio_callback *cb, u32_t pin)
{
	disable_button_interrupt(PIN_BUTTON_1);

	//LOG_INF("BUTTON 1 PRESSED");
	enable_pin(PIN_NRF52_RESET_T0);  // enable LED 2
	//printk("A");

	enable_button_1_delayed_interrupt(K_SECONDS(5));
}

void button_2_interrupt_handler(struct device *dev, struct gpio_callback *cb, u32_t pin)
{
	disable_button_interrupt(PIN_BUTTON_2);

	//LOG_INF("BUTTON 1 PRESSED");
	disable_pin(PIN_NRF52_RESET_T0);  // disable LED 2
	//printk("A");

	//enable_button_interrupt(PIN_BUTTON_2);
	enable_button_2_delayed_interrupt(K_SECONDS(5));
}

void main(void) 
{ 
	LOG_INF("Nrf test tool: Hello");
	initialize_peripherals();

	init_tca_blink_work();

	// TODO
	// enable_pin(PIN_NRF52_RESET_T0);
	// k_sleep(K_SECONDS(5));
	// disable_pin(PIN_NRF52_RESET_T0);

	// button_1_pressed(button_1_interrupt_handler);
	// button_2_pressed(button_2_interrupt_handler);
}
