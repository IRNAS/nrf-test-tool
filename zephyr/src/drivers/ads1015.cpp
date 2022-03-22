/**************************************************************************/
/*!
    @file     ADS1015.cpp
    @author   K.Townsend (Adafruit Industries)
    @mainpage Adafruit ADS1X15 ADC Breakout Driver
    @section intro_sec Introduction
    This is a library for the Adafruit ADS1X15 ADC breakout boards.
    Adafruit invests time and resources providing this open source code,
    please support Adafruit and open-source hardware by purchasing
    products from Adafruit!
    @section author Author
    Written by Kevin "KTOWN" Townsend for Adafruit Industries.
    @section  HISTORY
    v1.0  - First release
    v1.1  - Added ADS1115 support - W. Earl
    @section license License
    BSD license, all text here must be included in any redistribution

    Reference: https://github.com/adafruit/Adafruit_ADS1X15
*/
/**************************************************************************/

#include "ads1015.hpp"
#include <zephyr.h>

#include "i2c_funs.h"

/**************************************************************************/
/*!
    @brief  Instantiates a new ADS1015 class w/appropriate properties
    @param i2cAddress I2C address of device
*/
/**************************************************************************/
ADS1015::ADS1015(uint8_t i2cAddress) {
    m_i2cAddress = i2cAddress;
    m_conversionDelay = ADS1015_CONVERSIONDELAY;
    m_bitShift = 4;
    m_gain = GAIN_TWOTHIRDS; /* +/- 6.144V range (limited to VDD +0.3V max!) */
}

/**************************************************************************/
/*!
    @brief  Sets the gain and input voltage range
    @param gain gain setting to use
*/
/**************************************************************************/
void ADS1015::setGain(uint16_t gain) { m_gain = gain; }

/**************************************************************************/
/*!
    @brief  Gets a gain and input voltage range
    @return the gain setting
*/
/**************************************************************************/
uint16_t ADS1015::getGain() { return m_gain; }

/**************************************************************************/
/*!
    @brief  Gets a single-ended ADC reading from the specified channel
    @param channel ADC channel to read
    @return the ADC reading
*/
/**************************************************************************/
uint16_t ADS1015::readADC_SingleEnded(uint8_t channel) {
    if (channel > 3) 
    {
        return 0;
    }

    // Start with default values
    uint16_t config =
        ADS1015_REG_CONFIG_CQUE_NONE |    // Disable the comparator (default val)
        ADS1015_REG_CONFIG_CLAT_NONLAT |  // Non-latching (default val)
        ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
        ADS1015_REG_CONFIG_CMODE_TRAD |   // Traditional comparator (default val)
        ADS1015_REG_CONFIG_DR_1600SPS |   // 1600 samples per second (default)
        ADS1015_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)
        //ADS1015_REG_CONFIG_MODE_CONTIN;

    // Set PGA/voltage range
    config |= m_gain;

    // Set single-ended input channel
    switch (channel) 
    {
        case (0):
            config |= ADS1015_REG_CONFIG_MUX_SINGLE_0;
            break;
        case (1):
            config |= ADS1015_REG_CONFIG_MUX_SINGLE_1;
            break;
        case (2):
            config |= ADS1015_REG_CONFIG_MUX_SINGLE_2;
            break;
        case (3):
            config |= ADS1015_REG_CONFIG_MUX_SINGLE_3;
            break;
    }

    // Set 'start single-conversion' bit
    config |= ADS1015_REG_CONFIG_OS_SINGLE;

    //printk("Set config register %d \n", config);

    // Write config register to the ADC
    //writeRegister(m_i2cAddress, ADS1015_REG_POINTER_CONFIG, config);
    write_word(m_i2cAddress, ADS1015_REG_POINTER_CONFIG, config);

    // Wait for the conversion to complete
    //delay(m_conversionDelay);
    k_sleep(K_MSEC(ADS1015_CONVERSIONDELAY));

    // uint16_t config_register = read_word(m_i2cAddress, ADS1015_REG_POINTER_CONFIG);
    // printk("Read config register %d \n", config_register);


    // write_empty(m_i2cAddress, ADS1015_REG_POINTER_CONVERT);
    // Read the conversion results
    // Shift 12-bit results right 4 bits for the ADS1015
    //return readRegister(m_i2cAddress, ADS1015_REG_POINTER_CONVERT) >> m_bitShift;
    uint16_t result = read_word(m_i2cAddress, ADS1015_REG_POINTER_CONVERT);
    //printk("Conversion result %d \n", result);
    return result >> m_bitShift;
}

/**************************************************************************/
/*!
    @brief  Reads the conversion results, measuring the voltage
            difference between the P (AIN0) and N (AIN1) input.  Generates
            a signed value since the difference can be either
            positive or negative.
    @return the ADC reading
*/
/**************************************************************************/
int16_t ADS1015::readADC_Differential_0_1() {
    // Start with default values
    uint16_t config =
        ADS1015_REG_CONFIG_CQUE_NONE |    // Disable the comparator (default val)
        ADS1015_REG_CONFIG_CLAT_NONLAT |  // Non-latching (default val)
        ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
        ADS1015_REG_CONFIG_CMODE_TRAD |   // Traditional comparator (default val)
        ADS1015_REG_CONFIG_DR_1600SPS |   // 1600 samples per second (default)
        ADS1015_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)

    // Set PGA/voltage range
    config |= m_gain;

    // Set channels
    config |= ADS1015_REG_CONFIG_MUX_DIFF_0_1; // AIN0 = P, AIN1 = N

    // Set 'start single-conversion' bit
    config |= ADS1015_REG_CONFIG_OS_SINGLE;

    // Write config register to the ADC
    //writeRegister(m_i2cAddress, ADS1015_REG_POINTER_CONFIG, config);
    write_reg(m_i2cAddress, ADS1015_REG_POINTER_CONFIG, config);

    // Wait for the conversion to complete
    //delay(m_conversionDelay);
    k_sleep(K_MSEC(ADS1015_CONVERSIONDELAY));

    // Read the conversion results
    //uint16_t res = readRegister(m_i2cAddress, ADS1015_REG_POINTER_CONVERT) >> m_bitShift;
    uint16_t res = read_reg(m_i2cAddress, ADS1015_REG_POINTER_CONVERT) >> m_bitShift;
    if (m_bitShift == 0) 
    {
        return (int16_t)res;
    } 
    else 
    {
        // Shift 12-bit results right 4 bits for the ADS1015,
        // making sure we keep the sign bit intact
        if (res > 0x07FF)
        {
            // negative number - extend the sign to 16th bit
            res |= 0xF000;
        }
    return (int16_t)res;
    }
}

/**************************************************************************/
/*!
    @brief  Reads the conversion results, measuring the voltage
            difference between the P (AIN2) and N (AIN3) input.  Generates
            a signed value since the difference can be either
            positive or negative.
    @return the ADC reading
*/
/**************************************************************************/
int16_t ADS1015::readADC_Differential_2_3() {
    // Start with default values
    uint16_t config =
        ADS1015_REG_CONFIG_CQUE_NONE |    // Disable the comparator (default val)
        ADS1015_REG_CONFIG_CLAT_NONLAT |  // Non-latching (default val)
        ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
        ADS1015_REG_CONFIG_CMODE_TRAD |   // Traditional comparator (default val)
        ADS1015_REG_CONFIG_DR_1600SPS |   // 1600 samples per second (default)
        ADS1015_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)

    // Set PGA/voltage range
    config |= m_gain;

    // Set channels
    config |= ADS1015_REG_CONFIG_MUX_DIFF_2_3; // AIN2 = P, AIN3 = N

    // Set 'start single-conversion' bit
    config |= ADS1015_REG_CONFIG_OS_SINGLE;

    // Write config register to the ADC
    //writeRegister(m_i2cAddress, ADS1015_REG_POINTER_CONFIG, config);
    write_reg(m_i2cAddress, ADS1015_REG_POINTER_CONFIG, config);

    // Wait for the conversion to complete
    //delay(m_conversionDelay);
    k_sleep(K_MSEC(ADS1015_CONVERSIONDELAY));

    // Read the conversion results
    //uint16_t res = readRegister(m_i2cAddress, ADS1015_REG_POINTER_CONVERT) >> m_bitShift;
    uint16_t res = read_reg(m_i2cAddress, ADS1015_REG_POINTER_CONVERT) >> m_bitShift;
    if (m_bitShift == 0) 
    {
        return (int16_t)res;
    } 
    else 
    {
        // Shift 12-bit results right 4 bits for the ADS1015,
        // making sure we keep the sign bit intact
        if (res > 0x07FF) 
        {
            // negative number - extend the sign to 16th bit
            res |= 0xF000;
        }
    return (int16_t)res;
    }
}

/**************************************************************************/
/*!
    @brief  Sets up the comparator to operate in basic mode, causing the
            ALERT/RDY pin to assert (go from high to low) when the ADC
            value exceeds the specified threshold.
            This will also set the ADC in continuous conversion mode.
    @param channel ADC channel to use
    @param threshold comparator threshold
*/
/**************************************************************************/
void ADS1015::startComparator_SingleEnded(uint8_t channel, int16_t threshold) {
    // Start with default values
    uint16_t config =
        ADS1015_REG_CONFIG_CQUE_1CONV |   // Comparator enabled and asserts on 1
                                        // match
        ADS1015_REG_CONFIG_CLAT_LATCH |   // Latching mode
        ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
        ADS1015_REG_CONFIG_CMODE_TRAD |   // Traditional comparator (default val)
        ADS1015_REG_CONFIG_DR_1600SPS |   // 1600 samples per second (default)
        ADS1015_REG_CONFIG_MODE_CONTIN |  // Continuous conversion mode
        ADS1015_REG_CONFIG_MODE_CONTIN;   // Continuous conversion mode

    // Set PGA/voltage range
    config |= m_gain;

    // Set single-ended input channel
    switch (channel) 
    {
        case (0):
            config |= ADS1015_REG_CONFIG_MUX_SINGLE_0;
            break;
        case (1):
            config |= ADS1015_REG_CONFIG_MUX_SINGLE_1;
            break;
        case (2):
            config |= ADS1015_REG_CONFIG_MUX_SINGLE_2;
            break;
        case (3):
            config |= ADS1015_REG_CONFIG_MUX_SINGLE_3;
            break;
    }

    // Set the high threshold register
    // Shift 12-bit results left 4 bits for the ADS1015
    //writeRegister(m_i2cAddress, ADS1015_REG_POINTER_HITHRESH, threshold << m_bitShift);
    write_reg(m_i2cAddress, ADS1015_REG_POINTER_HITHRESH, threshold << m_bitShift);

    // Write config register to the ADC
    //writeRegister(m_i2cAddress, ADS1015_REG_POINTER_CONFIG, config);
    write_reg(m_i2cAddress, ADS1015_REG_POINTER_CONFIG, config);
}

/**************************************************************************/
/*!
    @brief  In order to clear the comparator, we need to read the
            conversion results.  This function reads the last conversion
            results without changing the config value.
    @return the last ADC reading
*/
/**************************************************************************/
int16_t ADS1015::getLastConversionResults() {
    // Wait for the conversion to complete
    //delay(m_conversionDelay);
    k_sleep(K_MSEC(ADS1015_CONVERSIONDELAY));

    // Read the conversion results
    //uint16_t res = readRegister(m_i2cAddress, ADS1015_REG_POINTER_CONVERT) >> m_bitShift;
    uint16_t res = read_reg(m_i2cAddress, ADS1015_REG_POINTER_CONVERT) >> m_bitShift;
    if (m_bitShift == 0) 
    {
        return (int16_t)res;
    }
    else 
    {
        // Shift 12-bit results right 4 bits for the ADS1015,
        // making sure we keep the sign bit intact
        if (res > 0x07FF) 
        {
            // negative number - extend the sign to 16th bit
            res |= 0xF000;
        }
        return (int16_t)res;
    }
}