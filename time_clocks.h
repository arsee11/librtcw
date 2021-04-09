#ifndef TIME_CLOCKS_H
#define TIME_CLOCKS_H

#include <cstdint>

namespace rtcgw {

class Clock
{
public:
    static uint64_t timestamp_ms();
};

}//rtcgw

#endif // TIME_CLOCKS_H
