///sdp_info.cpp

#include "sdp_info.h"

///webrtc includes
#include <pc/session_description.h>
#include <rtc_base/message_digest.h>
#include <api/jsep.h>
#include <api/jsep_session_description.h>

namespace rtcgw {


static StreamInfo ConverContentInfoToStreamParams(const cricket::ContentInfo& info)
{
    StreamInfo sinfo;
    switch (info.media_description()->type()) {
    case cricket::MediaType::MEDIA_TYPE_AUDIO:
        sinfo.stream_type = STREAM_AUDIO;
        break;
    case cricket::MediaType::MEDIA_TYPE_VIDEO:
        sinfo.stream_type = STREAM_VIDEO;
        break;
    case cricket::MediaType::MEDIA_TYPE_DATA:
        sinfo.stream_type = STREAM_DATA;
        break;
    default:
        sinfo.stream_type = -1;
    }
    sinfo.is_rtcpmux = info.media_description()->rtcp_mux();
    sinfo.is_rtcp_reduced_size = info.media_description()->rtcp_reduced_size();
    sinfo.direction = (StreamDirection)info.media_description()->direction();
    for( auto streams : info.media_description()->streams()){
        for(auto ssrc : streams.ssrcs){
            sinfo.ssrcs.insert(ssrc) ;
        }
    }
    if(sinfo.stream_type == STREAM_VIDEO){
        const cricket::VideoContentDescription* vcodec = info.media_description()->as_video();
        for(auto codec : vcodec->codecs()){
            CodecParams cparam{codec.name, codec.id, CodecType::CODEC_VIDEO,{},{}};
            for(auto fbparam : codec.feedback_params.params()){
                cparam.feedback_params.push_back(rtcgw::FeedbackParam{fbparam.id(), fbparam.param()});
            }
            cparam.fmtps = codec.params;
            sinfo.codecs.push_back(cparam);
        }

    }

    if(sinfo.stream_type == STREAM_AUDIO){
        const cricket::AudioContentDescription* acodec = info.media_description()->as_audio();
        for(auto codec : acodec->codecs()){
            CodecParams cparam{codec.name, codec.id, CodecType::CODEC_AUDIO,{},{}};
            for(auto fbparam : codec.feedback_params.params()){
                cparam.feedback_params.push_back(rtcgw::FeedbackParam{fbparam.id(), fbparam.param()});
            }
            cparam.fmtps = codec.params;
            sinfo.codecs.push_back(cparam);
        }
    }

    return sinfo;
}

static TransportInfo ConverContentInfoToTransportParams(const cricket::ContentInfo& info)
{
    TransportInfo tinfo;
    tinfo.is_media = info.media_description()->type()==cricket::MediaType::MEDIA_TYPE_DATA ? false :true;
    tinfo.is_rtcp_mux = info.media_description()->rtcp_mux();
    tinfo.mid = info.mid();

    if(info.media_description()->cryptos().size() > 0){
        for(auto c : info.media_description()->cryptos()){
            CryptoInfo ci;
            ci.tag = c.tag;
            ci.cipher_suite = c.cipher_suite;
            ci.key_params = c.key_params;
            ci.session_params = c.session_params;
            tinfo.cryptos.push_back(ci);
        }
        tinfo.policy = TransportPolicy::SRTP;
    }

    return tinfo;
}

static std::tuple<StreamInfos, TransportInfos> parseSdp(const std::string& sdp)
{
    StreamInfos sinfos;
    TransportInfos tinfos;
    webrtc::SdpParseError error;
    std::unique_ptr<webrtc::SessionDescriptionInterface> descinterface =
        webrtc::CreateSessionDescription(webrtc::SdpType::kOffer, sdp, &error);
    if (!descinterface) {
       //LogE() << "Can't parse received session description message. "
       //                   "SdpParseError was: "
       //                << error.description<<std::endl;
      return std::make_tuple(sinfos, tinfos);
    }

    cricket::SessionDescription* desc = descinterface->description();
    for(size_t i=0; i<desc->contents().size(); i++){
        cricket::ContentInfo content = desc->contents()[i];
        StreamInfo sinfo = ConverContentInfoToStreamParams(content);
        sinfo.id = content.mid();
        sinfos.push_back(sinfo);

        TransportInfo tinfo = ConverContentInfoToTransportParams(content);
        tinfo.ice_role = cricket::ICEROLE_CONTROLLED;
        tinfo.type = TransportType::ICE;
        tinfo.local_ice_ufrag = rtc::CreateRandomString(cricket::ICE_UFRAG_LENGTH);
        tinfo.local_ice_pwd = rtc::CreateRandomString(cricket::ICE_PWD_LENGTH);
        tinfo.remote_ice_ufrag = desc->transport_infos()[i].description.ice_ufrag;
        tinfo.remote_ice_pwd = desc->transport_infos()[i].description.ice_pwd;
        if(desc->transport_infos()[i].description.identity_fingerprint != nullptr){
            tinfo.fingerprint_alg = desc->transport_infos()[i].description.identity_fingerprint->algorithm;
            tinfo.fingerprint = desc->transport_infos()[i].description.identity_fingerprint->GetRfc4572Fingerprint();
            tinfo.policy = TransportPolicy::DTLS_SRTP;
        }

        tinfos.push_back(tinfo);
    }

    return std::make_tuple(sinfos, tinfos);
}

std::string streamInfoToSdp(const StreamInfos& sinfos, const TransportInfos& tinfos)
{
    std::unique_ptr<cricket::SessionDescription> sdesc = std::make_unique<cricket::SessionDescription>();

    sdesc->set_extmap_allow_mixed(true);
    sdesc->set_msid_signaling(cricket::kMsidSignalingSsrcAttribute);

    for(size_t i=0; i<sinfos.size(); i++){
        StreamInfo sinfo = sinfos[i];
        std::string protocol="UDP/TLS/RTP/SAVPF";
        if(tinfos[i].policy == TransportPolicy::SRTP){
            protocol = "RTP/SAVPF";
        }
        else if(tinfos[i].policy == TransportPolicy::UNENCRYPTED){
            protocol = "RTP/AVPF";
        }
        std::unique_ptr<cricket::MediaContentDescription> content;
        if(sinfo.stream_type == STREAM_AUDIO){
            auto audioc = new cricket::AudioContentDescription();
            content.reset(audioc);
            audioc->set_protocol(protocol);

            for(auto cp: sinfo.codecs){
                cricket::AudioCodec c;
                c.id = cp.payload_type;
                c.name = cp.name;
                for(auto fbparam : cp.feedback_params){
                    c.feedback_params.Add( {fbparam.id, fbparam.param} );
                }
                c.params = cp.fmtps;
                audioc->AddCodec(c);
            }
        }
        else if(sinfo.stream_type == STREAM_VIDEO){
            auto videoc = new cricket::VideoContentDescription();
            content.reset(videoc);
            videoc->set_protocol(protocol);
            for(auto cp: sinfo.codecs){
                cricket::VideoCodec c;
                c.id = cp.payload_type;
                c.name = cp.name;
                for(auto fbparam : cp.feedback_params){
                    c.feedback_params.Add( {fbparam.id, fbparam.param} );
                }
                c.params = cp.fmtps;
                videoc->AddCodec(c);
            }
        }
        else{

        }
        if(content != nullptr){
            cricket::RtpHeaderExtensions rhext;
            content->set_rtp_header_extensions(rhext);
            content->set_rtcp_mux(sinfo.is_rtcpmux);
            content->set_rtcp_reduced_size(sinfo.is_rtcp_reduced_size);
            content->set_rtcp_reduced_size(false);
            content->set_remote_estimate(false);
            if(tinfos[i].policy == TransportPolicy::SRTP){
                cricket::CryptoParams crypto;
                content->AddCrypto(crypto);
            }
            content->set_direction((webrtc::RtpTransceiverDirection)sinfo.direction);
            cricket::StreamParams sp;
            for(auto issrc : sinfo.ssrcs){
                sp.add_ssrc(issrc);
            }
            sp.cname = sinfo.cname;
            sp.id = sinfo.id;
            content->AddStream(sp);
            sdesc->AddContent(sinfo.id
                             ,cricket::MediaProtocolType::kRtp
                             ,std::move(content));

        }
        TransportInfo tinfo = tinfos[i];
        cricket::TransportInfo trans_info;
        trans_info.description.ice_ufrag = tinfo.local_ice_ufrag;
        trans_info.description.ice_pwd = tinfo.local_ice_pwd;
        trans_info.content_name = tinfo.mid;
        trans_info.description.transport_options.push_back("trickle");
        trans_info.description.identity_fingerprint;
                rtc::SSLFingerprint::CreateFromRfc4572(tinfo.fingerprint_alg, tinfo.fingerprint);
        sdesc->AddTransportInfo(trans_info);

    }

    std::string session_id="123456";
    std::string session_version="2";
    webrtc::JsepSessionDescription desc(webrtc::SdpType::kAnswer, std::move(sdesc), session_id, session_version);
    std::string sdp;
    desc.ToString(&sdp);
    return sdp;
}

MediaInfo parseSdp2MeidaInfo(const std::string &sdpstr)
{
    MediaInfo minfo;
    std::tie(minfo.sinfos, minfo.tinfos) = parseSdp(sdpstr);
    return minfo;
}

std::string convertMeidaInfo2Sdp(const MediaInfo &minfo)
{
    return streamInfoToSdp(minfo.sinfos, minfo.tinfos);
}

}//rtcw

