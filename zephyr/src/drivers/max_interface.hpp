#ifndef _MAX_INTERFACE_HPP_
#define _MAX_INTERFACE_HPP_

#include <zephyr.h>

#ifdef __cplusplus
extern "C"
{
#endif

    void max_clear_all(void);

    void max_set_A_switches(uint16_t switch_config);
    void max_set_B_switches(uint16_t switch_config);
    void max_set_A(uint8_t swA0, uint8_t swA1);
    void max_set_B(uint8_t swB0, uint8_t swB1);

    uint8_t max_read_dir0();
    uint8_t max_read_dir1();
    uint8_t max_read_dir2();
    uint8_t max_read_dir3();

#ifdef __cplusplus
} // extern C
#endif

#endif // _MAX_INTERFACE_HPP_