#include "json_signaling_client.h"
#include "signaling.h"
#include "session/stream.h"

///webrtc includes
//#include <third_party/jsoncpp/json.h>
//#include <rtc_base/strings/json.h>
#include <pc/session_description.h>
#include <rtc_base/message_digest.h>
#include <api/jsep.h>
#include <api/jsep_session_description.h>
#include <rtc_base/rtc_certificate_generator.h>


#include <iostream>
using namespace  std;

namespace rtcgw {


JsonSignalingClient::JsonSignalingClient(Signaling* s, const std::string &server_ip, uint16_t server_port,
                                         NetEventQueue *evt_queue, ThreadScopePolling *thr)
    :SignalingClient(s)
    ,_server_ip(server_ip)
    ,_server_port(server_port)
    ,_evt_queue(evt_queue)
    ,_connector(_evt_queue, 0)
    ,_thread_scope(thr)
{
    using std::placeholders::_1;
    _parser.setDispatcher(
        CmdDispatcher<AnswerCmd>{std::bind(&JsonSignalingClient::onRecvAnswer, this, _1)}
    );
    _parser.setDispatcher(
        CmdDispatcher<CandidateCmd>{std::bind(&JsonSignalingClient::onRecvCandidate, this, _1)}
    );
    _parser.setDispatcher(
        CmdDispatcher<SiginOKCmd>{std::bind(&JsonSignalingClient::onSignedIn, this, _1)}
    );


}

JsonSignalingClient::~JsonSignalingClient()
{
    close();
}

bool JsonSignalingClient::open()
{
    _conn = _connector.connect({_server_ip, _server_port});
    if(_conn == nullptr){
        return false;
    }
    _conn->listenOnRecv([this](const ConnectionPtr& conn, const arsee::net::MsgSt& buf){
        if(buf.size() > 0){
            this->onMessageFromSvr(buf.rd_ptr(), buf.size());
        }
    });

    using std::placeholders::_1;
    using std::placeholders::_2;
    using std::placeholders::_3;
    _signaling->listenResponse(std::bind(&JsonSignalingClient::onCreateSessionResponse, this, _1, _2, _3));
    _signaling->listenResponse(std::bind(&JsonSignalingClient::onStreamEndpointResponse, this, _1, _2));

    return true;
}


StreamInfo ConverContentInfoToStreamParams(const cricket::ContentInfo& info)
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

TransportInfo ConverContentInfoToTransportParams(const cricket::ContentInfo& info)
{
    TransportInfo tinfo;
    tinfo.is_media = info.media_description()->type()==cricket::MediaType::MEDIA_TYPE_DATA ? false :true;
    tinfo.is_rtcp_mux = info.media_description()->rtcp_mux();
    tinfo.mid = info.mid();

    if(info.media_description()->cryptos().size() == 0){
        int64_t expires_ms= 24*60*60*1000; //1 day
        rtc::scoped_refptr<rtc::RTCCertificate> cert =
                rtc::RTCCertificateGenerator::GenerateCertificate(rtc::KeyParams(rtc::KeyType::KT_RSA), expires_ms);
        tinfo.certificate = cert.release();

    }
    else{
        for(auto c : info.media_description()->cryptos()){
            CryptoInfo ci;
            ci.tag = c.tag;
            ci.cipher_suite = c.cipher_suite;
            ci.key_params = c.key_params;
            ci.session_params = c.session_params;
            tinfo.cryptos.push_back(ci);
        }
    }

    return tinfo;
}

std::tuple<StreamInfos, TransportInfos> pasreSdp(const std::string& sdp)
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
        tinfo.policy = DTLS_SRTP;
        tinfo.is_ice = true;
        tinfo.local_ice_ufrag = rtc::CreateRandomString(cricket::ICE_UFRAG_LENGTH);
        tinfo.local_ice_pwd = rtc::CreateRandomString(cricket::ICE_PWD_LENGTH);
        tinfo.remote_ice_ufrag = desc->transport_infos()[i].description.ice_ufrag;
        tinfo.remote_ice_pwd = desc->transport_infos()[i].description.ice_pwd;
        tinfo.fingerprint_alg = desc->transport_infos()[i].description.identity_fingerprint->algorithm;
        tinfo.fingerprint = desc->transport_infos()[i].description.identity_fingerprint->GetRfc4572Fingerprint();
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
        std::unique_ptr<cricket::MediaContentDescription> content;
        if(sinfo.stream_type == STREAM_AUDIO){
            auto audioc = new cricket::AudioContentDescription();
            content.reset(audioc);
            audioc->set_protocol("UDP/TLS/RTP/SAVPF");

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
            videoc->set_protocol("UDP/TLS/RTP/SAVPF");
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
            cricket::CryptoParams crypto;
            content->AddCrypto(crypto);
            content->set_direction((webrtc::RtpTransceiverDirection)sinfo.direction);
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
        trans_info.description.identity_fingerprint =
                rtc::SSLFingerprint::CreateFromCertificate(*static_cast<rtc::RTCCertificate*>(tinfo.certificate));
        sdesc->AddTransportInfo(trans_info);

    }

    std::string session_id="123456";
    std::string session_version="2";
    webrtc::JsepSessionDescription desc(webrtc::SdpType::kAnswer, std::move(sdesc), session_id, session_version);
    std::string sdp;
    desc.ToString(&sdp);
    return sdp;
}

void JsonSignalingClient::onCreateSessionResponse(int error, const StreamInfos& sinfos, const TransportInfos& tinfos)
{
    std::string answer = streamInfoToSdp(sinfos, tinfos);
    if( !answer.empty()){
        AnswerCmd cmd;
        cmd.from = _myname;
        cmd.to = _peer_name;
        cmd.sdp = answer;
        auto e = cmd.encoder();
        sendMessage(e->buf(), e->size());
    }
}

void JsonSignalingClient::onStreamEndpointResponse(const std::string& stream_id, const EndpointInfo& ep)
{
    //std::string candidate = streamEndpointToSdp(stream_id, ep);
    CandidateCmd cmd;
    cmd.from = _myname;
    cmd.to = _peer_name;
    cmd.mid = stream_id;
    cmd.candidate = ep.specify_info;
    auto e = cmd.encoder();
    sendMessage(e->buf(), e->size());
}

void JsonSignalingClient::sendMessage(const void* buf, int size)
{
    if(_conn != nullptr){
        _conn->write(arsee::net::MsgSt(buf, size));
    }
    else{
        //log<<"can not send conn is not ready";
    }
}

std::string newTransactionId()
{
    static int id=0;
    return std::to_string(id++);
}

void JsonSignalingClient::close()
{
}

void JsonSignalingClient::signIn(const string &user)
{
    //TODO: post to signal thread scope
    _myname = user;
    SiginCmd cmd(user, "123456", "rtcgate");
    cmd.id = newTransactionId();
    auto e = cmd.encoder();
    sendMessage(e->buf(), e->size());
}

void JsonSignalingClient::signOut()
{
    //TODO: post to signal thread scope

}

std::string JsonSignalingClient::createDialog()
{
    //TODO: post to signal thread scope

}

void JsonSignalingClient::joinDialog(const std::string& dialog_id)
{
    //TODO: post to signal thread scope

}

void rtcgw::JsonSignalingClient::inviteToDialog(const std::string &dialog_id)
{
    //TODO: post to signal thread scope

}

void JsonSignalingClient::onInvitedToDailog(const OfferCmd& cmd)
{
    _peer_name = cmd.from;
    std::string sid = cmd.from;
    bool isok = _signaling->onCreateSession(cmd.dialog_id, sid);
    if(isok){
        StreamInfos sinfos;
        TransportInfos tinfos;
        std::tie(sinfos, tinfos) = pasreSdp(cmd.sdp);
        StreamInfos local_sinfos;
        for(auto i : sinfos){
            StreamInfo f = i;
            local_sinfos.push_back(f);
        }
        _signaling->onAddStream(sid, local_sinfos, tinfos);
        _signaling->onSetRemoteStream(sid, sinfos);
    }
}

void JsonSignalingClient::leaveDialog()
{
    //TODO: post to signal thread scope

}

void JsonSignalingClient::onMessageFromSvr(const uint8_t *msg, int size)
{
    cout<<__FUNCTION__<<"(size="<<size<<")"<<endl;
    cmd_ptr req = _parser.parse((const char*)msg, size);
    cmd_ptr rsp = req->dispatch(this);
    if(rsp != nullptr){
        auto cmde = rsp->encoder();
        sendMessage(cmde->buf(), cmde->size());
    }
}

cmd_ptr JsonSignalingClient::onSignedIn(Command* req)
{
    cout<<"Login to signaling server successfully."<<endl;
    return nullptr;
}

cmd_ptr JsonSignalingClient::onRecvAnswer(Command* req)
{
    AnswerCmd* cmd = static_cast<AnswerCmd*>(req);
    if(_signaling != nullptr){
        StreamInfos sinfos;
        TransportInfos tinfos;
        std::tie(sinfos, tinfos) = pasreSdp(cmd->sdp);
        _signaling->onSetRemoteStream(cmd->from, sinfos);
    }

    return nullptr;
}

cmd_ptr JsonSignalingClient::onRecvCandidate(Command *req)
{
    CandidateCmd* cmd = static_cast<CandidateCmd*>(req);

    std::string sid = cmd->from;
    if(_signaling != nullptr){
          EndpointInfo cinfo;
          cinfo.type = EndpintType::CONN_ICE;
          cinfo.specify_info = cmd->candidate;
          _signaling->onRemoteStreamEndpoint(sid, cmd->mid, cinfo);
    }

    return nullptr;
}



}//rtcgw
