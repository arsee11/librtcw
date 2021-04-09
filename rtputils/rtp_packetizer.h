//rtp_packetizer.h

#ifndef RTP_PACKETIZER_H
#define RTP_PACKETIZER_H

#include <cstdint>
#include <string>
#include <vector>
#include "rtp_defines.h"
#include "media_frame.h"

namespace rtcgw {

class RtpPacketizer
{
public:
    static RtpPacketizer* create(const std::string& name, int payload_type);
    virtual std::vector<std::unique_ptr<RtpPacketToSend>> buildPackets(const MediaFrame& frame)=0;

};

}//rtcgw

#endif /*RTP_PACKETIZER_H*/
