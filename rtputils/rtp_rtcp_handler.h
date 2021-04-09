#ifndef RTP_RTCP_HANDLER_H
#define RTP_RTCP_HANDLER_H

#include "rtp_defines.h"
#include <set>

namespace rtcgw {

using RequestPacketRTXCb = std::function<void(uint32_t ssrc, std::vector<uint16_t> packet_numbers)>;
using RequestKeyFrameCb = std::function<void(uint32_t ssrc)>;
using SendRtcpRequestCb = std::function<void(const uint8_t* packet, int length)>;

class Receiver
{
public:
    virtual ~Receiver(){}
    virtual void onRecvRtpPacket(const RtpPacket& packet)=0;
    virtual void onRecvRtcpPacket(const RtpRtcpBuffer* packet)=0;
    virtual void setLocalSsrc(uint32_t ssrc)=0;
    virtual void setRemoteSsrc(uint32_t ssrc)=0;
    virtual bool NTP(uint32_t* received_ntp_secs,
                   uint32_t* received_ntp_frac,
                   uint32_t* rtcp_arrival_time_secs,
                   uint32_t* rtcp_arrival_time_frac,
                   uint32_t* rtcp_timestamp) const =0;
    virtual std::vector<ReceiveTimeInfo> consumeReceivedXrReferenceTimeInfo()=0;


    void setRequestKeyFrameCb(RequestKeyFrameCb cb){
        _keyframe_request_cb = cb;
    }

    void setRequestPacketRTXCb(RequestPacketRTXCb cb){
        _rtx_request_cb = cb;
    }

protected:
    RequestPacketRTXCb _rtx_request_cb;
    RequestKeyFrameCb _keyframe_request_cb;

};

extern Receiver* createRtcpReceiver(int stream_type);




//////////////////////////////////////////////////////////////////////////////////////////////////
class ReceiveStatistics
{
public:
    virtual ~ReceiveStatistics(){}
    virtual void onRtpPacket(const RtpPacketReceived& packet)=0;
};

extern ReceiveStatistics *createReceiveStatistics();




//////////////////////////////////////////////////////////////////////////////////////////////////
class Sender
{
public:
    virtual ~Sender(){}
    virtual void sendingRtp(const FeedbackState &state, const RtpPacket& packet)=0;
    virtual bool sendRtcp(const FeedbackState &state, const std::set<RTCPPacketType>& rtcp)=0;

    virtual void setLocalSsrc(uint32_t ssrc)=0;
    virtual void setRemoteSsrc(uint32_t ssrc)=0;
    virtual void setRtcpMode(RtcpMode mode)=0;
    virtual bool timeToSendRtcpReport()=0;

    void setSendRtcpRequestCb(SendRtcpRequestCb cb){ _send_rtcp_request_cb=cb;}

protected:
    SendRtcpRequestCb _send_rtcp_request_cb;

};

extern Sender* createRtcpSender(int stream_type, ReceiveStatistics *statist);




//////////////////////////////////////////////////////////////////////////////////////////////////
class SendStatistics
{
public:
    SendStatistics()
        :total_bitrate_sent_(1000, RateStatistics::kBpsScale)
        ,nack_bitrate_sent_(1000, RateStatistics::kBpsScale)
    {}

    void updateSendingRtp(const RtpPacketToSend &packet);

    //return bps.
    uint32_t bitrate()const;

    StreamDataCounters rtp_stats;
    StreamDataCounters rtx_stats;
    uint32_t rtx_ssrc;

private:
    RateStatistics total_bitrate_sent_;
    RateStatistics nack_bitrate_sent_;
};




//////////////////////////////////////////////////////////////////////////////////////////////////
class RtpRtcpHandler
{
public:
    RtpRtcpHandler(int stream_type
                   ,ReceiveStatistics* recv_statist
                   ,SendStatistics* send_statist
                   ,RequestPacketRTXCb rtxcb
                   ,RequestKeyFrameCb keycb
                   ,SendRtcpRequestCb sendcb

    );

    void onRecvRtcpPacket(const RtpRtcpBuffer* packet);
    void updateSendingRtp(const RtpPacket& packet);
    void sendNack(const std::vector<uint16_t> &packet_numbers);
    void sendFirPli();
    void setLocalSsrc(uint32_t ssrc);
    void setRemoteSsrc(uint32_t ssrc);
    void setRtcpMode(RtcpMode mode);

    void checkAndSendRtcp();

private:
    FeedbackState getFeedbackState();
    bool getLastReceivedNTP(uint32_t *rtcp_arrival_time_secs, uint32_t *rtcp_arrival_time_frac, uint32_t *remote_sr) const;

private:
    std::unique_ptr<Receiver> _rtcp_receiver;
    std::unique_ptr<Sender> _rtcp_sender;
    SendStatistics* _rtp_send_statist=nullptr;
};

}//rtcgw

#endif // RTP_RTCP_HANDLER_H
