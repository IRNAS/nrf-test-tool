#ifdef __cplusplus

#include "ads1015_interface.hpp"

#include <zephyr.h>
#include "ads1015.hpp"

// tca driver class instance
ADS1015 m_ADS1015;

void ads_set_gain(adsGain_t gain) 
{
    m_ADS1015.setGain(gain);
}

adsGain_t ads_get_gain() 
{
    return m_ADS1015.getGain();
}

uint16_t ads_read_ADC_single_ended(uint8_t channel)
{
    return m_ADS1015.readADC_SingleEnded(channel);
}

int16_t ads_read_ADC_differential_0_1()
{
    return m_ADS1015.readADC_Differential_0_1();
}

int16_t ads_read_ADC_differential_2_3()
{
    return m_ADS1015.readADC_Differential_2_3();
}

void ads_start_comparator_single_ended(uint8_t channel, int16_t threshold)
{
    m_ADS1015.startComparator_SingleEnded(channel, threshold);
}

int16_t ads_get_last_conversion_result()
{
    return m_ADS1015.getLastConversionResults();
}

#endif //__cplusplus
