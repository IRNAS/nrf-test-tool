#ifndef _I2C_FUNS_H_
#define _I2C_FUNS_H_

#include <device.h>
#include <zephyr.h>

    int i2c_init(void);
    void i2c_scan(void);

#ifdef __cplusplus
extern "C"
{
#endif

    int8_t write_reg(uint8_t address, uint8_t reg, uint8_t val);
    int8_t read_reg(uint8_t address, uint8_t reg);

    int8_t write_word(uint8_t address, uint8_t reg, uint16_t val);
    int16_t read_word(uint8_t address, uint8_t reg);

    int8_t write_empty(uint8_t address, uint8_t reg);

#ifdef __cplusplus
} // extern C
#endif

#endif //_I2C_FUNS_H_
