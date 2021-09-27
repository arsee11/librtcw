#include "rtp_demuxer.h"

#include <modules/rtp_rtcp/source/video_rtp_depacketizer.h>
#include <modules/rtp_rtcp/source/create_video_rtp_depacketizer.h>
#include <api/video_codecs/video_codec.h>
#include <absl/types/optional.h>

#include <iostream>

using  std::cout;
using std::endl;

namespace rtcgw {

class RtpDemuxerImpl : public RtpDemuxer
{
public:
    RtpDemuxerImpl(const std::string& codec_name);

    // RtpDemuxer interface
public:
    void put(const RtpPacket& packet)override;
    MediaFrame get()override;

private:
    std::string _codec_name;
    std::unique_ptr<webrtc::VideoRtpDepacketizer> _depacketizer;
    rtc::CopyOnWriteBuffer _frame;
};

RtpDemuxerImpl::RtpDemuxerImpl(const std::string &codec_name)
{
    _codec_name = codec_name;
    webrtc::VideoCodecType vt = webrtc::PayloadStringToCodecType(codec_name);
    _depacketizer = webrtc::CreateVideoRtpDepacketizer(vt);
}



void RtpDemuxerImpl::put(const webrtc::RtpPacket &packet)
{
    if(_depacketizer != nullptr){
        absl::optional<webrtc::VideoRtpDepacketizer::ParsedRtpPayload> parsed_payload =
                _depacketizer->Parse(packet.PayloadBuffer());

        if (parsed_payload == std::nullopt) {
            //log error
            _frame.Clear();
            return;
        }
        if(parsed_payload->video_header.codec = webrtc::VideoCodecType::kVideoCodecVP9){
            webrtc::RTPVideoHeaderVP9 vp9header = std::get<webrtc::RTPVideoHeaderVP9>(parsed_payload->video_header.video_type_header);
            cout<<"is end of frame:"<<vp9header.end_of_frame<<endl;

        }
        _frame = parsed_payload->video_payload;
        //webrtc::RTPVideoHeader header = parsed_payload->video_header;
    }
}

MediaFrame RtpDemuxerImpl::get()
{
    MediaFrame frame;
    frame.data = _frame.data();
    frame.size = _frame.size();
    return frame;
}


RtpDemuxer* RtpDemuxer::create(const std::string &codec_name)
{
    return new RtpDemuxerImpl(codec_name);
}


}//rtcgw
