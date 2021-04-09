#ifndef SIGNALING_H
#define SIGNALING_H

#include <string>
#include <functional>
#include "session/stream.h"

namespace rtcgw {

class StreamInfo;
class TransportInfo;

using CreateSessionResponse=std::function<void(int error, const StreamInfos& stream_infos, const TransportInfos& tinfos)>;
using StreamEndpointInfoResponse=std::function<void(const std::string& stream_id, const EndpointInfo& ep)>;


class Signaling
{
public:
    virtual ~Signaling(){}
    virtual bool onCreateSession(const std::string &dialog_id, const std::string &session_id)=0;
    virtual bool onSessionBind(const std::string& src_session, const std::string& listener_session)=0;
    virtual void onAddStream(const std::string& session_id, const StreamInfos &sinfos, const TransportInfos& tinfos)=0;
    virtual void onSetRemoteStream(const std::string& session_id, const StreamInfos &sinfos)=0;
    virtual void onDelStream()=0;
    virtual void onUpdateStream()=0;
    virtual bool onRemoteStreamEndpoint(const std::string &session_id, const std::string &mid, const EndpointInfo& cinfo)=0;

    void listenResponse(CreateSessionResponse l){_session_rsp=l;}
    void listenResponse(StreamEndpointInfoResponse l){_endpoit_rsp=l;}

    void responseCreateSession(int error, const StreamInfos& stream_infos, const TransportInfos &tinfos);
    void responseStreamEndpointInfo(const std::string& stream_id, const EndpointInfo& ep);

protected:

    void do_responseCreateSession(int error, const StreamInfos& stream_infos, const TransportInfos &tinfos);
    void do_responseStreamEndpointInfo(const std::string& stream_id, const EndpointInfo& ep);

protected:
    CreateSessionResponse _session_rsp;
    StreamEndpointInfoResponse _endpoit_rsp;
    ThreadScope* _thread_scope=nullptr;
};

}

#endif // SIGNALING_H
