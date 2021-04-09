#ifndef VIDEO_STREAM_H
#define VIDEO_STREAM_H

#include "rtputils/nack_handler.h"
#include "rtputils/rtp_rtcp_handler.h"
#include "stream.h"
#include "stream_events.h"


namespace rtcgw {

class VideoStream : public Stream, public StreamEventHandler, public StreamEventSource
{
public:
    VideoStream(const std::string& id, ThreadScope *thr);

    // Stream interface
public:
    void onRecvRtpFromTransport(Transport *trans, const RtpPacketReceived &packet)override;
    void onRecvRtcpFromTransport(Transport *trans, const RtpRtcpBuffer *packet)override;
    void setLocalParams(const StreamInfo &params)override;
    void setRemoteParams(const StreamInfo &params)override;
    std::tuple<StreamInfo, TransportInfo> doNegotiation()override;
    void onTimerEvent()override;
    int type()const override{ return STREAM_VIDEO; }

    // Observer interface
public:
    void onSubscribeFrame(const MediaFrame& frame)override;
    void onSubscribePacket(const RtpPacket& packet)override;


    // StreamEventHandler interface
public:
    bool isListening(const StreamEvent *evt)override;
    void onKeyFrameRequest(const KeyFrameRequestEvent *event)override;
    void onPacketRTXRequest(const PacketRTXRequestEvent *event)override;

protected:
    bool isNormalPayload(int pt);
    bool sendRtpToTransport(const RtpPacketToSend &packet);
    void requestPacketRTX(uint32_t ssrc, const std::vector<uint16_t> &packet_numbers);
    void requestKeyFrame(uint32_t media_ssrc);
    void requestSendRtcpPacket(const uint8_t *packet, size_t length);
    void dumpPacket(const RtpPacket &packet);

    StreamDirection negotiationDirection();

private:
    std::unique_ptr<ReceiveStatistics> _recv_statist;
    std::unique_ptr<SendStatistics> _send_statist;
    RtpRtcpHandler _rtcp_handler;
    NackHandler _nack_handler;
};

}

#endif // VIDEO_STREAM_H
