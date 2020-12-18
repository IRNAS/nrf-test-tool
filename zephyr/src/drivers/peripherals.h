#ifndef _I2C_FUNCTIONS_H_
#define _I2C_FUNCTIONS_H_

#include <device.h>
#include <zephyr.h>

    int i2c_init(void);
    void i2c_scan(void);

#ifdef __cplusplus
extern "C"
{
#endif
    // i2c functions
    void write_reg(uint8_t address, uint8_t reg, uint8_t val);
    uint8_t read_reg(uint8_t address, uint8_t reg);

    //gpio functions
    void enable_pin(uint8_t pin);
    void disable_pin(uint8_t pin);

    int read_pin(uint8_t pin);

#ifdef __cplusplus
} // extern C
#endif

#endif //_I2C_FUNCTIONS_H_
