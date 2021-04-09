#include "rtp_rtcp_handler.h"
#include "time_clocks.h"
#include "session/stream.h"
///webrtc includes
#include <modules/rtp_rtcp/source/rtcp_receiver.h>
#include <modules/rtp_rtcp/source/rtcp_sender.h>

#include <iostream>

namespace rtcgw {

class RtcpReceiver : public Receiver
        ,public webrtc::RtcpBandwidthObserver
        ,public webrtc::RtcpIntraFrameObserver
        ,public webrtc::RtcpLossNotificationObserver
        ,public webrtc::NetworkStateEstimateObserver
        ,public webrtc::TransportFeedbackObserver
        ,public webrtc::RTCPReceiver::ModuleRtpRtcp
        ,public webrtc::RtcpStatisticsCallback

{
public:
    RtcpReceiver(int stream_type);
    ~RtcpReceiver()override;

    // Receiver interface
public:
    void onRecvRtpPacket(const RtpPacket &packet)override{ /*not implements;*/ }
    void onRecvRtcpPacket(const RtpRtcpBuffer *packet)override;
    void setLocalSsrc(uint32_t ssrc)override;
    void setRemoteSsrc(uint32_t ssrc)override;
    bool NTP(uint32_t* received_ntp_secs,
           uint32_t* received_ntp_frac,
           uint32_t* rtcp_arrival_time_secs,
           uint32_t* rtcp_arrival_time_frac,
           uint32_t* rtcp_timestamp) const override;

    std::vector<ReceiveTimeInfo> consumeReceivedXrReferenceTimeInfo() override;

    // RtcpBandwidthObserver interface
private:
    void OnReceivedEstimatedBitrate(uint32_t bitrate)override;
    void OnReceivedRtcpReceiverReport(const webrtc::ReportBlockList &report_blocks, int64_t rtt, int64_t now_ms)override;


    // RtcpIntraFrameObserver interface
private:
    void OnReceivedIntraFrameRequest(uint32_t ssrc)override;


    // RtcpLossNotificationObserver interface
private:
    void OnReceivedLossNotification(uint32_t ssrc, uint16_t seq_num_of_last_decodable, uint16_t seq_num_of_last_received, bool decodability_flag)override;


    // NetworkStateEstimateObserver interface
private:
    void OnRemoteNetworkEstimate(webrtc::NetworkStateEstimate estimate)override;


    // TransportFeedbackObserver interface
private:
    void OnAddPacket(const webrtc::RtpPacketSendInfo &packet_info)override;
    void OnTransportFeedback(const webrtc::rtcp::TransportFeedback &feedback)override;



    // ModuleRtpRtcp interface
private:
    void SetTmmbn(std::vector<webrtc::rtcp::TmmbItem> bounding_set)override;
    void OnRequestSendReport()override;
    void OnReceivedNack(const std::vector<uint16_t> &nack_sequence_numbers)override;
    void OnReceivedRtcpReportBlocks(const webrtc::ReportBlockList &report_blocks)override;

    // RtcpStatisticsCallback interface
private:
    void StatisticsUpdated(const webrtc::RtcpStatistics &statistics, uint32_t ssrc)override;

private:
    std::unique_ptr<webrtc::RTCPReceiver> _receiver_impl;
    int _stream_type;
    uint32_t _local_ssrc;
    uint32_t _remote_ssrc;


};

Receiver* createRtcpReciver(int stream_type)
{
    return new RtcpReceiver(stream_type);
}

RtcpReceiver::RtcpReceiver(int stream_type)
    :_stream_type(stream_type)
{
}

RtcpReceiver::~RtcpReceiver()
{

}

void RtcpReceiver::onRecvRtcpPacket(const RtpRtcpBuffer *packet)
{
    if(_receiver_impl != nullptr){
        _receiver_impl->IncomingPacket(packet->data(), packet->size());
    }
}

void RtcpReceiver::setLocalSsrc(uint32_t ssrc)
{
    _local_ssrc = ssrc;

    webrtc::RtpRtcp::Configuration config;
    config.audio = _stream_type==STREAM_AUDIO;
    config.local_media_ssrc = _local_ssrc;
    config.receiver_only = false;
    config.bandwidth_callback = this;
    config.intra_frame_callback = this;
    config.rtcp_loss_notification_observer = this;
    config.network_state_estimate_observer = this;
    config.transport_feedback_callback = this;
    config.rtt_stats = nullptr;
    config.clock = webrtc::Clock::GetRealTimeClock();
    _receiver_impl.reset(new webrtc::RTCPReceiver(config, this));
}

void RtcpReceiver::setRemoteSsrc(uint32_t ssrc)
{
    _remote_ssrc = ssrc;
    if(_receiver_impl != nullptr){
        _receiver_impl->SetRemoteSSRC(ssrc);
    }
}

bool RtcpReceiver::NTP(uint32_t *received_ntp_secs,
                       uint32_t *received_ntp_frac,
                       uint32_t *rtcp_arrival_time_secs,
                       uint32_t *rtcp_arrival_time_frac,
                       uint32_t *rtcp_timestamp) const
{
    if(_receiver_impl != nullptr){
        return _receiver_impl->NTP(received_ntp_secs, received_ntp_frac
                           ,rtcp_arrival_time_secs, rtcp_arrival_time_frac
                           ,rtcp_timestamp);
    }

    return false;
}

std::vector<ReceiveTimeInfo>
RtcpReceiver::consumeReceivedXrReferenceTimeInfo()
{
  std::vector<ReceiveTimeInfo> last_xr_rtis;
  if(_receiver_impl != nullptr){
      return _receiver_impl->ConsumeReceivedXrReferenceTimeInfo();
  }
  return last_xr_rtis;
}


void RtcpReceiver::OnReceivedEstimatedBitrate(uint32_t bitrate)
{
    std::cout << __FUNCTION__ <<" bitrate="<<bitrate<<std::endl;
}

void RtcpReceiver::OnReceivedRtcpReceiverReport(const webrtc::ReportBlockList &report_blocks, int64_t rtt, int64_t now_ms)
{
    std::cout << __FUNCTION__ <<" rtt="<<rtt<<std::endl;
}

void RtcpReceiver::OnReceivedIntraFrameRequest(uint32_t ssrc)
{
    if(_keyframe_request_cb != nullptr){
        _keyframe_request_cb(ssrc);
    }
}

void RtcpReceiver::OnReceivedLossNotification(uint32_t ssrc, uint16_t seq_num_of_last_decodable, uint16_t seq_num_of_last_received, bool decodability_flag)
{
    //forward to upper to handle
}

void RtcpReceiver::OnRemoteNetworkEstimate(webrtc::NetworkStateEstimate estimate)
{
    //forward to upper to handle
}

void RtcpReceiver::OnAddPacket(const webrtc::RtpPacketSendInfo &packet_info)
{

}

void RtcpReceiver::OnTransportFeedback(const webrtc::rtcp::TransportFeedback &feedback)
{

}

void RtcpReceiver::SetTmmbn(std::vector<webrtc::rtcp::TmmbItem> bounding_set)
{
    //send Tmmbn to peer
}

void RtcpReceiver::OnRequestSendReport()
{
    //collect FeedbackState
    //send SR packet to peer
}

void RtcpReceiver::OnReceivedNack(const std::vector<uint16_t> &nack_sequence_numbers)
{
    if(_rtx_request_cb != nullptr){
        _rtx_request_cb(_remote_ssrc, nack_sequence_numbers);
    }
}

void RtcpReceiver::OnReceivedRtcpReportBlocks(const webrtc::ReportBlockList &report_blocks)
{
    //notify the rtp sender extended_highest_sequence_number has ack
}

void RtcpReceiver::StatisticsUpdated(const webrtc::RtcpStatistics &statistics, uint32_t ssrc)
{
    //notify to upper
}




///////////////////////////////////////////////////////////////////////////////////////////////////////
class ReceiveStatisticsImpl : public ReceiveStatistics
{
public:
    ~ReceiveStatisticsImpl(){
    }
    std::unique_ptr<webrtc::ReceiveStatistics> impl;

    // ReceiveStatistics interface
public:
    void onRtpPacket(const RtpPacketReceived &packet){
        if(impl != nullptr){
            impl->OnRtpPacket(packet);
        }
    }
};

ReceiveStatistics *createReceiveStatistics()
{
    ReceiveStatisticsImpl* stat = new ReceiveStatisticsImpl();
    stat->impl = webrtc::ReceiveStatistics::Create(webrtc::Clock::GetRealTimeClock());
    return stat;
}




///////////////////////////////////////////////////////////////////////////////////////////////////////
class RtcpSender : public Sender
        ,public webrtc::Transport
{
public:
     RtcpSender(int stream_type, ReceiveStatistics* statist);
     ~RtcpSender()override;

     // Sender interface
 public:
    void setLocalSsrc(uint32_t ssrc)override;
    void setRemoteSsrc(uint32_t ssrc)override;
    void setRtcpMode(RtcpMode mode)override;
    void sendingRtp(const FeedbackState &state, const RtpPacket &packet)override;
    bool sendRtcp(const FeedbackState& state, const std::set<RTCPPacketType>&  rtcp)override;
    bool timeToSendRtcpReport()override;

private:
    std::unique_ptr<webrtc::RTCPSender> _sender_impl;
    int _stream_type;
    uint32_t _local_ssrc;
    uint32_t _remote_ssrc;
    ReceiveStatisticsImpl* _recv_statist=nullptr;

    // Transport interface
private:
    bool SendRtp(const uint8_t *packet, size_t length, const webrtc::PacketOptions &options)override;
    bool SendRtcp(const uint8_t *packet, size_t length)override;
};

Sender *createRtcpSender(int stream_type, ReceiveStatistics* statist)
{
    return new RtcpSender(stream_type, statist);
}

RtcpSender::RtcpSender(int stream_type, ReceiveStatistics *statist)
    :_stream_type(stream_type)
    ,_recv_statist(static_cast<ReceiveStatisticsImpl*>(statist))
{

}

RtcpSender::~RtcpSender()
{

}

bool RtcpSender::sendRtcp(const FeedbackState &state, const std::set<RTCPPacketType> &rtcp)
{
    if(_sender_impl != nullptr){
        webrtc::RTCPSender::FeedbackState fbstate;
        fbstate.packets_sent = state.packets_sent;
        fbstate.media_bytes_sent = state.media_bytes_sent;
        fbstate.send_bitrate = state.send_bitrate;
        fbstate.last_rr_ntp_secs = state.last_rr_ntp_secs;
        fbstate.last_rr_ntp_frac = state.last_rr_ntp_frac;
        fbstate. remote_sr = state.remote_sr;
        std::set<webrtc::RTCPPacketType> rtcps;
        for(auto i : rtcp){
            rtcps.insert((webrtc::RTCPPacketType)i );
        }
        _sender_impl->SendCompoundRTCP(fbstate, rtcps);
    }

    return true;
}

bool RtcpSender::timeToSendRtcpReport()
{
    if(_sender_impl != nullptr){
        return _sender_impl->TimeToSendRTCPReport();
    }

    return false;
}


void RtcpSender::setLocalSsrc(uint32_t ssrc)
{
    _local_ssrc = ssrc;

    webrtc::RtpRtcp::Configuration config;
    config.audio = _stream_type==STREAM_AUDIO;
    config.local_media_ssrc = _local_ssrc;
    config.outgoing_transport = this;
    config.clock = webrtc::Clock::GetRealTimeClock();
    config.receive_statistics = _recv_statist->impl.get();
    _sender_impl.reset(new webrtc::RTCPSender(config));
}

void RtcpSender::setRemoteSsrc(uint32_t ssrc)
{
    _remote_ssrc = ssrc;
    if(_sender_impl != nullptr){
        _sender_impl->SetRemoteSSRC(ssrc);
    }
}

void RtcpSender::setRtcpMode(RtcpMode mode)
{
    if(_sender_impl != nullptr){
        _sender_impl->SetRTCPStatus((webrtc::RtcpMode)mode);
    }
}

void RtcpSender::sendingRtp(const FeedbackState &state, const RtpPacket &packet)
{
    if(_sender_impl != nullptr){
        webrtc::RTCPSender::FeedbackState fbstate;
        fbstate.packets_sent = state.packets_sent;
        fbstate.media_bytes_sent = state.media_bytes_sent;
        fbstate.send_bitrate = state.send_bitrate;
        fbstate.last_rr_ntp_secs = state.last_rr_ntp_secs;
        fbstate.last_rr_ntp_frac = state.last_rr_ntp_frac;
        fbstate. remote_sr = state.remote_sr;
        _sender_impl->SetSendingStatus(fbstate, true);
        _sender_impl->SetLastRtpTime(packet.Timestamp(), -1, packet.PayloadType());
    }
}

bool RtcpSender::SendRtp(const uint8_t *packet, size_t length, const webrtc::PacketOptions &options)
{
}

bool RtcpSender::SendRtcp(const uint8_t *packet, size_t length)
{
    if(_send_rtcp_request_cb != nullptr){
        _send_rtcp_request_cb(packet, length);
    }
}




////////////////////////////////////////////////////////////////////////////////////////////////////////
void SendStatistics::updateSendingRtp(const RtpPacketToSend &packet)
{
    int64_t now_ms = Clock::timestamp_ms();
    total_bitrate_sent_.Update(packet.size(), now_ms);

    StreamDataCounters* counters = packet.Ssrc() == rtx_ssrc ? &rtx_stats : &rtp_stats;
    if(counters->first_packet_time_ms == -1) {
        counters->first_packet_time_ms = now_ms;
    }

    if(packet.packet_type() == RtpPacketMediaType::kForwardErrorCorrection){
        counters->fec.AddPacket(packet);
    }

    if(packet.packet_type() == RtpPacketMediaType::kRetransmission){
        counters->retransmitted.AddPacket(packet);
        nack_bitrate_sent_.Update(packet.size(), now_ms);
    }
    counters->transmitted.AddPacket(packet);
}

uint32_t SendStatistics::bitrate() const
{
    webrtc::DataRate dr = webrtc::DataRate::BitsPerSec(
        total_bitrate_sent_.Rate( Clock::timestamp_ms( )).value_or(0)
    );

    return dr.bps<uint32_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////////
RtpRtcpHandler::RtpRtcpHandler(int stream_type
                               , ReceiveStatistics *recv_statist
                               , SendStatistics *send_statist
                               , RequestPacketRTXCb rtxcb
                               , RequestKeyFrameCb keycb
                               , SendRtcpRequestCb sendcb)
{
    _rtp_send_statist =  send_statist;

    _rtcp_receiver.reset( createRtcpReciver(stream_type) );
    _rtcp_receiver->setRequestKeyFrameCb(keycb);
    _rtcp_receiver->setRequestPacketRTXCb(rtxcb);

    _rtcp_sender.reset( createRtcpSender(stream_type, recv_statist) );
    _rtcp_sender->setSendRtcpRequestCb(sendcb);
}

void RtpRtcpHandler::onRecvRtcpPacket(const RtpRtcpBuffer *packet)
{
    if(_rtcp_receiver != nullptr){
        _rtcp_receiver->onRecvRtcpPacket(packet);
    }
}

void RtpRtcpHandler::updateSendingRtp(const RtpPacket &packet)
{
    if(_rtcp_sender != nullptr){
        _rtcp_sender->sendingRtp(getFeedbackState(), packet);
    }
}

void RtpRtcpHandler::sendNack(const std::vector<uint16_t> &packet_numbers)
{

}

void RtpRtcpHandler::sendFirPli()
{
    if(_rtcp_sender != nullptr){
        FeedbackState state;
        _rtcp_sender->sendRtcp(state, std::set<RTCPPacketType>{kRtcpPli});
    }
}

void RtpRtcpHandler::setLocalSsrc(uint32_t ssrc)
{
    if(_rtcp_receiver != nullptr){
        _rtcp_receiver->setLocalSsrc(ssrc);
    }

    if(_rtcp_sender != nullptr){
        _rtcp_sender->setLocalSsrc(ssrc);
    }
}

void RtpRtcpHandler::setRemoteSsrc(uint32_t ssrc)
{
    if(_rtcp_receiver != nullptr){
        _rtcp_receiver->setRemoteSsrc(ssrc);
    }

    if(_rtcp_sender != nullptr){
        _rtcp_sender->setRemoteSsrc(ssrc);
    }
}

void RtpRtcpHandler::setRtcpMode(RtcpMode mode)
{
    if(_rtcp_sender != nullptr){
        _rtcp_sender->setRtcpMode(mode);
    }
}

void RtpRtcpHandler::checkAndSendRtcp()
{
    //send SR or RR
    if (_rtcp_sender != nullptr &&_rtcp_sender->timeToSendRtcpReport()){
        FeedbackState state = getFeedbackState();
        _rtcp_sender->sendRtcp(state, std::set<RTCPPacketType>{kRtcpReport});
    }
}

FeedbackState RtpRtcpHandler::getFeedbackState() {
  FeedbackState state;
  // This is called also when receiver_only is true. Hence below
  // checks that rtp_sender_ exists.
  if (_rtp_send_statist != nullptr) {

    state.packets_sent = _rtp_send_statist->rtp_stats.transmitted.packets +
                         _rtp_send_statist->rtx_stats.transmitted.packets;

    state.media_bytes_sent = _rtp_send_statist->rtp_stats.transmitted.payload_bytes +
                             _rtp_send_statist->rtx_stats.transmitted.payload_bytes;

    state.send_bitrate = _rtp_send_statist->bitrate();
  }

  getLastReceivedNTP(&state.last_rr_ntp_secs, &state.last_rr_ntp_frac,
                  &state.remote_sr);

  state.last_xr_rtis = _rtcp_receiver->consumeReceivedXrReferenceTimeInfo();

  return state;
}

bool RtpRtcpHandler::getLastReceivedNTP(
    uint32_t* rtcp_arrival_time_secs,  // When we got the last report.
    uint32_t* rtcp_arrival_time_frac,
    uint32_t* remote_sr) const {
  // Remote SR: NTP inside the last received (mid 16 bits from sec and frac).
  uint32_t ntp_secs = 0;
  uint32_t ntp_frac = 0;

  if (!_rtcp_receiver->NTP(&ntp_secs, &ntp_frac, rtcp_arrival_time_secs
                          ,rtcp_arrival_time_frac, NULL))
  {
    return false;
  }

  *remote_sr =
      ((ntp_secs & 0x0000ffff) << 16) + ((ntp_frac & 0xffff0000) >> 16);

  return true;
}


}//rtcgw
