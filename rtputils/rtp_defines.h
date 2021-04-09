#ifndef RTP_DEFINES_H
#define RTP_DEFINES_H

///webrtc includes
#include <modules/rtp_rtcp/source/rtcp_packet/dlrr.h>
#include <modules/rtp_rtcp/source/rtp_packet.h>
#include <modules/rtp_rtcp/source/rtp_packet_received.h>
#include <modules/rtp_rtcp/source/rtp_packet_to_send.h>
#include <rtc_base/copy_on_write_buffer.h>
#include <rtc_base/rate_statistics.h>


namespace rtcgw {

using RtpPacketMediaType = webrtc::RtpPacketMediaType;

using RtpPacket = webrtc::RtpPacket;
using RtpPacketReceived = webrtc::RtpPacketReceived;
using RtpPacketToSend = webrtc::RtpPacketToSend;
using RtpRtcpBuffer = rtc::CopyOnWriteBuffer;
using RateStatistics = webrtc::RateStatistics;

enum RTCPPacketType : uint32_t {
  kRtcpReport = 0x0001,
  kRtcpSr = 0x0002,
  kRtcpRr = 0x0004,
  kRtcpSdes = 0x0008,
  kRtcpBye = 0x0010,
  kRtcpPli = 0x0020,
  kRtcpNack = 0x0040,
  kRtcpFir = 0x0080,
  kRtcpTmmbr = 0x0100,
  kRtcpTmmbn = 0x0200,
  kRtcpSrReq = 0x0400,
  kRtcpApp = 0x1000,
  kRtcpLossNotification = 0x2000,
  kRtcpRemb = 0x10000,
  kRtcpTransmissionTimeOffset = 0x20000,
  kRtcpXrReceiverReferenceTime = 0x40000,
  kRtcpXrDlrrReportBlock = 0x80000,
  kRtcpTransportFeedback = 0x100000,
  kRtcpXrTargetBitrate = 0x200000
};

enum class RtcpMode {
    kOff,
    kCompound,
    kReducedSize
};

using ReceiveTimeInfo = webrtc::rtcp::ReceiveTimeInfo;

struct FeedbackState
{
  uint32_t packets_sent=0;
  size_t media_bytes_sent=0;
  uint32_t send_bitrate=0;

  uint32_t last_rr_ntp_secs=0; //ntp seconds
  uint32_t last_rr_ntp_frac=0; //ntp fractions
  uint32_t remote_sr=0;

  std::vector<ReceiveTimeInfo> last_xr_rtis;

};

struct RtpPacketCounter {
  RtpPacketCounter()
      : header_bytes(0), payload_bytes(0), padding_bytes(0), packets(0) {}

  void Add(const RtpPacketCounter& other) {
    header_bytes += other.header_bytes;
    payload_bytes += other.payload_bytes;
    padding_bytes += other.padding_bytes;
    packets += other.packets;
  }

  void Subtract(const RtpPacketCounter& other) {
    RTC_DCHECK_GE(header_bytes, other.header_bytes);
    header_bytes -= other.header_bytes;
    RTC_DCHECK_GE(payload_bytes, other.payload_bytes);
    payload_bytes -= other.payload_bytes;
    RTC_DCHECK_GE(padding_bytes, other.padding_bytes);
    padding_bytes -= other.padding_bytes;
    RTC_DCHECK_GE(packets, other.packets);
    packets -= other.packets;
  }

  bool operator==(const RtpPacketCounter& other) const {
    return header_bytes == other.header_bytes &&
           payload_bytes == other.payload_bytes &&
           padding_bytes == other.padding_bytes && packets == other.packets;
  }

  // Not inlined, since use of RtpPacket would result in circular includes.
  void AddPacket(const RtpPacket& packet){
      ++packets;
      header_bytes += packet.headers_size();
      padding_bytes += packet.padding_size();
      payload_bytes += packet.payload_size();
  }

  size_t TotalBytes() const {
    return header_bytes + payload_bytes + padding_bytes;
  }

  size_t header_bytes;   // Number of bytes used by RTP headers.
  size_t payload_bytes;  // Payload bytes, excluding RTP headers and padding.
  size_t padding_bytes;  // Number of padding bytes.
  uint32_t packets;      // Number of packets.
};

struct StreamDataCounters {
  StreamDataCounters() : first_packet_time_ms(-1) {}

  void Add(const StreamDataCounters& other) {
    transmitted.Add(other.transmitted);
    retransmitted.Add(other.retransmitted);
    fec.Add(other.fec);
    if (other.first_packet_time_ms != -1 &&
        (other.first_packet_time_ms < first_packet_time_ms ||
         first_packet_time_ms == -1)) {
      // Use oldest time.
      first_packet_time_ms = other.first_packet_time_ms;
    }
  }

  void Subtract(const StreamDataCounters& other) {
    transmitted.Subtract(other.transmitted);
    retransmitted.Subtract(other.retransmitted);
    fec.Subtract(other.fec);
    if (other.first_packet_time_ms != -1 &&
        (other.first_packet_time_ms > first_packet_time_ms ||
         first_packet_time_ms == -1)) {
      // Use youngest time.
      first_packet_time_ms = other.first_packet_time_ms;
    }
  }

  int64_t TimeSinceFirstPacketInMs(int64_t now_ms) const {
    return (first_packet_time_ms == -1) ? -1 : (now_ms - first_packet_time_ms);
  }

  // Returns the number of bytes corresponding to the actual media payload (i.e.
  // RTP headers, padding, retransmissions and fec packets are excluded).
  // Note this function does not have meaning for an RTX stream.
  size_t MediaPayloadBytes() const {
    return transmitted.payload_bytes - retransmitted.payload_bytes -
           fec.payload_bytes;
  }

  int64_t first_packet_time_ms;  // Time when first packet is sent/received.
  // The timestamp at which the last packet was received, i.e. the time of the
  // local clock when it was received - not the RTP timestamp of that packet.
  // https://w3c.github.io/webrtc-stats/#dom-rtcinboundrtpstreamstats-lastpacketreceivedtimestamp
  int64_t last_packet_received_timestamp_ms;
  RtpPacketCounter transmitted;    // Number of transmitted packets/bytes.
  RtpPacketCounter retransmitted;  // Number of retransmitted packets/bytes.
  RtpPacketCounter fec;            // Number of redundancy packets/bytes.
};



}//rtcgw

#endif // RTP_DEFINES_H
