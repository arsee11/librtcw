#include <iostream>
#include "sdp_info.h"

using namespace std;

void addMediaInfo(int media_type, rtcgw::MediaInfo& minfo)
{
    if( media_type == rtcgw::STREAM_VIDEO ){
        rtcgw::StreamInfo sinfo;
        sinfo.binfo.id = "123abc";
        sinfo.binfo.mid = sinfo.binfo.id;
        sinfo.binfo.stream_type = rtcgw::STREAM_VIDEO;
        sinfo.dinfo.direction = rtcgw::StreamDirection::SENDRECV;
        sinfo.dinfo.codecs.push_back(rtcgw::CodecParams{"VP8", 99, rtcgw::CodecType::CODEC_VIDEO});
        sinfo.dinfo.ssrcs.push_back(12345678);
        sinfo.dinfo.cname = "abcdefahikl";
        minfo.sinfos.push_back(sinfo);

        rtcgw::TransportInfo tinfo;
        tinfo.binfo.type = rtcgw::TransportType::ICE;
        tinfo.ice_param.ice_role = 0;//controlling
        tinfo.binfo.stream_id = sinfo.binfo.id;
        minfo.tinfos.push_back(tinfo);
    }
    if( media_type == rtcgw::STREAM_AUDIO ){
        rtcgw::StreamInfo sinfo;
        sinfo.binfo.id = "456def";
        sinfo.binfo.mid = sinfo.binfo.id;
        sinfo.binfo.stream_type = rtcgw::STREAM_AUDIO;
        sinfo.dinfo.direction = rtcgw::StreamDirection::SENDRECV;
        sinfo.dinfo.codecs.push_back(rtcgw::CodecParams{"opus", 120, rtcgw::CodecType::CODEC_AUDIO});
        sinfo.dinfo.ssrcs.push_back(23456789);
        sinfo.dinfo.cname = "abcdefahikl";
        minfo.sinfos.push_back(sinfo);

        rtcgw::TransportInfo tinfo;
        tinfo.binfo.type = rtcgw::TransportType::ICE;
        tinfo.ice_param.ice_role = 0;//controlling
        tinfo.binfo.stream_id = sinfo.binfo.mid;
        tinfo.ice_param.ice_role = 0;
        tinfo.ice_param.ice_ufrag ="test";
        tinfo.ice_param.ice_pwd = "test123";
        tinfo.binfo.policy = rtcgw::DTLS_SRTP;
        minfo.tinfos.push_back(tinfo);
    }
}

int main(int argc, char *argv[])
{

    rtcgw::MediaInfo minfo;
    addMediaInfo(rtcgw::STREAM_AUDIO, minfo);
    addMediaInfo(rtcgw::STREAM_VIDEO, minfo);

    std::string sdp = rtcgw::convertMeidaInfo2Sdp(minfo);

    cout<<sdp<<endl;
    return 0;
}
