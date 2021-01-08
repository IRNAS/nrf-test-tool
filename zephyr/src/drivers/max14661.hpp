/******************************************************************//**
* @file max14661.h
*
* @author Justin Jordan
*
* @version 1.0
*
* Started: 11NOV14
*
* Updated: 
*
* @brief Header file for MAX14661 class
*
***********************************************************************
*
* @copyright 
* Copyright (C) 2015 Maxim Integrated Products, Inc., All Rights Reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
* OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*
* Except as contained in this notice, the name of Maxim Integrated
* Products, Inc. shall not be used except as stated in the Maxim Integrated
* Products, Inc. Branding Policy.
*
* The mere transfer of this software does not imply any licenses
* of trade secrets, proprietary technology, copyrights, patents,
* trademarks, maskwork rights, or any other form of intellectual
* property whatsoever. Maxim Integrated Products, Inc. retains all
* ownership rights.
**********************************************************************/
#ifndef MAX14661_H
#define MAX14661_H
 
#include <zephyr.h>
 
#define MAX14661_DEFAULT_ADDRESS  0x4C

/** MAX14661 Library, Provides utilities for configuring the MAX14661
over I2C
*
* Example:
* @code
* // Enable only switch B3 and read back switch state.
*
* #include "MAX14661.h"
*
* MAX14661 mux(p28, p27);
*
* int main() {
* mux.setAB(0x0000, MAX14661::SW03);
* printf("mux = 0x%08X\n", mux.read());
* mux.clearAll();
* printf("mux = 0x%08X\n", mux.read());
* }
* @endcode
*/
class MAX14661
{
    public:
        /** Create a MAX14661 interface
         *
         * @param addr MAX14661 I2C address
         */
        MAX14661(int addr = MAX14661_DEFAULT_ADDRESS);
        ~MAX14661();

        /** Name the register addresses*/
        enum MAX14661regs {
            REG_DIR0 = 0x00, /**< 8A-1A Direct Access */
            REG_DIR1, /**< 16A-9A Direct Access */
            REG_DIR2, /**< 8B-1B Direct Access */
            REG_DIR3, /**< 16B-9B Direct Access */
            REG_SHDW0 = 0x10, /**< 8A-1A Shadow */
            REG_SHDW1, /**< 16A-9A Shadow */
            REG_SHDW2, /**< 8B-1B Shadow */
            REG_SHDW3, /**< 16B-9B Shadow */
            REG_CMD_A, /**< Command A */
            REG_CMD_B /**< Command A */
        };

        /** Name the command codes*/
        enum MAX14661cmds {
        CMD_DIS = 0x10, /**< Disable switches */
        CMD_COPY = 0x11, /**< Copy shadow registers to switches */
        CMD_NOOP = 0x1F /**< Keep current state, no changes */
        };

        /* Clears all bits to opens all 32 switches */
        int8_t clearAll();

        /** Set A switches simultaneously
         *
         * @param swA0 the desired state of switches [A09 - A01]
         * @param swA1 the desired state of switches [A16 - A10]
         */
        int8_t setA(uint8_t swA0, uint8_t swA1);

        /** Set B switches simultaneously
         *
         * @param swB0 the desired state of switches [B09 - B01]
         * @param swB1 the desired state of switches [B16 - B10]
         */
        int8_t setB(uint8_t swB0, uint8_t swB1);

        /** Read the status of all A08-A01 switches concatenated into a uint8_t
        *
        * @returns
        * the switch states [A08-A01]
        */
        int8_t readDir0();

        /** Read the status of all A16-A09 switches concatenated into a uint8_t
        *
        * @returns
        * the switch states [A16-A09]
        */
        int8_t readDir1();

        /** Read the status of all B08-B01 switches concatenated into a uint8_t
        *
        * @returns
        * the switch states [B08-B01]
        */
        int8_t readDir2();

        /** Read the status of all B16-B09 switches concatenated into a uint8_t
        *
        * @returns
        * the switch states [B16-B09]
        */
        int8_t readDir3();

    private:
        int _addr;
};
#endif /* MAX14661_H*/