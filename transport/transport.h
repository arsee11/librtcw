#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <string>
#include <memory>
#include <set>
#include <vector>

#include "rtputils/rtp_defines.h"

namespace rtcgw {

enum TransportPolicy{
    UNENCRYPTED,
    SRTP,
    DTLS_SRTP
};

struct CryptoInfo
{
    bool matches(const CryptoInfo& params) const {
      return (tag == params.tag && cipher_suite == params.cipher_suite);
    }

    int tag;
    std::string cipher_suite;
    std::string key_params;
    std::string session_params;

    struct{
        // Enable GCM crypto suites from RFC 7714 for SRTP. GCM will only be used
        // if both sides enable it.
        bool enable_gcm_crypto_suites = false;

        // If set to true, the (potentially insecure) crypto cipher
        // SRTP_AES128_CM_SHA1_32 will be included in the list of supported ciphers
        // during negotiation. It will only be used if both peers support it and no
        // other ciphers get preferred.
        bool enable_aes128_sha1_32_crypto_cipher = false;

        // The most commonly used cipher. Can be disabled, mostly for testing
        // purposes.
        bool enable_aes128_sha1_80_crypto_cipher = true;

        // If set to true, encrypted RTP header extensions as defined in RFC 6904
        // will be negotiated. They will only be used if both peers support them.
        bool enable_encrypted_rtp_header_extensions = false;
    } srtp;

};

enum EndpintType
{
    CONN_UNKNOWN=-1,
    CONN_ICE,
    CONN_UDP,
    CONN_TCP
};

struct EndpointInfo
{
    EndpintType type;
    std::string specify_info;
    uint16_t port;
};

struct TransportInfo
{
    TransportPolicy policy=UNENCRYPTED;
    bool is_media=true;  //is media or data
    bool is_ice=true;    //use ice or not
    bool is_rtcp_mux=true;
    bool use_datagram_transport=false;
    bool use_datagram_transport_for_data_channels=false;
    bool enable_external_auth=false;
    bool active_reset_srtp_params=false;

    int ice_role; //enum{ICEROLE_CONTROLLING = 0, ICEROLE_CONTROLLED, ICEROLE_UNKNOWN};
    void* certificate=nullptr;         //DTLS-SRTP config
    std::vector<CryptoInfo> cryptos; //SRTP config Note: certificate and cryptos were exclusion
    std::string mid;
    std::string local_ice_ufrag;
    std::string local_ice_pwd;
    std::string remote_ice_ufrag;
    std::string remote_ice_pwd;
    std::string ice_options;
    std::string fingerprint_alg;//RFC4572 fingerprint
    std::string fingerprint;    //RFC4572 fingerprint

};

using TransportInfos = std::vector<TransportInfo>;

class Transport;

using TransportRecvRtpCallback =std::function<void (Transport* trans, const RtpPacketReceived& packet)>;
using TransportRecvRtcpCallback =std::function<void (Transport* trans, const RtpRtcpBuffer* buffer, int64_t time_us)>;
using TransportErrorCallback = std::function<void (Transport* trans, int error, const std::string& error_str)>;
using EndpointInfoCallback = std::function<void (Transport* trans, const EndpointInfo& cinfo)>;

using transport_ptr=std::shared_ptr<Transport>;

class Transport
{
public:
    Transport(){}
    virtual ~Transport(){}

    std::string stream_id()const {return _stream_id; }
    TransportInfo info()const {return _info;}
    void info(const TransportInfo& val){ _info = val; }

    virtual std::string address()=0;
    virtual void open()=0;
    virtual void listenRtpBySsrc(const std::set<uint32_t>& ssrcs, TransportRecvRtpCallback listener)=0;
    virtual void listenRtcp(TransportRecvRtcpCallback listener)=0;
    virtual void listenOnError(TransportErrorCallback listener)=0;
    virtual void listenOnEndpointInfo(EndpointInfoCallback listener)=0;

    virtual void addRemoteEndpoint(const EndpointInfo& info)=0;
    virtual void sendRtp(const RtpPacket& packet)=0;
    virtual void sendRtcp(const RtpRtcpBuffer& buffer)=0;


private:
    //disable copy and asign operation
    Transport(const Transport&);
    Transport& operator=(const Transport&);

protected:
    std::string _stream_id;
    TransportInfo _info;
    TransportRecvRtpCallback _rtp_cb;
    TransportRecvRtcpCallback _rtcp_cb;
    TransportErrorCallback _error_cb;
    EndpointInfoCallback _conn_cb;
};


class TransportLayer
{
public:
    static TransportLayer& instance(){
        static TransportLayer self;
        return self;
    }

    bool startNThreads(int N=1);
    bool stopAllThreads();
    transport_ptr createTransport(const TransportInfo& params);

private:
    transport_ptr createTransport_n(const TransportInfo& params, void *thr);
};


}
#endif // TRANSPORT_H
