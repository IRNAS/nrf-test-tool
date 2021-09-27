#include <zephyr.h>

#ifndef _TCA_INTERFACE_HPP_
#define _TCA_INTERFACE_HPP_

#ifdef __cplusplus
extern "C"
{
#endif

    uint8_t test_connection(void);
    void tca_init(void);
    void write_pin(uint16_t pin, uint8_t value);
    void write_bank(uint8_t bank, uint8_t value);
    uint8_t read_pin(uint16_t pin);
    uint8_t read_bank(uint8_t bank);
    uint8_t read_bank_direction(uint8_t bank);
    
#ifdef __cplusplus
} // extern C
#endif

#endif // _TCA_INTERFACE_HPP_