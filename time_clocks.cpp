#include "time_clocks.h"

#include <chrono>

namespace rtcgw {

using SystemClock = std::chrono::system_clock;

uint64_t Clock::timestamp_ms()
{
    auto now = SystemClock::now();
    auto d = now.time_since_epoch();

    return std::chrono::duration_cast<std::chrono::milliseconds>(d).count();
}


}//rtcgw
