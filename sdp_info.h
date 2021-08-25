#ifndef SDP_INFO_H
#define SDP_INFO_H

#include "media_def.h"

namespace rtcgw {

MediaInfo parseSdp2MeidaInfo(const std::string& sdpstr);
std::string convertMeidaInfo2Sdp(const MediaInfo& minfo);

}//rtcw

#endif /*SDP_INFO_H*/
