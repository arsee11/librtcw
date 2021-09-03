#ifndef RTP_DEMUXER_H
#define RTP_DEMUXER_H

#include "media_frame.h"
#include <memory>

namespace rtcgw {


class RtpDemuxer
{
public:
    RtpDemuxer();
    void put();
    MediaFrame get();
};

}//rtcgw

#endif // RTP_DEMUXER_H
