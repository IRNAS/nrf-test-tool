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

    m_tca6424.setBankDirection(1, 85);  // write 170 to set direction 1 to red pins bank - detect device
    m_tca6424.setBankPolarity(1, 255);  // reverse polarity of all pins
    write_bank(1,170);
    // m_tca6424.setBankDirection(1,0);
    // write_bank(1,0);  // write 0 to set all low
    // m_tca6424.writePin(9, 0);
    // m_tca6424.writePin(11, 0);
    // m_tca6424.writePin(13, 0);
    // m_tca6424.writePin(15, 0);

    // something weird is going on - RED led toggles when writing command to toggle green - shouldnt be a problem, the logic seems to be reversed tho

    // GREEN LED TURNS on when red LED pin is high and green pin is LOW
    // set some pins on bank 1 as inputs, some as outputs
    // m_tca6424.setPinDirection(10,1);  // red LEDs previously
    // m_tca6424.setPinDirection(12,1);  // red LEDs previously
    // m_tca6424.setPinDirection(14,1);  // red LEDs previously
    // m_tca6424.setPinDirection(16,1);  // red LEDs previously

    // m_tca6424.setPinDirection(11,0);  // green LED
    // m_tca6424.setPinDirection(13,0);  // green LED
    // m_tca6424.setPinDirection(15,0);  // green LED
    // m_tca6424.setPinDirection(17,0);  // green LED
    // write_bank(1,0);


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

uint8_t read_bank_direction(uint8_t bank) 
{
    return m_tca6424.getBankDirection(bank);
}

uint8_t read_bank_outputs(uint8_t bank)
{
    return m_tca6424.getBankOutputLevel(bank);
}

#endif //__cplusplus
