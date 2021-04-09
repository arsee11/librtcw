///stream_factory.cpp
///

#include "stream_factory.h"
#include "video_stream.h"

namespace rtcgw {


Stream *StreamFactory::createStream(int stream_type, const std::string& stream_id, ThreadScope* thr)
{
    Stream* stream=nullptr;
    if(stream_type == STREAM_VIDEO){
        stream = new VideoStream(stream_id, thr);
    }

    return stream;
}

StreamFactory::StreamFactory()
{

}

}///rtcgw
