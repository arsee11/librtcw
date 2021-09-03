#ifndef MEDIA_DEF_H
#define MEDIA_DEF_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include <tuple>

#include "transport_def.h"

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

using stream_id_t = std::string;

struct StreamInfo
{
    stream_id_t id;
    int stream_type;
    StreamDirection direction;
    bool is_rtcpmux=false;
    bool is_rtcp_reduced_size=false;
    std::set<uint32_t> ssrcs;
    std::string cname;
    std::vector<CodecParams> codecs;
};

using StreamInfos = std::vector<StreamInfo>;


struct StreamStatistics
{

};

struct MediaInfo
{
    StreamInfos sinfos;
    TransportInfos tinfos;
};

}//rtcw

#endif /*MEDIA_DEF_H*/
