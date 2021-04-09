///stream.cpp
///


#include "stream.h"

namespace rtcgw{

bool operator==(const CodecParams &lhs, const CodecParams &rhs)
{
    return (  lhs.name == rhs.name
            &&lhs.codec_type == rhs.codec_type
            &&lhs.payload_type == rhs.payload_type
           );
}

void Stream::setRemoteParams(const StreamInfo &params){ _remote_params = params; }

void Stream::setLocalParams(const StreamInfo &params){ _local_params = params; }

std::vector<std::string> rtcgw::Stream::sendCodecs()
{
    std::vector<std::string> cs;
    for(auto icodec : _local_params.codecs){
        cs.push_back( icodec.name );
    }

    return cs;
}

std::vector<string> Stream::recvCodecs()
{
    std::vector<std::string> cs;
    for(auto icodec : _remote_params.codecs){
        cs.push_back( icodec.name );
    }

    return cs;
}

}///rtcgw
