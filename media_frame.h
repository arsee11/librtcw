#ifndef MEDIA_FRAME_H
#define MEDIA_FRAME_H

#include <cstdint>
#include <utility>
#include <tuple>

namespace rtcgw {

class MediaFrame
{
public:
    union{
        struct{
            int w;
            int h;
            int fps;
        };
        struct{
            int sr;
            int chns;
        };
    };
    int type; ////0-video, 1-audio
    bool is_key_frame;
    uint64_t timestamp_ms;
    int size=0;
    uint8_t* data=nullptr;

    MediaFrame()=default;
    MediaFrame& operator=(const MediaFrame& o)=delete;
    MediaFrame(const MediaFrame& o)=delete;
    MediaFrame& operator=(MediaFrame&& o){
        if(data != nullptr){
            delete[] data;
            data = nullptr;
            size = 0;
        }
        data = o.data;
        size = o.size;
        o.data = nullptr;
        o.size = 0;
    }
    MediaFrame(MediaFrame&& o){
        *this = std::move(o);
    }

    std::tuple<uint8_t*, int> release(){
        uint8_t* tmpd = data;
        int tmps = size;
        data = nullptr;
        size = 0;
        return std::make_tuple(tmpd, tmps);
    }
};


}

#endif // MEDIA_FRAME_H
