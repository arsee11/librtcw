///transport.cpp

#include "transport.h"

///webrtc includes
#include <api/jsep.h>
#include <api/transport/datagram_transport_interface.h>
#include <call/rtp_packet_sink_interface.h>
#include <modules/rtp_rtcp/source/rtp_packet_received.h>
#include <p2p/base/basic_packet_socket_factory.h>
#include <p2p/base/dtls_transport.h>
#include <p2p/base/dtls_transport_internal.h>
#include <p2p/base/p2p_transport_channel.h>
#include <p2p/base/packet_transport_internal.h>
#include <p2p/client/basic_port_allocator.h>
#include <pc/dtls_srtp_transport.h>
#include <pc/rtp_transport.h>
#include <pc/srtp_transport.h>
#include "pc/webrtc_sdp.h"
#include <rtc_base/rtc_certificate.h>


#include <iostream>
using namespace std;

namespace rtcgw
{

class LowerTransport
{
public:
    std::unique_ptr<cricket::IceTransportInternal> _ice_transport;
    std::unique_ptr<cricket::DtlsTransportInternal> _packet_transport;
};

class RtpRtcpTransport : public Transport
        ,public webrtc::RtpPacketSinkInterface
        ,public sigslot::has_slots<sigslot::multi_threaded_local>
{
public:
    RtpRtcpTransport(const TransportInfo& tinfo, rtc::Thread* netthread)
        :_transport_info(tinfo)
        ,_netthread(netthread)
    {
        _stream_id = tinfo.mid;
    }

    virtual std::string address()override{
        std::string str="{local:"+ _local_candidate.ToString()+"  remote:"+_remote_candidate.ToString()+"}";
        return str;
    }

    void lower_rtp_transport(std::unique_ptr<LowerTransport>&& val){
        _lower_rtp_transport=std::move(val);
        _lower_rtp_transport->_ice_transport->SignalCandidateGathered.connect(this, &RtpRtcpTransport::onCandidateGathered);
        _lower_rtp_transport->_ice_transport->SignalStateChanged.connect(this, &RtpRtcpTransport::onStateChange);
        _lower_rtp_transport->_ice_transport->SignalIceTransportStateChanged.connect(this, &RtpRtcpTransport::onIceTransportStateChanged);
        _lower_rtp_transport->_ice_transport->SignalCandidatePairChanged.connect(this, &RtpRtcpTransport::onCandidatePairChanged);
    }

    void lower_rtcp_transport(std::unique_ptr<LowerTransport>&& val){
        _lower_rtcp_transport=std::move(val);
        if(_lower_rtcp_transport != nullptr){
            _lower_rtcp_transport->_ice_transport->SignalCandidateGathered.connect(this, &RtpRtcpTransport::onCandidateGathered);
            _lower_rtcp_transport->_ice_transport->SignalStateChanged.connect(this, &RtpRtcpTransport::onStateChange);
            _lower_rtcp_transport->_ice_transport->SignalIceTransportStateChanged.connect(this, &RtpRtcpTransport::onIceTransportStateChanged);
            _lower_rtcp_transport->_ice_transport->SignalCandidatePairChanged.connect(this, &RtpRtcpTransport::onCandidatePairChanged);
        }
    }

    void holding_transport(std::unique_ptr<webrtc::RtpTransportInternal>&& val){
        _holding_transport = std::move(val);
        _holding_transport->SignalRtcpPacketReceived.connect(this, &RtpRtcpTransport::OnRtcpPacketReceived);
        _holding_transport->SignalReadyToSend.connect(this, &RtpRtcpTransport::OnReadyToSend);
        _holding_transport->SignalNetworkRouteChanged.connect(this, &RtpRtcpTransport::OnNetworkRouteChanged);
        _holding_transport->SignalWritableState.connect(this, &RtpRtcpTransport::OnWritableState);
        _holding_transport->SignalSentPacket.connect(this, &RtpRtcpTransport::OnSentPacket);

    }

    // Transport interface
public:
    void open()override{
        _netthread->Invoke<void>(RTC_FROM_HERE, [this](){this->open_n();});
    }
    void listenRtpBySsrc(const std::set<uint32_t> &ssrcs, TransportRecvRtpCallback listener)override{
        if(_holding_transport != nullptr){
            webrtc::RtpDemuxerCriteria criteria;
            criteria.ssrcs = ssrcs;
            _holding_transport->RegisterRtpDemuxerSink(criteria, this);
        }

        _rtp_cb = listener;
    }
    void listenRtcp(TransportRecvRtcpCallback listener)override{
        _rtcp_cb = listener;
    }
    void listenOnError(TransportErrorCallback listener)override{
        _error_cb = listener;
    }
    void listenOnEndpointInfo(EndpointInfoCallback listener)override{
        _conn_cb = listener;
    }
    void addRemoteEndpoint(const EndpointInfo &info)override{
       _netthread->Invoke<void>(RTC_FROM_HERE, [this, info](){
           this->addRemoteEndpoint_n(info);
       });
    }
    void sendRtp(const RtpPacket& packet)override{
        RtpRtcpBuffer buffer = packet.Buffer();
        postSendRtp(buffer);

    }
    void sendRtcp(const RtpRtcpBuffer& buffer)override{
        postSendRtcp(buffer);
    }

private:
    RtpRtcpTransport(const RtpRtcpTransport&);
    RtpRtcpTransport& operator=(const RtpRtcpTransport&);

    void open_n(){
        if(_lower_rtp_transport != nullptr){
            _lower_rtp_transport->_ice_transport->MaybeStartGathering();
        }

        if(_lower_rtcp_transport != nullptr){
            _lower_rtcp_transport->_ice_transport->MaybeStartGathering();
        }
    }

    void addRemoteEndpoint_n(const EndpointInfo &info){
        cricket::Candidate cand;
        webrtc::SdpParseError error;
        if( webrtc::ParseCandidate(info.specify_info, &cand, &error, true)){
            _lower_rtp_transport->_ice_transport->AddRemoteCandidate(cand);
        }
        else{
            //LogE()<<"Endpoint info parse failed:"<<error.description<<endl;
        }
    }

    // holding transport callbacks
    void OnRtcpPacketReceived(rtc::CopyOnWriteBuffer* packet, int64_t packet_time_us){
        if(_rtcp_cb != nullptr){
            _rtcp_cb(this, packet, packet_time_us);
        }
    }
    void OnReadyToSend(bool){
        cout<<"transport mid:"<<_transport_info.mid <<" ReadyToSend:"<<endl;
        cout<<"local cand="<<_local_candidate.ToString()<<endl;
        cout<<" remote cand="<<_remote_candidate.ToString()<<endl;
    }
    void OnNetworkRouteChanged(absl::optional<rtc::NetworkRoute>){
    }
    void OnWritableState(bool){
    }
    void OnSentPacket(const rtc::SentPacket&){
    }

    // RtpPacketSinkInterface interface
    void OnRtpPacket(const webrtc::RtpPacketReceived &packet) override{
        if(_rtp_cb != nullptr){
            _rtp_cb(this, packet);
        }
    }

    // ice transport state callbacks
    void onCandidateGathered(cricket::IceTransportInternal *channel, const cricket::Candidate &candidate){
        std::string str=webrtc::SdpSerializeCandidate(candidate);
        if(_conn_cb != nullptr){
            EndpointInfo cinfo;
            cinfo.type = EndpintType::CONN_ICE;
            cinfo.specify_info = str;
            _conn_cb(this, cinfo);
        }

        //cout<<__FUNCTION__<<":"<<str<<endl;
    }
    void onStateChange(cricket::IceTransportInternal *ice){
        //cricket::IceTransportState ice_state = ice->GetState();
        //cout<<__FUNCTION__<<": state="<<(int)ice_state<<endl;
    }
    void onIceTransportStateChanged(cricket::IceTransportInternal *ice){
        //webrtc::IceTransportState wice_state = ice->GetIceTransportState();
        //cout<<__FUNCTION__<<": ice transport state="<<(int)wice_state<<endl;
    }
    void onCandidatePairChanged(const cricket::CandidatePairChangeEvent& event){
        cout<<__FUNCTION__<<":\n";
        cout<<"local cand="<<event.selected_candidate_pair.local_candidate().ToString()<<endl;
        cout<<" remote cand="<<event.selected_candidate_pair.remote_candidate().ToString()<<endl;
        _local_candidate = event.selected_candidate_pair.local_candidate();
        _remote_candidate = event.selected_candidate_pair.remote_candidate();
    }

    void postSendRtp(const RtpRtcpBuffer& buffer){
        _netthread->PostTask(RTC_FROM_HERE, [this, buffer]{
            if(_holding_transport != nullptr){
                _holding_transport->SendRtpPacket((RtpRtcpBuffer*)&buffer,
                                                  rtc::PacketOptions(), cricket::PF_SRTP_BYPASS);
            }
        });
    }

    void postSendRtcp(const RtpRtcpBuffer& buffer){
        _netthread->PostTask(RTC_FROM_HERE, [this, buffer]{
            if(_holding_transport != nullptr){
                _holding_transport->SendRtcpPacket((RtpRtcpBuffer*)&buffer,
                                                   rtc::PacketOptions(), cricket::PF_SRTP_BYPASS);
            }
        });
    }

protected:
    std::unique_ptr<LowerTransport> _lower_rtp_transport;
    std::unique_ptr<LowerTransport> _lower_rtcp_transport;
    std::unique_ptr<webrtc::RtpTransportInternal> _holding_transport;
    cricket::Candidate _local_candidate;
    cricket::Candidate _remote_candidate;
    TransportInfo _transport_info;
    rtc::Thread* _netthread=nullptr;
};





////////////////////////////////////////////////////////////////////////////////////////////////////////
std::unique_ptr<cricket::IceTransportInternal> CreateIceTransport(bool is_rtcp, const TransportInfo& param
                                                                  ,const std::set<rtc::SocketAddress>& stuns
                                                                  ,const std::vector<cricket::RelayServerConfig> turns
                                                                  ,rtc::Thread* netthread)
{
    int component = is_rtcp ? cricket::ICE_CANDIDATE_COMPONENT_RTCP
                            : cricket::ICE_CANDIDATE_COMPONENT_RTP;

    //TODO: move to global scope or TransportLayer scope
    rtc::NetworkManager* netmgr = new rtc::BasicNetworkManager();
    rtc::BasicPacketSocketFactory* pack_factory = new rtc::BasicPacketSocketFactory(netthread);
    cricket::PortAllocator* alloc =new cricket::BasicPortAllocator(netmgr, pack_factory);

    alloc->Initialize();
    alloc->SetPortRange(2000, 3000);
    alloc->set_flags(cricket::PORTALLOCATOR_DISABLE_TCP|cricket::PORTALLOCATOR_ENABLE_IPV6);
    alloc->SetConfiguration(stuns, turns, 1, false);

    std::unique_ptr<cricket::P2PTransportChannel> p2p(new cricket::P2PTransportChannel("rtcgw", component, alloc));
    p2p->SetIceRole((cricket::IceRole)param.ice_role);
    p2p->SetIceConfig(cricket::IceConfig());
    p2p->SetIceTiebreaker(rtc::CreateRandomId64());
    p2p->SetIceCredentials(param.local_ice_ufrag, param.local_ice_pwd);
    p2p->SetRemoteIceCredentials(param.remote_ice_ufrag, param.remote_ice_pwd);

    return p2p;
}

std::unique_ptr<cricket::DtlsTransportInternal> CreateDtlsTransport(
    const TransportInfo& config,
    cricket::IceTransportInternal* ice,
    webrtc::DatagramTransportInterface* datagram_transport) {

    std::unique_ptr<cricket::DtlsTransportInternal> dtls;

    if (datagram_transport) {
        RTC_DCHECK(config.use_datagram_transport ||
               config.use_datagram_transport_for_data_channels);
    } else {
        webrtc::CryptoOptions opt;
        dtls = std::make_unique<cricket::DtlsTransport>(ice, opt, nullptr);
    }

    RTC_DCHECK(dtls);
    dtls->SetSslMaxProtocolVersion(rtc::SSLProtocolVersion::SSL_PROTOCOL_TLS_12);
    dtls->ice_transport()->SetIceRole((cricket::IceRole)config.ice_role);
    //dtls->ice_transport()->SetIceTiebreaker(ice_tiebreaker_);
    //dtls->ice_transport()->SetIceConfig(ice_config_);
    dtls->SetDtlsRole(rtc::SSL_CLIENT);
    if (config.certificate != nullptr) {
        rtc::RTCCertificate* cert = static_cast<rtc::RTCCertificate*>(config.certificate);
        bool ok = dtls->SetLocalCertificate( rtc::scoped_refptr<rtc::RTCCertificate>(cert));
        RTC_DCHECK(ok);
        std::unique_ptr<rtc::SSLFingerprint> sslfp = rtc::SSLFingerprint::CreateUniqueFromRfc4572(config.fingerprint_alg, config.fingerprint);
        dtls->SetRemoteFingerprint(sslfp->algorithm, sslfp->digest.cdata(), sslfp->digest.size());
    }

    return dtls;
}

std::unique_ptr<webrtc::RtpTransport> CreateUnencryptedRtpTransport(
      const std::string& transport_name,
      rtc::PacketTransportInternal* rtp_packet_transport,
      rtc::PacketTransportInternal* rtcp_packet_transport)
{
    auto unencrypted_rtp_transport =
        std::make_unique<webrtc::RtpTransport>(rtcp_packet_transport == nullptr);

    unencrypted_rtp_transport->SetRtpPacketTransport(rtp_packet_transport);
    if (rtcp_packet_transport) {
        unencrypted_rtp_transport->SetRtcpPacketTransport(rtcp_packet_transport);
    }
    return unencrypted_rtp_transport;
}

std::unique_ptr<webrtc::SrtpTransport>CreateSdesTransport(
      const std::string& transport_name,
      cricket::DtlsTransportInternal* rtp_dtls_transport,
      cricket::DtlsTransportInternal* rtcp_dtls_transport)
{
    auto srtp_transport =
        std::make_unique<webrtc::SrtpTransport>(rtcp_dtls_transport == nullptr);
    RTC_DCHECK(rtp_dtls_transport);
    srtp_transport->SetRtpPacketTransport(rtp_dtls_transport);
    if (rtcp_dtls_transport) {
        srtp_transport->SetRtcpPacketTransport(rtcp_dtls_transport);
    }
    if (false/*config.enable_external_auth*/) {
        srtp_transport->EnableExternalAuth();
    }
    return srtp_transport;
}

std::unique_ptr<webrtc::DtlsSrtpTransport> CreateDtlsSrtpTransport(
      const TransportInfo& config,
      cricket::DtlsTransportInternal* rtp_dtls_transport,
      cricket::DtlsTransportInternal* rtcp_dtls_transport)
{
    auto dtls_srtp_transport = std::make_unique<webrtc::DtlsSrtpTransport>(
        rtcp_dtls_transport == nullptr);
    if (config.enable_external_auth) {
        dtls_srtp_transport->EnableExternalAuth();
    }

    dtls_srtp_transport->SetDtlsTransports(rtp_dtls_transport,
                                           rtcp_dtls_transport);
    dtls_srtp_transport->SetActiveResetSrtpParams(
        config.active_reset_srtp_params);

//    dtls_srtp_transport->SignalDtlsStateChange.connect(
//        this, &JsepTransportController::UpdateAggregateStates_n);

    return dtls_srtp_transport;
}

std::unique_ptr<rtc::Thread> netthread;
bool TransportLayer::startNThreads(int N)
{
    netthread = rtc::Thread::CreateWithSocketServer();
    std::string name = "net_thread_"+std::to_string(0);
    netthread->SetName(name, nullptr);
    netthread->Start();
    return true;
}

bool TransportLayer::stopAllThreads()
{
    netthread->Stop();

}

rtc::Thread* selectOneThread()
{
    return netthread.get();
}

transport_ptr rtcgw::TransportLayer::createTransport(const TransportInfo &params)
{
    transport_ptr trans;
    rtc::Thread* thr = selectOneThread();
    trans = thr->Invoke<transport_ptr>(RTC_FROM_HERE, [this, params, thr](){
        return this->createTransport_n(params, thr);
    });

    return trans;
}

transport_ptr rtcgw::TransportLayer::createTransport_n(const TransportInfo &params, void *thr)
{
    rtc::Thread* netthr = static_cast<rtc::Thread*>(thr);

    if (params.certificate !=nullptr && !params.cryptos.empty()) {
        //Log()<<"SDES and DTLS-SRTP cannot be enabled at the same time.";
        return nullptr;
    }

    std::unique_ptr<cricket::IceTransportInternal> ice;
    std::unique_ptr<cricket::IceTransportInternal> rtcp_ice;
    std::unique_ptr<cricket::DtlsTransportInternal> rtcp_dtls_transport;

    if(params.is_ice){
        std::set<rtc::SocketAddress> stuns;
        //stuns.insert( rtc::SocketAddress(stun_server, stun_port));
        std::vector<cricket::RelayServerConfig> turns;
        //turns.push_back( cricket::RelayServerConfig(rtc::SocketAddress(turn_server, turn_port)
        //                                ,turn_user, turn_pwd, proto)
        //);
        ice = CreateIceTransport(false, params, stuns, turns, netthr);

        if ( !params.is_rtcp_mux ) {
            rtcp_ice = CreateIceTransport(true, params, stuns, turns, netthr);
            rtcp_dtls_transport = CreateDtlsTransport(params, rtcp_ice.get(), nullptr);
        }
    }

    std::unique_ptr<cricket::DtlsTransportInternal> rtp_dtls_transport =
    CreateDtlsTransport(params, ice.get(), nullptr);

    std::unique_ptr<webrtc::RtpTransport> unencrypted_rtp_transport;
    std::unique_ptr<webrtc::SrtpTransport> sdes_transport;
    std::unique_ptr<webrtc::DtlsSrtpTransport> dtls_srtp_transport;
    std::unique_ptr<webrtc::RtpTransportInternal> datagram_rtp_transport;

    if (params.policy == UNENCRYPTED) {
       //Log()<<"Creating UnencryptedRtpTransport, becayse encryption is disabled.";
        unencrypted_rtp_transport = CreateUnencryptedRtpTransport(
            params.mid, rtp_dtls_transport.get(), rtcp_dtls_transport.get());
    }
    else if (!params.cryptos.empty()) {//srtp
        sdes_transport = CreateSdesTransport(
            params.mid, rtp_dtls_transport.get(), rtcp_dtls_transport.get());
        //Log() << "Creating SdesTransport.";
    }
    else { //dtls-srtp
        //Log()<< "Creating DtlsSrtpTransport.";
        dtls_srtp_transport = CreateDtlsSrtpTransport(
            params, rtp_dtls_transport.get(), rtcp_dtls_transport.get());
    }

    RtpRtcpTransport* transport = new RtpRtcpTransport(params, netthr);
    if( !params.is_rtcp_mux ){
        LowerTransport* lrtcp = new LowerTransport;
        lrtcp->_ice_transport = std::move(rtcp_ice);
        lrtcp->_packet_transport = std::move(rtcp_dtls_transport);

        transport->lower_rtcp_transport(std::unique_ptr<LowerTransport>(lrtcp));
    }
    LowerTransport* lrtp = new LowerTransport;
    lrtp->_ice_transport = std::move(ice);
    lrtp->_packet_transport = std::move(rtp_dtls_transport);
    transport->lower_rtp_transport(std::unique_ptr<LowerTransport>(lrtp));

    if(unencrypted_rtp_transport != nullptr){
        transport->holding_transport( std::move(unencrypted_rtp_transport) );
    }
    else if(sdes_transport != nullptr){
        transport->holding_transport( std::move(sdes_transport) );
    }
    else{
        transport->holding_transport( std::move(dtls_srtp_transport) );
    }
    transport->info(params);
    return transport_ptr(transport);
}


}//rtcgw
