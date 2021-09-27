#include "i2c_funs.h"

#include <drivers/i2c.h>
#include <logging/log.h>

LOG_MODULE_REGISTER(i2c_functions);

#define I2C_DEV "I2C_2"

static struct device *i2c_dev;

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

    /*start and configure i2c*/
    // LOG_INF("Value of NRF_TWIM3_NS->PSEL.SCL: %d ", NRF_TWIM2_NS->PSEL.SCL);
    // LOG_INF("Value of NRF_TWIM3_NS->PSEL.SDA: %d ", NRF_TWIM2_NS->PSEL.SDA);
    // LOG_INF("Value of NRF_TWIM3_NS->FREQUENCY: %d ", NRF_TWIM2_NS->FREQUENCY);

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

/*
 * @brief Writes value to register over I2C
 *
 * @param reg
 * @param val
 *
 */

void write_bit(uint8_t address, uint8_t reg, uint8_t bit, uint8_t val)
{
    if (i2c_reg_update_byte(i2c_dev, address, reg, bit, val) != 0)
    {
        LOG_ERR("Error on i2c_update()");
    }
    else
    {
        LOG_DBG("i2c_update: no error");
    }
}

void write_reg(uint8_t address, uint8_t reg, uint8_t val)
{
    if (i2c_reg_write_byte(i2c_dev, address, reg, val) != 0)
    {
        LOG_ERR("Error on i2c_write()");
    }
    else
    {
        //LOG_DBG("i2c_write: no error");
    }
}

/*
 * @brief Reads value from register over I2C
 *
 * @param reg
 *
 * @return content of reg
 */
uint8_t read_reg(uint8_t address, uint8_t reg)
{
    uint8_t read_data;
    if (i2c_reg_read_byte(i2c_dev, address, reg, &read_data) != 0)
    {
        LOG_ERR("Error on i2c_read()");
    }
    else
    {
        //LOG_DBG("i2c_read: no error\r");
    }
    return read_data;
}

/*
 * @brief Writes value to register over I2C
 *
 * @param reg
 * @param val
 *
 */
void write_word(uint8_t address, uint8_t reg, uint16_t val)
{
    uint8_t buf[3] = {reg, val >> 8, val & 0xff};
    //printk("High byte of config register %d \n", buf[1]);
    //printk("Low byte of config register %d \n", buf[2]);
    if (i2c_write(i2c_dev, buf, 3, address) != 0)
    {
        LOG_ERR("Error on i2c_write()");
    }
    else
    {
        //LOG_DBG("i2c_write: no error");
    }
}

/*
 * @brief Reads value from register over I2C
 *
 * @param reg
 *
 * @return content of reg
 */

uint16_t read_word(uint8_t address, uint8_t reg)
{
    uint8_t read_data[2];
    if (i2c_burst_read(i2c_dev, address, reg, read_data, 2) != 0)
    {
        LOG_ERR("Error on i2c_read()");
    }
    else
    {
        //LOG_DBG("i2c_read: no error\r");
    }
    //printk("Read high byte %d \n", read_data[0]);
    //printk("Read low byte %d \n", read_data[1]);
    return read_data[0] << 8 | read_data[1];
}

void write_empty(uint8_t address, uint8_t reg) 
{
    uint8_t temp = 0;
    if (i2c_write(i2c_dev, &temp, 0, address) != 0)
    {
        LOG_ERR("Error on i2c_write()");
    }
}