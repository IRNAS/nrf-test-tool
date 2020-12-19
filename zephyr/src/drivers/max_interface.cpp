#ifdef __cplusplus

#include "max_interface.hpp"

#include <zephyr.h>

#include "max14661.hpp"

// tca driver class instance
MAX14661 m_max14661; //= MAX14661(0x77);

void clear_all() 
{
    m_max14661.clearAll();
}

void set_A(uint8_t swA0, uint8_t swA1)
{
    m_max14661.setA(swA0, swA1);
}

void set_B(uint8_t swB0, uint8_t swB1)
{
    m_max14661.setB(swB0, swB1);
}

uint8_t read_dir0()
{
    return m_max14661.readDir0();
}

uint8_t read_dir1()
{
    return m_max14661.readDir1();
}

uint8_t read_dir2()
{
    return m_max14661.readDir2();
}

uint8_t read_dir3()
{   
    return m_max14661.readDir3();
}

#endif //__cplusplus
