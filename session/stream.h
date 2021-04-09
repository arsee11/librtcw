#ifndef STREAM_H
#define STREAM_H

#include <string>
#include <set>
#include <vector>

#include <rtputils/rtp_packetizer.h>

#include "source.h"
#include "observer.h"
#include "transport/transport.h"
#include "threads.h"

namespace rtcgw {

enum{
    STREAM_VIDEO,
    STREAM_AUDIO,
    STREAM_DATA
};

enum CodecType {
  CODEC_VIDEO,
  CODEC_RED,
  CODEC_ULPFEC,
  CODEC_FLEXFEC,
  CODEC_RTX,
  CODEC_AUDIO
};

struct FeedbackParam {
  std::string id;     // e.g. "nack", "ccm"
  std::string param;  // e.g. "", "rpsi", "fir"
};

typedef std::map<std::string, std::string> CodecParameterMap;

struct CodecParams
{
    std::string name;
    int payload_type;
    int codec_type;
    CodecParameterMap fmtps;
    std::vector<FeedbackParam> feedback_params;
};

bool operator==(const CodecParams& lhs, const CodecParams& rhs);

enum class StreamDirection {
  SENDRECV,
  SENDONLY,
  RECVONLY,
  INACTIVE,
  STOPPED,
};

struct StreamInfo
{
    std::string id;
    int stream_type;
    StreamDirection direction;
    bool is_rtcpmux=false;
    bool is_rtcp_reduced_size=false;
    std::set<uint32_t> ssrcs;
    std::vector<CodecParams> codecs;
};

using StreamInfos = std::vector<StreamInfo>;


struct StreamStatistics
{

};


class Stream :public SourceImpl, public Observer
{
public:
    Stream(const std::string&  id, ThreadScope* thr)
        :_id(id)
        ,_thread_scope(thr)
    {}
    ~Stream(){}

    const std::string&  id()const { return _id; }
    std::string onwer_session()const { return "";};
    void rtcp_mux(bool val){ rtcp_mux_=val; }
    bool rtcp_mux()const { return rtcp_mux_; }

    virtual void setRemoteParams(const StreamInfo& params);
    virtual void setLocalParams(const StreamInfo& params);
    std::vector<std::string> sendCodecs();
    std::vector<std::string> recvCodecs();

    transport_ptr transport(){ return _transport; }
    void transport(transport_ptr trans){ _transport = trans; }

    virtual int type()const =0;
    virtual std::tuple<StreamInfo, TransportInfo> doNegotiation()=0;
    virtual void onRecvRtpFromTransport(Transport* trans, const RtpPacketReceived& packet)=0;
    virtual void onRecvRtcpFromTransport(Transport* trans, const RtpRtcpBuffer* packet)=0;
    virtual void onTimerEvent()=0;


protected:
    std::string  _id;
    ThreadScope* _thread_scope;

    bool rtcp_mux_=true;
    int direction; //send-recv  send-only recv-only
    StreamInfo _remote_params;
    StreamInfo _local_params;
    transport_ptr _transport;
    std::unique_ptr<RtpPacketizer> _rtp_packetitzer;

};

}

#endif // STREAM_H
