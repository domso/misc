#ifndef ip_pkg_h
#define ip_pkg_h


#include <iostream>
#include "ip_addr.h"

namespace network {
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // 
    template <typename IP_ADDR_TYPE>
    struct ip_pkt {
        IP_ADDR_TYPE* addr;
        char* data;
    };
   
}

#endif
