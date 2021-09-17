#ifndef TRANSPORT_DEF_H
#define TRANSPORT_DEF_H

#include <string>
#include <vector>


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

enum TransportType
{
    UNKNOWN=-1,
    ICE,
    UDP,
    TCP
};

struct EndpointInfo
{
    TransportType type;
    std::string specify_info;
    std::string stream_id;
};

struct TransportInfo
{
    TransportPolicy policy=UNENCRYPTED;
    bool is_media=true;  //is media or data
    TransportType type=TransportType::ICE;    //use ice or not
    bool is_rtcp_mux=true;
    bool use_datagram_transport=false;
    bool use_datagram_transport_for_data_channels=false;
    bool enable_external_auth=false;
    bool active_reset_srtp_params=false;

    int ice_role; //enum{ICEROLE_CONTROLLING = 0, ICEROLE_CONTROLLED, ICEROLE_UNKNOWN};
    std::vector<CryptoInfo> cryptos; //SRTP config
    std::string mid;
    std::string local_ice_ufrag;
    std::string local_ice_pwd;
    std::string remote_ice_ufrag;
    std::string remote_ice_pwd;
    std::string ice_options;
    std::string remote_fingerprint_alg;//RFC4572 fingerprint
    std::string remote_fingerprint;    //RFC4572 fingerprint

};

using TransportInfos = std::vector<TransportInfo>;

}
#endif // TRANSPORT_DEF_H
