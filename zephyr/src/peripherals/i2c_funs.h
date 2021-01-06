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

    void write_reg(uint8_t address, uint8_t reg, uint8_t val);
    uint8_t read_reg(uint8_t address, uint8_t reg);

    void write_word(uint8_t address, uint8_t reg, uint16_t val);
    uint16_t read_word(uint8_t address, uint8_t reg);

    void write_empty(uint8_t address, uint8_t reg);

#ifdef __cplusplus
} // extern C
#endif

#endif //_I2C_FUNS_H_
