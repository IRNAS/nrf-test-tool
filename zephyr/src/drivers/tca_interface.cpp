#ifdef __cplusplus

#include "tca_interface.hpp"

#include <zephyr.h>

#include "tca6424.hpp"

// tca driver class instance
TCA6424A m_tca6424; //= TCA6424A(0x49);

int test_connection(void) 
{
    return m_tca6424.testConnection();
}

#endif //__cplusplus
