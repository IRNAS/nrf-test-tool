#ifndef _ADS1015_INTERFACE_HPP_
#define _ADS1015_INTERFACE_HPP_

#include "ads1015.hpp"
#include <zephyr.h>

#ifdef __cplusplus
extern "C"
{
#endif

    void ads_set_gain(adsGain_t);
    adsGain_t ads_get_gain(void);

    uint16_t ads_read_ADC_single_ended(uint8_t channel);
    int16_t ads_read_ADC_differential_0_1(void);
    int16_t ads_read_ADC_differential_2_3(void);
    void ads_start_comparator_single_ended(uint8_t channel, int16_t threshold);
    int16_t ads_get_last_conversion_result();

#ifdef __cplusplus
} // extern C
#endif

#endif // _ADS1015_INTERFACE_HPP_