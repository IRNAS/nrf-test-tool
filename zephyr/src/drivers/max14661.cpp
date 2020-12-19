/******************************************************************//**
    https://www.mouser.com/datasheet/2/256/maxrefdes72-quick-start-guide-587807.pdf
**********************************************************************/
 
 
#include "max14661.hpp"
 
#include <zephyr.h>
#include <logging/log.h>

#include "peripherals.h"

LOG_MODULE_REGISTER(MAX14661);

MAX14661::MAX14661(int addr)
{
    _addr = addr;
}

MAX14661::~MAX14661()
{

}

void MAX14661::clearAll()
{
    write_reg(_addr, REG_CMD_A, CMD_DIS);
    write_reg(_addr, REG_CMD_B, CMD_DIS);
}

void MAX14661::setA(uint8_t swA0, uint8_t swA1)
{
    write_reg(_addr, REG_SHDW0, swA0);  // set SHDW0 register
    write_reg(_addr, REG_SHDW1, swA1);  // set SHDW1 register

    write_reg(_addr, REG_CMD_A, CMD_COPY);  // copy A shadows registers (SHDW0 and SHDW1) to switches
}

void MAX14661::setB(uint8_t swB0, uint8_t swB1)
{
    write_reg(_addr, REG_SHDW2, swB0);  // set SHDW2 register
    write_reg(_addr, REG_SHDW3, swB1);  // set SHDW2 register

    write_reg(_addr, REG_CMD_B, CMD_COPY);  // copy B shadow registers (SHDW2 and SHDW3) to switches
}

uint8_t MAX14661::readDir0()
{
    uint8_t dir0_state = read_reg(_addr, REG_DIR0);
    return dir0_state;
}

uint8_t MAX14661::readDir1()
{
    uint8_t dir1_state = read_reg(_addr, REG_DIR1);
    return dir1_state;
}

uint8_t MAX14661::readDir2()
{
    uint8_t dir2_state = read_reg(_addr, REG_DIR2);
    return dir2_state;
}

uint8_t MAX14661::readDir3()
{
    uint8_t dir3_state = read_reg(_addr, REG_DIR3);
    return dir3_state;
}