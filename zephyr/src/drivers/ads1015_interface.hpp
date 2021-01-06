#ifndef _ADS1015_INTERFACE_HPP_
#define _ADS1015_INTERFACE_HPP_
#include <zephyr.h>

#ifdef __cplusplus

extern "C"
{
#endif

    void ads_init(void);

    void ads_set_gain(uint8_t target, uint16_t gain);
    uint16_t ads_get_gain(uint8_t target);

    uint16_t ads_read_ADC_single_ended(uint8_t target, uint8_t channel);
    int16_t ads_read_ADC_differential_0_1(uint8_t target);
    int16_t ads_read_ADC_differential_2_3(uint8_t target);
    void ads_start_comparator_single_ended(uint8_t target, uint8_t channel, int16_t threshold);
    int16_t ads_get_last_conversion_result(uint8_t target);
    uint16_t ads_convert_to_analog(uint8_t target, uint16_t digital_value);

#ifdef __cplusplus
} // extern C
#endif

#endif // _ADS1015_INTERFACE_HPP_