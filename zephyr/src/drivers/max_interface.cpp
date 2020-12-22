#ifdef __cplusplus

#include "max_interface.hpp"

#include <zephyr.h>

#include "max14661.hpp"

// tca driver class instance
MAX14661 m_max14661;

void max_set_A_switches(uint16_t switch_config)
{
    uint8_t swA0 = switch_config & 0xFF;  //lower byte configures A08-A01
    uint8_t swA1 = (switch_config & 0xFF00) >> 8;  //upper byte configures A16-A09
    max_set_A(swA0, swA1);      // control is done using shadow registers - this allows for simultaneous updates of the switches
}

void max_set_B_switches(uint16_t switch_config)
{
    uint8_t swB0 = switch_config & 0xFF;  //lower byte configures B08-B01
    uint8_t swB1 = (switch_config & 0xFF00) >> 8;  //upper byte configures B16-B09
    max_set_B(swB0, swB1);
}

void max_clear_all() 
{
    m_max14661.clearAll();
}

void max_set_A(uint8_t swA0, uint8_t swA1)
{
    m_max14661.setA(swA0, swA1);
}

void max_set_B(uint8_t swB0, uint8_t swB1)
{
    m_max14661.setB(swB0, swB1);
}

uint8_t max_read_dir0() // read DIR0 which holds A08-A01
{
    return m_max14661.readDir0();
}

uint8_t max_read_dir1() // read DIR1 which holds A16-A09
{
    return m_max14661.readDir1();
}

uint8_t max_read_dir2() // read DIR2 which holds B08-B01
{
    return m_max14661.readDir2();
}

uint8_t max_read_dir3() // read DIR3 which holds B16-B09
{   
    return m_max14661.readDir3();
}

#endif //__cplusplus
