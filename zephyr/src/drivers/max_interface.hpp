#include <zephyr.h>

#ifndef _MAX_INTERFACE_HPP_
#define _MAX_INTERFACE_HPP_

#ifdef __cplusplus
extern "C"
{
#endif

    void clear_all(void);
    void set_A(uint8_t swA0, uint8_t swA1);
    void set_B(uint8_t swB0, uint8_t swB1);
    uint8_t read_dir0();
    uint8_t read_dir1();
    uint8_t read_dir2();
    uint8_t read_dir3();

#ifdef __cplusplus
} // extern C
#endif

#endif // _MAX_INTERFACE_HPP_