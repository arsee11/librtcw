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
    std::tuple<uint8_t *, size_t> get()override;

private:
    std::string _codec_name;
    std::unique_ptr<webrtc::VideoRtpDepacketizer> _depacketizer;
    struct FrameBuffer{
        void clear(){
            fragments.clear();
            is_completed = false;
        }
        size_t size()const{
            size_t s = 0;
            for(auto f : fragments){
                s += f.size();
            }
            return s;
        }
        std::vector<rtc::CopyOnWriteBuffer> fragments;
        bool is_completed=false;
    } _buffer;
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
            return;
        }
        if(_buffer.is_completed){ //it means a new frame is comming
            _buffer.clear();
        }

        _buffer.fragments.push_back(parsed_payload->video_payload);
//        if(parsed_payload->video_header.codec = webrtc::VideoCodecType::kVideoCodecVP9){
//            webrtc::RTPVideoHeaderVP9 vp9header = std::get<webrtc::RTPVideoHeaderVP9>(parsed_payload->video_header.video_type_header);
//            if(vp9header.end_of_frame){
//                _buffer.is_completed = true;
//            }
//        }
        if(parsed_payload->video_header.codec = webrtc::VideoCodecType::kVideoCodecVP8){
            webrtc::RTPVideoHeaderVP8 vp8header = std::get<webrtc::RTPVideoHeaderVP8>(parsed_payload->video_header.video_type_header);
                _buffer.is_completed = packet.Marker();
        }
        else{
            cout<<"[RtpDemuxer] not support codec:"<<parsed_payload->video_header.codec<<endl;
        }
    }
}

std::tuple<uint8_t*, size_t> RtpDemuxerImpl::get()
{
    uint8_t* frame=nullptr;
    size_t size = 0;
    if(_buffer.is_completed){
        frame = new uint8_t[_buffer.size()];
        for(auto f : _buffer.fragments){
            memcpy(frame+size, f.data(), f.size());
            size += f.size();
        }
    }
    return std::make_tuple(frame, size);
}


RtpDemuxer* RtpDemuxer::create(const std::string &codec_name)
{
    return new RtpDemuxerImpl(codec_name);
}


}//rtcgw
