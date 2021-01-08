#ifndef _MAX_INTERFACE_HPP_
#define _MAX_INTERFACE_HPP_

#include <zephyr.h>

#ifdef __cplusplus
extern "C"
{
#endif

    int8_t max_clear_all(void);

    int8_t max_set_A_switches(uint16_t switch_config);
    int8_t max_set_B_switches(uint16_t switch_config);
    int8_t max_set_A(uint8_t swA0, uint8_t swA1);
    int8_t max_set_B(uint8_t swB0, uint8_t swB1);

    int8_t max_read_dir0();
    int8_t max_read_dir1();
    int8_t max_read_dir2();
    int8_t max_read_dir3();

#ifdef __cplusplus
} // extern C
#endif

#endif // _MAX_INTERFACE_HPP_