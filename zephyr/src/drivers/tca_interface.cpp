#ifdef __cplusplus

#include "tca_interface.hpp"

#include <zephyr.h>
#include "tca6424.hpp"

// tca driver class instance
TCA6424A m_tca6424 = TCA6424A(0x22);

void tca_init(void) 
{
    // make banks 0, 2 output
    m_tca6424.setBankDirection(0,0);
    // set all channels to 0
    write_bank(0,0);

    m_tca6424.setBankDirection(2,0);
    write_bank(2,0);

    m_tca6424.setBankDirection(1,0);
    write_bank(1,0);

    // this works for red and green LEDs
    m_tca6424.setBankDirection(1, 85);  // this works for LED outputs - set even pins as inputs - 1 is input, 0 is output
    write_bank(1, 170);
}

uint8_t test_connection(void) 
{
    return m_tca6424.testConnection();
}

void write_pin(uint16_t pin, uint8_t value)
{
    m_tca6424.writePin(pin, value);
}

void write_pin_inverse(uint16_t pin, uint8_t value)
{
    m_tca6424.writePinInverse(pin, value);
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

uint8_t read_bank_direction(uint8_t bank) 
{
    return m_tca6424.getBankDirection(bank);
}

uint8_t read_bank_outputs(uint8_t bank)
{
    return m_tca6424.getBankOutputLevel(bank);
}

#endif //__cplusplus
