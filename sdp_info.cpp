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
        sinfo.binfo.stream_type = STREAM_AUDIO;
        break;
    case cricket::MediaType::MEDIA_TYPE_VIDEO:
        sinfo.binfo.stream_type = STREAM_VIDEO;
        break;
    case cricket::MediaType::MEDIA_TYPE_DATA:
        sinfo.binfo.stream_type = STREAM_DATA;
        break;
    default:
        sinfo.binfo.stream_type = -1;
    }
    sinfo.dinfo.is_rtcpmux = info.media_description()->rtcp_mux();
    sinfo.dinfo.is_rtcp_reduced_size = info.media_description()->rtcp_reduced_size();
    sinfo.dinfo.direction = (StreamDirection)info.media_description()->direction();
    for( auto streams : info.media_description()->streams()){
        for(auto ssrc : streams.ssrcs){
            sinfo.dinfo.ssrcs.push_back(ssrc) ;
        }
    }
    if(sinfo.binfo.stream_type == STREAM_VIDEO){
        const cricket::VideoContentDescription* vcodec = info.media_description()->as_video();
        for(const auto& codec : vcodec->codecs()){
            CodecParams cparam;
            cparam.name = codec.name;
            cparam.payload_type = codec.id;
            cparam.codec_type = CodecType::CODEC_VIDEO;
            cparam.clockrate = codec.clockrate;
            for(const auto& fbparam : codec.feedback_params.params()){
                cparam.feedback_params.push_back(rtcgw::FeedbackParam{fbparam.id(), fbparam.param()});
            }
            cparam.fmtps = codec.params;
            sinfo.dinfo.codecs.push_back(cparam);
        }

    }

    if(sinfo.binfo.stream_type == STREAM_AUDIO){
        const cricket::AudioContentDescription* acodec = info.media_description()->as_audio();
        for(const auto& codec : acodec->codecs()){
            CodecParams cparam;
            cparam.name = codec.name;
            cparam.payload_type = codec.id;
            cparam.codec_type = CodecType::CODEC_AUDIO;
            cparam.clockrate = codec.clockrate;
            cparam.channels = codec.channels;
            cparam.bitrate = codec.bitrate;
            for(const auto& fbparam : codec.feedback_params.params()){
                cparam.feedback_params.push_back(rtcgw::FeedbackParam{fbparam.id(), fbparam.param()});
            }
            cparam.fmtps = codec.params;
            sinfo.dinfo.codecs.push_back(cparam);
        }
    }

    return sinfo;
}

static TransportInfo ConverContentInfoToTransportParams(const cricket::ContentInfo& info)
{
    TransportInfo tinfo;
    tinfo.binfo.is_media = info.media_description()->type()==cricket::MediaType::MEDIA_TYPE_DATA ? false :true;
    tinfo.binfo.is_rtcp_mux = info.media_description()->rtcp_mux();
    tinfo.binfo.stream_id = info.mid();

    // if(info.media_description()->cryptos().size() > 0){
    //     for(const auto& c : info.media_description()->cryptos()){
    //         CryptoInfo ci;
    //         ci.tag = c.tag;
    //         ci.cipher_suite = c.cipher_suite;
    //         ci.key_params = c.key_params;
    //         ci.session_params = c.session_params;
    //         tinfo.cryptos.push_back(ci);
    //     }
    //     tinfo.binfo.policy = TransportPolicy::SRTP;
    // }

    return tinfo;
}

static GroupInfos ConvertContentGroupToGroupInfos(const cricket::ContentGroups& groups)
{
    GroupInfos ginfos;
    for(const auto& g : groups){
        ginfos.push_back({g.semantics(), g.content_names()});
    }

    return ginfos;
}

static std::tuple<GroupInfos, StreamInfos, TransportInfos> parseSdp(const std::string& sdp)
{
    StreamInfos sinfos;
    TransportInfos tinfos;
    GroupInfos ginfos;
    webrtc::SdpParseError error;
    std::unique_ptr<webrtc::SessionDescriptionInterface> descinterface =
        webrtc::CreateSessionDescription(webrtc::SdpType::kOffer, sdp, &error);
    if (!descinterface) {
       //LogE() << "Can't parse received session description message. "
       //                   "SdpParseError was: "
       //                << error.description<<std::endl;
      return std::make_tuple(ginfos, sinfos, tinfos);
    }

    cricket::SessionDescription* desc = descinterface->description();
    for(size_t i=0; i<desc->contents().size(); i++){
        cricket::ContentInfo content = desc->contents()[i];
        StreamInfo sinfo = ConverContentInfoToStreamParams(content);
        sinfo.binfo.mid = content.mid();
        sinfos.push_back(sinfo);

        TransportInfo tinfo = ConverContentInfoToTransportParams(content);
        tinfo.ice_param.ice_role = cricket::ICEROLE_CONTROLLED;
        tinfo.binfo.type = TransportType::ICE;
        tinfo.ice_param.ice_ufrag = desc->transport_infos()[i].description.ice_ufrag;
        tinfo.ice_param.ice_pwd = desc->transport_infos()[i].description.ice_pwd;
        if(desc->transport_infos()[i].description.identity_fingerprint != nullptr){
            tinfo.fingerprint_param.fingerprint_alg = desc->transport_infos()[i].description.identity_fingerprint->algorithm;
            tinfo.fingerprint_param.fingerprint = desc->transport_infos()[i].description.identity_fingerprint->GetRfc4572Fingerprint();
            tinfo.binfo.policy = TransportPolicy::DTLS_SRTP;
        }

        tinfos.push_back(tinfo);
    }
    ginfos = ConvertContentGroupToGroupInfos(desc->groups());

    return std::make_tuple(ginfos, sinfos, tinfos);
}

std::string streamInfoToSdp(const GroupInfos& ginfos, const StreamInfos& sinfos, const TransportInfos& tinfos)
{
    std::unique_ptr<cricket::SessionDescription> sdesc = std::make_unique<cricket::SessionDescription>();

    sdesc->set_extmap_allow_mixed(true);
    sdesc->set_msid_signaling(cricket::kMsidSignalingSsrcAttribute);

    for(const auto& g : ginfos){
        cricket::ContentGroup group(g.group_name);
        for(const auto& s : g.streams){
            group.AddContentName(s);
        }
        sdesc->AddGroup(group);
    }

    for(size_t i=0; i<sinfos.size(); i++){
        StreamInfo sinfo = sinfos[i];
        std::string protocol="UDP/TLS/RTP/SAVPF";
        if(tinfos[i].binfo.policy == TransportPolicy::SRTP){
            protocol = "RTP/SAVPF";
        }
        else if(tinfos[i].binfo.policy == TransportPolicy::UNENCRYPTED){
            protocol = "RTP/AVPF";
        }
        std::unique_ptr<cricket::MediaContentDescription> content;
        if(sinfo.binfo.stream_type == STREAM_AUDIO){
            auto audioc = new cricket::AudioContentDescription();
            content.reset(audioc);
            audioc->set_protocol(protocol);

            for(const auto& cp: sinfo.dinfo.codecs){
                cricket::Codec c = cricket::CreateAudioCodec(cp.payload_type,
                    cp.name,
                    cp.clockrate,
                    cp.channels);
                c.bitrate = cp.bitrate;
                for(const auto& fbparam : cp.feedback_params){
                    c.feedback_params.Add( {fbparam.id, fbparam.param} );
                }
                c.params = cp.fmtps;
                audioc->AddCodec(c);
            }
        }
        else if(sinfo.binfo.stream_type == STREAM_VIDEO){
            auto videoc = new cricket::VideoContentDescription();
            content.reset(videoc);
            videoc->set_protocol(protocol);
            for(const auto& cp: sinfo.dinfo.codecs){
                cricket::Codec c = cricket::CreateVideoCodec(cp.payload_type,
                cp.name);
                for(const auto& fbparam : cp.feedback_params){
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
            content->set_rtcp_mux(sinfo.dinfo.is_rtcpmux);
            content->set_rtcp_reduced_size(sinfo.dinfo.is_rtcp_reduced_size);
            content->set_rtcp_reduced_size(false);
            content->set_remote_estimate(false);
            // if(tinfos[i].binfo.policy == TransportPolicy::SRTP){
            //     cricket::CryptoParams crypto;
            //     content->AddCrypto(crypto);
            // }
            content->set_direction((webrtc::RtpTransceiverDirection)sinfo.dinfo.direction);
            cricket::StreamParams sp;
            for(auto issrc : sinfo.dinfo.ssrcs){
                sp.add_ssrc(issrc);
            }
            sp.cname = sinfo.dinfo.cname;
            sp.id = sinfo.binfo.mid;
            content->AddStream(sp);
            sdesc->AddContent(sinfo.binfo.mid
                             ,cricket::MediaProtocolType::kRtp
                             ,std::move(content));

        }
        TransportInfo tinfo = tinfos[i];
        cricket::TransportInfo trans_info;
        trans_info.description.ice_ufrag = tinfo.ice_param.ice_ufrag;
        trans_info.description.ice_pwd = tinfo.ice_param.ice_pwd;
        trans_info.content_name = tinfo.binfo.stream_id;
        trans_info.description.transport_options.push_back("trickle");
        trans_info.description.identity_fingerprint.reset(
                rtc::SSLFingerprint::CreateFromRfc4572(
                         tinfo.fingerprint_param.fingerprint_alg,
                         tinfo.fingerprint_param.fingerprint));
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
    std::tie(minfo.groups, minfo.sinfos, minfo.tinfos) = parseSdp(sdpstr);
    return minfo;
}

std::string convertMeidaInfo2Sdp(const MediaInfo &minfo)
{
    return streamInfoToSdp(minfo.groups, minfo.sinfos, minfo.tinfos);
}

}//rtcw

