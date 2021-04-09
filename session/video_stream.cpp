///video_stream.cpp


#include "video_stream.h"

//webrtc includes
#include <rtc_base/helpers.h>
#include <api/video_codecs/video_codec.h>
#include <modules/rtp_rtcp/source/create_video_rtp_depacketizer.h>

using namespace std::placeholders;

namespace rtcgw{

VideoStream::VideoStream(const std::string &id, ThreadScope* thr)
    :Stream(id, thr)
    ,_recv_statist(createReceiveStatistics())
    ,_send_statist(new SendStatistics)
    ,_rtcp_handler(STREAM_VIDEO
                  ,_recv_statist.get()
                  ,_send_statist.get()
                  ,std::bind(&VideoStream::requestPacketRTX, this, _1, _2)
                  ,std::bind(&VideoStream::requestKeyFrame, this, _1)
                  ,std::bind(&VideoStream::requestSendRtcpPacket, this, _1, _2)


    )
{
}

uint8_t buffer[640*480];
int buffer_pos=0;
void VideoStream::dumpPacket(const RtpPacket& packet)
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

        if (parsed_payload == absl::nullopt) {
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

void VideoStream::onRecvRtpFromTransport(Transport *trans, const RtpPacketReceived& packet)
{
    //build packet
    if( isNormalPayload(packet.PayloadType()) ){
        pushPacket(packet);
    }
    else{
        //ToDo: proccess fec red packeet
    }

    //for test
    dumpPacket(packet);
    onSubscribePacket(packet);

    if( !packet.recovered() ){
        _recv_statist->onRtpPacket(packet);
    }
}



void VideoStream::onRecvRtcpFromTransport(Transport *trans, const RtpRtcpBuffer* packet)
{
    _rtcp_handler.onRecvRtcpPacket(packet);
    //_rtcp_receiver->getRtcpStatistics();

}

void VideoStream::setRemoteParams(const StreamInfo &params)
{
    Stream::setRemoteParams(params);
    _rtcp_handler.setRemoteSsrc( *params.ssrcs.begin() );
    if(params.is_rtcp_reduced_size){
        _rtcp_handler.setRtcpMode(RtcpMode::kReducedSize);
    }
    else{
        _rtcp_handler.setRtcpMode(RtcpMode::kCompound);
    }

    if(_transport != nullptr){
        _transport->listenRtpBySsrc(params.ssrcs, std::bind(&VideoStream::onRecvRtpFromTransport, this, _1, _2));
    }

    if(_transport != nullptr){
        _transport->listenRtcp(std::bind(&VideoStream::onRecvRtcpFromTransport, this, _1, _2));
    }
}

void VideoStream::setLocalParams(const StreamInfo &params)
{
    Stream::setLocalParams(params);
    _rtcp_handler.setLocalSsrc( *params.ssrcs.begin() );
}

 std::tuple<StreamInfo, TransportInfo> VideoStream::doNegotiation()
{
    StreamInfo sinfo;
    sinfo.stream_type = STREAM_VIDEO;
    sinfo.direction = negotiationDirection();
    sinfo.id = _remote_params.id;

    sinfo.is_rtcpmux = _local_params.is_rtcpmux;
    sinfo.ssrcs = _local_params.ssrcs;

    for(auto codec : _local_params.codecs){
        auto it = std::find(_remote_params.codecs.begin(), _remote_params.codecs.end(), codec);
        if(it != _remote_params.codecs.end() ){
            if(it->name == "VP9"){
                sinfo.codecs.push_back(codec);
            }
            //break;
        }
    }

    TransportInfo tinfo;
    if(_transport != nullptr){
        tinfo = _transport->info();
    }
    return std::make_tuple(sinfo, tinfo);
 }

 void VideoStream::onTimerEvent()
 {
    _rtcp_handler.checkAndSendRtcp();
 }

StreamDirection VideoStream::negotiationDirection()
{
    if (_local_params.direction == StreamDirection::SENDRECV){
        return StreamDirection::SENDRECV;
    }
    else if (_local_params.direction == StreamDirection::SENDONLY){
        return StreamDirection::RECVONLY;
    }
    else if (_local_params.direction == StreamDirection::RECVONLY){
        return StreamDirection::SENDONLY;
    }
    else{
        return _local_params.direction;
    }
}

void VideoStream::onSubscribeFrame(const MediaFrame &frame)
{
    auto packets = _rtp_packetitzer->buildPackets( frame );
    for(auto& ipack : packets){
        onSubscribePacket(*ipack.get());
    }
}

///packet from publicsher
void VideoStream::onSubscribePacket(const RtpPacket &packet)
{
    RtpPacketToSend send_packet(nullptr);
    send_packet.Parse(packet.data(), packet.size());
    uint32_t ssrc=0;
    if(_remote_params.ssrcs.size()>0){
         ssrc = *_remote_params.ssrcs.begin();
    }
    else{
        ssrc = rtc::CreateRandomNonZeroId();
        _remote_params.ssrcs.insert(ssrc);
    }
    send_packet.SetSsrc(ssrc);

    sendRtpToTransport(send_packet);
}

bool VideoStream::isListening(const StreamEvent *evt)
{
    return true;
}

void VideoStream::onKeyFrameRequest(const KeyFrameRequestEvent *event)
{
    _rtcp_handler.sendFirPli();
}

void VideoStream::onPacketRTXRequest(const PacketRTXRequestEvent *event)
{
    _rtcp_handler.sendNack(event->packet_numbers());
}

bool VideoStream::isNormalPayload(int pt)
{
    bool ok=false;
    for(auto i : _local_params.codecs){
        if(i.payload_type == pt){
            ok = true;
            break;
        }
    }
    if(ok){
        //return !isFecPayload(pt) && !isRedPayload(pt);
        return true;
    }

    return false;
}

bool VideoStream::sendRtpToTransport(const RtpPacketToSend &packet)
{
    if(_transport != nullptr){
        _transport->sendRtp(packet);
        _rtcp_handler.updateSendingRtp(packet);
        _send_statist->updateSendingRtp(packet);

        //ToDo: put packet to sender history
        //_rtx_history->addPacket(send_packet);
        return true;
    }

    return false;
}

void VideoStream::requestPacketRTX(uint32_t ssrc, const std::vector<uint16_t>& packet_numbers)
{
     //If this event I can not handle, then send the event to peer EventHandler to handle.
     //ToDo: Find these rtx packets from Rtx History
     //for(auto n : packet_numbers){
        //RtpPacket* packet = _rtx_history->find(n);
        //if(packet != nullptr){
        //  RtpPacket rtx_packet = buildRtxPacket(packet);
        //  sedRtpToTransport(rtx_packet);
        //}
        //else{
        //    rtx_numbers.push_back(n);
        //}
     //}
     PacketRTXRequestEvent evt(ssrc, packet_numbers);
     this->sendEvent(evt);
}

void VideoStream::requestKeyFrame(uint32_t media_ssrc)
{
     //If this event I can not handle, the send the event to peer EventHandler to handle.
     KeyFrameRequestEvent evt;
     this->sendEvent(evt);


     //for test
     onKeyFrameRequest(&evt);
}

void VideoStream::requestSendRtcpPacket(const uint8_t *packet, size_t length)
{
    if(_transport != nullptr){
        RtpRtcpBuffer buf(packet, length, 1400);
        _transport->sendRtcp(buf);
    }
}

}//rtcgw
