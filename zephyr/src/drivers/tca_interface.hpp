#include <zephyr.h>

#ifndef _TCA_INTERFACE_HPP_
#define _TCA_INTERFACE_HPP_

#ifdef __cplusplus
extern "C"
{
#endif

    int8_t test_connection(void);
    int8_t tca_init(void);
    int8_t write_pin(uint16_t pin, uint8_t value);
    int8_t write_bank(uint8_t bank, uint8_t value);
    int8_t read_pin(uint16_t pin);
    int8_t read_bank(uint8_t bank);
    
#ifdef __cplusplus
} // extern C
#endif

#endif // _TCA_INTERFACE_HPP_