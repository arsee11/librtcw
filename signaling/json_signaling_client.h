#ifndef HTTPSIGNALINGCLIENT_H
#define HTTPSIGNALINGCLIENT_H


#include "signaling/signaling_client.h"
#include <cstdint>
#include <string>
#include <thread>
#include "session/stream.h"
#include "signaling_msg.h"
#include <threads.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <easynet/connector.h>
#include <easynet/msg.h>

namespace rtcgw{

using Connector = arsee::net::Connector4;
using ConnectionPtr = Connector::netpeer_ptr;
using NetEventQueue = arsee::net::EventQueueEpoll;

class JsonSignalingClient : public SignalingClient
{
public:
    JsonSignalingClient(Signaling* s, const std::string& server_ip, uint16_t server_port,
                        NetEventQueue* evt_queue, ThreadScopePolling *thr);
    ~JsonSignalingClient();

    bool open();
    void close();

    // SignalingClient interface
public:
    void signIn(const std::string& user)override;
    void signOut()override;
    std::string createDialog()override;
    void joinDialog(const string &dialog_id)override;
    void inviteToDialog(const string &dialog_id)override;
    void onInvitedToDailog(const OfferCmd &cmd)override;
    void leaveDialog()override;

private:
    std::string _server_ip;
    uint16_t _server_port=80;
    NetEventQueue* _evt_queue;
    Connector _connector;
    ConnectionPtr _conn;
    CommandParser _parser;
    int _peer_id=0;
    ThreadScopePolling* _thread_scope;
    std::string _myname;
    std::string _peer_name;

    void net_loop();
protected:
    void onCreateSessionResponse(int error, const StreamInfos &sinfos, const TransportInfos &tinfos);
    void onStreamEndpointResponse(const std::string &stream_id, const EndpointInfo &ep);
    void sendMessage(const void *buf, int size);
    void onMessageFromSvr(const uint8_t *msg, int size);
    cmd_ptr onRecvAnswer(Command *req);
    cmd_ptr onRecvCandidate(Command *req);
    cmd_ptr onSignedIn(Command* req);


};

}//rtcgw

#endif // HTTPSIGNALINGCLIENT_H
