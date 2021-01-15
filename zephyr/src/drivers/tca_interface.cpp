#ifdef __cplusplus

#include "tca_interface.hpp"

#include <zephyr.h>
#include "tca6424.hpp"

// tca driver class instance
TCA6424A m_tca6424 = TCA6424A(0x22);

void tca_init(void) 
{
    // make banks 0 and 1 output
    m_tca6424.setBankDirection(0,0);
    m_tca6424.setBankDirection(1,0);

    // make bank 2 input
    m_tca6424.setBankDirection(2,1);

    // set all channels to 0
    write_bank(0,0);
    write_bank(1,0);

    //write_bank(2,1);
}

uint8_t test_connection(void) 
{
    return m_tca6424.testConnection();
}

void write_pin(uint16_t pin, uint8_t value)
{
    m_tca6424.writePin(pin, value);
}

void write_bank(uint8_t bank, uint8_t value) 
{
    m_tca6424.writeBank(bank, value);
}

uint8_t read_pin(uint16_t pin) 
{
    return m_tca6424.readPin(pin);
}

uint8_t read_bank(uint8_t bank) 
{
    return m_tca6424.readBank(bank);
}

#endif //__cplusplus
