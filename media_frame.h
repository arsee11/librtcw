#ifndef MEDIA_FRAME_H
#define MEDIA_FRAME_H

#include <cstdint>

namespace rtcgw {

class MediaFrame
{
public:
    union{
        struct{
            int w;
            int h;
        };
        struct{
            int sr;
            int chns;
        };
    };
    uint64_t timestamp_ms;
    int size;
    void* data;
};


}

#endif // MEDIA_FRAME_H
