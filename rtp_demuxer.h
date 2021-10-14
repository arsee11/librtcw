#ifndef RTP_DEMUXER_H
#define RTP_DEMUXER_H

#include "media_frame.h"
#include "media_def.h"
#include <memory>
#include <webrtc/modules/rtp_rtcp/source/rtp_packet.h>
namespace rtcgw {

using webrtc::RtpPacket;


class RtpDemuxer
{
public:
    static RtpDemuxer *create(const std::string& codec_name);
    RtpDemuxer(){}

    virtual ~RtpDemuxer()=default;
    virtual void put(const RtpPacket& packet)=0;
    virtual std::tuple<uint8_t *, size_t> get()=0;
};

}//rtcgw

#endif // RTP_DEMUXER_H
