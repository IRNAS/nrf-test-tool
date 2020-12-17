#include "communication.h"

#include <device.h>
#include <drivers/i2c.h>
#include <logging/log.h>

LOG_MODULE_REGISTER(communication);

#define I2C_DEV "I2C_0"

struct device *i2c_dev;

int i2c_init(void)
{
    i2c_dev = device_get_binding(I2C_DEV);
    if (!i2c_dev)
    {
        LOG_DBG("I2C_2 error");
        return -1;
    }
    else
    {
        //i2c_configure(i2c_dev, I2C_SPEED_SET(I2C_SPEED_STANDARD));
        LOG_DBG("I2C_2 Init OK");
        return 0;
    }
}

void i2c_scan(void)
{
    uint8_t error = 0u;

    LOG_INF("Starting i2c scanner...");

    /*start and configure i2c*/
    LOG_INF("Value of NRF_TWIM3_NS->PSEL.SCL: %ld ", NRF_TWIM2_NS->PSEL.SCL);
    LOG_INF("Value of NRF_TWIM3_NS->PSEL.SDA: %ld ", NRF_TWIM2_NS->PSEL.SDA);
    LOG_INF("Value of NRF_TWIM3_NS->FREQUENCY: %ld ", NRF_TWIM2_NS->FREQUENCY);

    int found[0x77 - 4];
    int found_i = 0;

    /*search for i2c devices*/
    for (u8_t i = 4; i <= 0x77; i++)
    {
        struct i2c_msg msgs[1];
        u8_t dst = 1;

        /* Send the address to read from */
        msgs[0].buf = &dst;
        msgs[0].len = 1U;
        msgs[0].flags = I2C_MSG_WRITE | I2C_MSG_STOP;

        error = i2c_transfer(i2c_dev, &msgs[0], 1, i);
        if (error == 0)
        {
            // LOG_INF("Found: 0x%2x", i);
            found[found_i++] = i;
        }
        else
        {
            //LOG_DBG("error %d \n", error);
        }
    }
    k_sleep(K_SECONDS(1));
    LOG_INF("\nFound:");
    for (int i = 0; i < found_i; i++)
    {
        LOG_INF("0x%2x", found[i]);
    }
}
