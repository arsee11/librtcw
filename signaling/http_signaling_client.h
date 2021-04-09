#ifndef HTTPSIGNALINGCLIENT_H
#define HTTPSIGNALINGCLIENT_H


#include "signaling/signaling_client.h"
#include <cstdint>
#include <string>
#include "session/stream.h"

namespace rtcgw{

class HttpSignalingClient : public SignalingClient
{
public:
    HttpSignalingClient(Signaling* s, const std::string& server_ip, uint16_t server_port);
    ~HttpSignalingClient();

    bool open();
    void close();

    // SignalingClient interface
public:
    void run();

private:
    void signIn()override;
    void signOut()override;
    std::string createDialog()override;
    void joinDialog(const string &dialog_id)override;
    void leaveDialog()override;

private:
    std::string _server_ip;
    uint16_t _server_port=80;
    int _peer_id=0;

    // PeerConnectionClientObserver interface
public:
    void OnSignedIn();
    void OnDisconnected();
    void OnPeerConnected(int id, const std::string &name);
    void OnPeerDisconnected(int peer_id);
    void OnMessageFromPeer(int peer_id, const std::string &message);
    void OnMessageSent(int err);
    void OnServerConnectionFailure();

protected:
    void onCreateSessionResponse(int error, const StreamInfos &sinfos, const TransportInfos &tinfos);
    void onStreamEndpointResponse(const std::string &stream_id, const EndpointInfo &ep);

    void SendMessage(const std::string &json_object);
    std::deque<std::string> _pending_messages;

};

}//rtcgw

#endif // HTTPSIGNALINGCLIENT_H
