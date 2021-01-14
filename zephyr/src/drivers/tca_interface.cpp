#ifdef __cplusplus

#include "tca_interface.hpp"

#include <zephyr.h>
#include "tca6424.hpp"

// tca driver class instance
TCA6424A m_tca6424 = TCA6424A(0x22);

int8_t tca_init(void) 
{
    // make all channels output
    uint8_t ret = m_tca6424.setBankDirection(0,0);
    if (ret == -1)
    {
        return -1;
    }
    ret = m_tca6424.setBankDirection(1,0);
    if (ret == -1)
    {
        return -1;
    }
    ret = m_tca6424.setBankDirection(2,0);
    if (ret == -1)
    {
        return -1;
    }

    // set all channels to 0
    ret = write_bank(0,0);
    if (ret == -1)
    {
        return -1;
    }
    ret = write_bank(1,0);
    if (ret == -1)
    {
        return -1;
    }
    ret = write_bank(2,0);
    if (ret == -1)
    {
        return -1;
    }

    return 0;
}

int8_t test_connection(void) 
{
    return m_tca6424.testConnection();
}

int8_t write_pin(uint16_t pin, uint8_t value)
{
    return m_tca6424.writePin(pin, value);
}

int8_t write_bank(uint8_t bank, uint8_t value) 
{
    return m_tca6424.writeBank(bank, value);
}

int8_t read_pin(uint16_t pin) 
{
    return m_tca6424.readPin(pin);
}

int8_t read_bank(uint8_t bank) 
{
    return m_tca6424.readBank(bank);
}

#endif //__cplusplus
