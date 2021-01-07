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

// EXAMPLE SUBCOMMAND
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
// END OF EXAMPLE

static int cmd_test_tca(const struct shell *shell, size_t argc, char **argv)
{
	test_tca_chip();
	return 0;
}

static int cmd_test_max(const struct shell *shell, size_t argc, char **argv)
{
	test_max_chip();
	return 0;
}

static int cmd_test_ads(const struct shell *shell, size_t argc, char **argv)
{
	test_ads_chip();
	return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sub_test,
	SHELL_CMD(tca, NULL, "Test TCA6424.", cmd_test_tca),
	SHELL_CMD(max, NULL, "Test MAX14661.", cmd_test_max),
	SHELL_CMD(ads, NULL, "Test ADS1015.", cmd_test_ads),
	SHELL_SUBCMD_SET_END /* Array terminated. */
);
/* Creating root (level 0) command "test" */
SHELL_CMD_REGISTER(test, &sub_test, "Test commands:", NULL);

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

static int cmd_led(const struct shell *shell, size_t argc, char **argv)
{
	char *after_num = NULL;
	int channel = strtol(argv[1], &after_num, 10);
	if (channel < 0 || channel > MAX_CHANNEL_NUM) 
	{
		shell_print(shell, "ERROR: wrong parameter <channel>");
		return -1;
	}
	if (strcmp("off", argv[2]) != 0 && strcmp("on", argv[2]) != 0) 
	{
		shell_print(shell, "wrong parameter <state> \nERROR");
		return -1;
	}
	char *state = argv[2];
	shell_print(shell, "Received command: led state: %s for channel: %d", state, channel);
	int err = tca_set_led(channel, state);		// TODO return OK or ERR
	shell_print(shell, "OK");
	return 0;
}
SHELL_CMD_ARG_REGISTER(led, NULL, "parameters: <channel num (0-7)> <state (on or off)>", cmd_led, 3, 0);

static int i2c_scan(const struct shell *shell, size_t argc, char **argv)
{
	perform_i2c_scan();
	shell_print(shell, "OK");
	return 0;
}
SHELL_CMD_REGISTER(scan, NULL, "testing i2c (init and scan)", i2c_scan);

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

	// TODO
	// enable_pin(PIN_NRF52_RESET_T0);
	// k_sleep(K_SECONDS(5));
	// disable_pin(PIN_NRF52_RESET_T0);

	// button_1_pressed(button_1_interrupt_handler);
	// button_2_pressed(button_2_interrupt_handler);
}
