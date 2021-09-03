#include "rtp_demuxer.h"

#include <modules/rtp_rtcp/source/video_rtp_depacketizer.h>
#include <absl/types/optional.h>

namespace rtcgw {

RtpDemuxer::RtpDemuxer()
{

}

void RtpDemuxer::put()
{
    std::unique_ptr<webrtc::VideoRtpDepacketizer> depacketizer;
    for( auto it : _local_params.codecs){
        if(it.payload_type == packet.PayloadType()){
            webrtc::VideoCodecType vt = webrtc::PayloadStringToCodecType(it.name);
            depacketizer = webrtc::CreateVideoRtpDepacketizer(vt);
            break;
        }
    }

    if(depacketizer != nullptr){
        absl::optional<webrtc::VideoRtpDepacketizer::ParsedRtpPayload> parsed_payload =
                depacketizer->Parse(packet.PayloadBuffer());

        if (parsed_payload == std::nullopt) {
            //log error
            return;
        }

        rtc::CopyOnWriteBuffer payload = parsed_payload->video_payload;
        webrtc::RTPVideoHeader header = parsed_payload->video_header;
        memcpy(buffer+buffer_pos, payload.data(), payload.size());
        buffer_pos += payload.size();

        if(packet.Marker()){
            //fwrite(buffer, 1, buffer_pos, fp);
            buffer_pos = 0;
        }
    }
}

MediaFrame RtpDemuxer::get()
{

}




}//rtcgw
