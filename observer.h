#ifndef OBSERVER_H
#define OBSERVER_H

#include <string>
#include "rtputils/rtp_defines.h"
#include "media_frame.h"

namespace rtcgw {

class Observer
{
public:
    virtual void onSubscribeFrame(const MediaFrame& frame)=0;
    virtual void onSubscribePacket(const RtpPacket& packet)=0;
    virtual void attached_source_name(const std::string& src_name){ _attached_source_name = src_name; }

private:
    std::string _attached_source_name;
};

}

#endif // OBSERVER_H
