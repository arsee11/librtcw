#ifndef RTP_MUXER_H
#define RTP_MUXER_H

#include "media_frame.h"
#include "media_def.h"
#include <memory>
#include <webrtc/modules/rtp_rtcp/source/rtp_packet.h>
namespace rtcgw {

using webrtc::RtpPacket;


class RtpMuxer
{
public:
    static RtpMuxer *create(const CodecParams &parmas, uint32_t ssrc);
    RtpMuxer(){}

    virtual ~RtpMuxer()=default;
    virtual void put(const MediaFrame& frame)=0;
    virtual std::vector<std::unique_ptr<RtpPacket>> get()=0;
};

}//rtcgw

#endif // RTP_MUXER_H
