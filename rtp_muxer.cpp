#include "rtp_muxer.h"

#include <modules/rtp_rtcp/source/rtp_format.h>
#include <modules/rtp_rtcp/source/rtp_packet_to_send.h>
#include <api/video_codecs/video_codec.h>
#include <absl/types/optional.h>

namespace rtcgw {

class RtpMuxerImpl : public RtpMuxer
{
public:
    RtpMuxerImpl(const std::string& codec_name);

    // RtpMuxer interface
public:
    void put(const MediaFrame& frame)override;
    std::vector<std::unique_ptr<webrtc::RtpPacket> > get()override;

private:
    std::string _codec_name;
    webrtc::VideoCodecType _codec_type;
    std::unique_ptr<webrtc::RtpPacketizer> _packetizer;
};

RtpMuxerImpl::RtpMuxerImpl(const std::string &codec_name)
{
    _codec_name = codec_name;
    _codec_type = webrtc::PayloadStringToCodecType(codec_name);
}



void RtpMuxerImpl::put(const MediaFrame &frame)
{
    webrtc::RTPVideoHeader vheader;
    vheader.width = frame.w;
    vheader.height = frame.h;
    vheader.codec = _codec_type;
    vheader.frame_type = frame.is_key_frame ?
                webrtc::VideoFrameType::kVideoFrameKey :
                webrtc::VideoFrameType::kVideoFrameDelta;

    webrtc::RTPFragmentationHeader fragment;
    _packetizer = webrtc::RtpPacketizer::Create(_codec_type,
            rtc::ArrayView<const uint8_t>(static_cast<const uint8_t*>(frame.data), frame.size),
            webrtc::RtpPacketizer::PayloadSizeLimits(),
            vheader,
            &fragment);
}

std::vector<std::unique_ptr<RtpPacket>> RtpMuxerImpl::get()
{
    std::vector<std::unique_ptr<RtpPacket>> packs;
    if(_packetizer != nullptr){
        webrtc::RtpHeaderExtensionMap ext;
        bool ret = false;
        do{
            webrtc::RtpPacketToSend* pack = new webrtc::RtpPacketToSend(&ext);
            ret = _packetizer->NextPacket(pack);
            packs.push_back(std::unique_ptr<RtpPacket>(pack));
        }while(ret);
    }
    return packs;
}


RtpMuxer* RtpMuxer::create(const std::string &codec_name)
{
    return new RtpMuxerImpl(codec_name);
}


}//rtcgw
