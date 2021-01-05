#ifdef __cplusplus

#include "ads1015_interface.hpp"

#include <zephyr.h>
#include "ads1015.hpp"

// tca driver class instance
ADS1015 m_ADS1015[4];

// init all four ADS1015 chips
void ads_init()
{
    m_ADS1015[0] = ADS1015(0x48);
    m_ADS1015[1] = ADS1015(0x49);
    m_ADS1015[2] = ADS1015(0x4a);
    m_ADS1015[3] = ADS1015(0x4b);
}

void ads_set_gain(uint8_t target, adsGain_t gain) 
{
    m_ADS1015[target].setGain(gain);
}

adsGain_t ads_get_gain(uint8_t target) 
{
    return m_ADS1015[target].getGain();
}

uint16_t ads_read_ADC_single_ended(uint8_t target, uint8_t channel)
{
    return m_ADS1015[target].readADC_SingleEnded(channel);
}

int16_t ads_read_ADC_differential_0_1(uint8_t target)
{
    return m_ADS1015[target].readADC_Differential_0_1();
}

int16_t ads_read_ADC_differential_2_3(uint8_t target)
{
    return m_ADS1015[target].readADC_Differential_2_3();
}

void ads_start_comparator_single_ended(uint8_t target, uint8_t channel, int16_t threshold)
{
    m_ADS1015[target].startComparator_SingleEnded(channel, threshold);
}

int16_t ads_get_last_conversion_result(uint8_t target)
{
    return m_ADS1015[target].getLastConversionResults();
}

#endif //__cplusplus
