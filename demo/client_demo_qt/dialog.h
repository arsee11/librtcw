#ifndef DIALOG_H
#define DIALOG_H

#include <session/stream.h>
#include <session/peer_session.h>
#include <signaling/signaling.h>

namespace rtcgw{

class Dialog
{
public:
    Dialog(const std::string& id, Signaling* s);
    void setSession(peer_session_ptr s);
    std::vector<Stream*> addStreams(const std::string &session_id, const StreamInfos &sinfos, const TransportInfos &tinfos);
    bool setRemoteStreamEndpoint(const std::string &session_id, const std::string &mid, const EndpointInfo &cinfo);
    void setRemoteStreamInfo(const std::string &session_id, const StreamInfos &sinfos);

private:
    std::string _id;
    peer_session_ptr _session;
    Signaling* _signaling=nullptr;

};

}///rtcgw

#endif // DIALOG_H
