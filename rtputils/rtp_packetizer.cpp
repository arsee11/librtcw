//rtp_packetizer.cpp

#include "rtp_packetizer.h"

///webrtc includes
#include <modules/rtp_rtcp/source/rtp_format.h>

namespace rtcgw {

RtpPacketizer *RtpPacketizer::create(const std::string &name, int payload_type)
{
    return nullptr;
}

class RtpPacketizerVP9 : public RtpPacketizer
{
    // RtpPacketizer interface
public:
    std::vector<std::unique_ptr<RtpPacketToSend>> buildPackets(const MediaFrame& frame);

private:
    int _payload_type;
};

std::unique_ptr<RtpPacketToSend> AllocatePacket() {
  static constexpr int kExtraCapacity = 16;
  int max_packet_size_=1200;
  auto packet = std::make_unique<RtpPacketToSend>(
              nullptr, max_packet_size_ + kExtraCapacity);

    // Reserve extensions, if registered, RtpSender set in SendToNetwork.
//    packet->ReserveExtension<AbsoluteSendTime>();
//    packet->ReserveExtension<TransmissionOffset>();
//    packet->ReserveExtension<TransportSequenceNumber>();
//    packet->SetExtension<RtpMid>(mid_);
//    packet->SetExtension<RtpStreamId>(rid_);

  return packet;
}

std::vector<std::unique_ptr<RtpPacketToSend>> RtpPacketizerVP9::buildPackets(const MediaFrame &frame)
{

    std::vector<std::unique_ptr<RtpPacketToSend>> rtp_packets;
    if(frame.size == 0)
        return rtp_packets;


    std::unique_ptr<RtpPacketToSend> single_packet = AllocatePacket();
    single_packet->SetPayloadType(_payload_type);
    //single_packet->SetTimestamp(rtp_timestamp);
    single_packet->set_capture_time_ms(frame.timestamp_ms);



    auto first_packet = std::make_unique<RtpPacketToSend>(*single_packet);
    auto middle_packet = std::make_unique<RtpPacketToSend>(*single_packet);
    auto last_packet = std::make_unique<RtpPacketToSend>(*single_packet);



    std::unique_ptr<webrtc::RtpPacketizer> packetizer = nullptr;/*webrtc::RtpPacketizer::Create(
        codec_type, _payload_type, limits, video_header, fragmentation);*/

    const size_t num_packets = packetizer->NumPackets();
    if (num_packets == 0)
        return rtp_packets;

    bool first_frame = false;//first_frame_sent_();
    for (size_t i = 0; i < num_packets; ++i) {
        std::unique_ptr<RtpPacketToSend> packet;
        int expected_payload_capacity;
        // Choose right packet template:
        if (num_packets == 1) {
            packet = std::move(single_packet);
            //expected_payload_capacity =
            //    limits.max_payload_len - limits.single_packet_reduction_len;
        } else if (i == 0) {
            packet = std::move(first_packet);
            //expected_payload_capacity =
            //    limits.max_payload_len - limits.first_packet_reduction_len;
        } else if (i == num_packets - 1) {
            packet = std::move(last_packet);
            //expected_payload_capacity =
            //    limits.max_payload_len - limits.last_packet_reduction_len;
        } else {
            packet = std::make_unique<RtpPacketToSend>(*middle_packet);
            //expected_payload_capacity = limits.max_payload_len;
        }

        packet->set_first_packet_of_frame(i == 0);

        if (!packetizer->NextPacket(packet.get()))
            return rtp_packets;

    //    if (!rtp_sender_->AssignSequenceNumber(packet.get()))
    //      return;

    //packet->set_allow_retransmission(allow_retransmission);

    // Put packetization finish timestamp into extension.
    //    if (packet->HasExtension<VideoTimingExtension>()) {
    //      packet->set_packetization_finish_time_ms(clock_->TimeInMilliseconds());
    //    }

        packet->set_packet_type(RtpPacketMediaType::kVideo);
        rtp_packets.emplace_back(std::move(packet));
    }

    return rtp_packets;}


}//rtcgw
