#include "rtp_muxer.h"

#include <modules/rtp_rtcp/source/rtp_format.h>
#include <modules/rtp_rtcp/source/rtp_packet_to_send.h>
#include <api/video_codecs/video_codec.h>
#include <absl/types/optional.h>
#include <random>

namespace rtcgw {

class RtpMuxerImpl : public RtpMuxer
{
public:
    RtpMuxerImpl(const CodecParams& params, uint32_t ssrc);

    // RtpMuxer interface
public:
    void put(const MediaFrame& frame)override;
    std::vector<std::unique_ptr<webrtc::RtpPacket> > get()override;

private:
    CodecParams _codec_params;
    int _fps=25;
    uint32_t _ssrc=0;
    uint16_t _seqno=0;
    uint32_t _timestamp=0;
    webrtc::VideoCodecType _codec_type;
    std::unique_ptr<webrtc::RtpPacketizer> _packetizer;
    uint32_t updateTimestamp();
};

RtpMuxerImpl::RtpMuxerImpl(const CodecParams &params, uint32_t ssrc)
{
    _codec_params = params;
    _ssrc = ssrc;

    auto e = std::default_random_engine(time(nullptr));
    auto rand = std::uniform_int_distribution<uint16_t>(0, 10240);
    _seqno = rand(e);

    _codec_type = webrtc::PayloadStringToCodecType(params.name);
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
    webrtc::RTPVideoHeaderVP9 vp9header;
    vp9header.InitRTPVideoHeaderVP9();
    vheader.video_type_header = vp9header;

    webrtc::RTPFragmentationHeader fragment;
    _packetizer = webrtc::RtpPacketizer::Create(_codec_type,
            rtc::ArrayView<const uint8_t>(static_cast<const uint8_t*>(frame.data), frame.size),
            webrtc::RtpPacketizer::PayloadSizeLimits(),
            vheader,
            &fragment);

    _fps = frame.fps;
}

uint32_t RtpMuxerImpl::updateTimestamp()
{
    if(_codec_params.codec_type == CODEC_VIDEO){
        _timestamp += 90000/_fps;//90000Hz/_fps
    }
    return _timestamp;
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
            if(!ret)
                break;
            pack->SetPayloadType(_codec_params.payload_type);
            pack->SetSequenceNumber(_seqno++);
            pack->SetTimestamp(_timestamp);
            pack->SetSsrc(_ssrc);
            packs.push_back(std::unique_ptr<RtpPacket>(pack));
            if(pack->Marker()){
                updateTimestamp();
            }
        }while(true);
    }
    return packs;
}


RtpMuxer* RtpMuxer::create(const CodecParams &parmas, uint32_t ssrc)
{
    return new RtpMuxerImpl(parmas, ssrc);
}


}//rtcgw
