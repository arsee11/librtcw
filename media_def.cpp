///media_def.cpp

#include "media_def.h"

///webrtc includes
#include <pc/session_description.h>
#include <rtc_base/message_digest.h>
#include <api/jsep.h>
#include <api/jsep_session_description.h>
#include <rtc_base/rtc_certificate_generator.h>

namespace rtcgw {

bool operator==(const CodecParams &lhs, const CodecParams &rhs)
{
    return (  lhs.name == rhs.name
            &&lhs.codec_type == rhs.codec_type
            &&lhs.payload_type == rhs.payload_type
           );
}

}//rtcw

