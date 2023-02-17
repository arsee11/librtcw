#ifndef MEDIA_DEF_H
#define MEDIA_DEF_H

#include <string>
#include <vector>
#include <map>
#include <vector>
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

enum VideoCodec {
  VC_UNKNOWN = 0,
  VC_VP8,
  VC_VP9,
  VC_AV1,
  VC_H264
};

constexpr char VN_Vp8[] = "VP8";
constexpr char VN_Vp9[] = "VP9";
constexpr char VN_Av1[] = "AV1X";
constexpr char VN_H264[] = "H264";
constexpr char VN_Generic[] = "Generic";
constexpr char VN_Multiplex[] = "Multiplex";

struct FeedbackParam {
  std::string id;     // e.g. "nack", "ccm"
  std::string param;  // e.g. "", "rpsi", "fir"
};

typedef std::map<std::string, std::string> CodecParameterMap;

struct CodecParams
{
    std::string name;
    int payload_type;
    CodecType codec_type;
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

struct  StreamBaseInfo
{
    stream_id_t id;
    std::string mid;//sdp mid
    int stream_type;
    std::string source_tag;
    std::string sink_tag;
};

struct StreamDetailInfo
{
    StreamDirection direction;
    bool is_rtcpmux=true;
    bool is_rtcp_reduced_size=false;
    std::vector<uint32_t> ssrcs;
    std::string cname;
    std::vector<CodecParams> codecs;
};

struct StreamInfo
{
    StreamBaseInfo binfo;
    StreamDetailInfo dinfo;
};

using StreamInfos = std::vector<StreamInfo>;


struct StreamStatistics
{

};

struct GroupInfo{
    std::string group_name;
    std::vector<std::string> streams;
};

using GroupInfos = std::vector<GroupInfo>;

struct MediaInfo
{
    GroupInfos groups;
    StreamInfos sinfos;
    TransportInfos tinfos;
};

}//rtcw

#endif /*MEDIA_DEF_H*/
