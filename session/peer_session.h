#ifndef PEER_SESSION_H
#define PEER_SESSION_H

#include <vector>
#include <memory>
#include <map>
#include "stream.h"
#include "timer.h"
#include "threads.h"

namespace rtcgw {

class PeerSessionCb
{
public:
    virtual void onOfferCreated()=0;
    virtual void onAnswerCreated()=0;
    virtual void onIceCondidate()=0;
};

class PeerSession;
using peer_session_ptr=std::shared_ptr<PeerSession>;

class PeerSession
{
public:
    using NegotiationCb = std::function<void(const StreamInfos& sinfos, const TransportInfos& tinfos)>;
    using EndpointCb = std::function<void (const std::string& stream_id, const EndpointInfo&)>;

public:
    PeerSession(const std::string& session_id, ThreadScope* thr, TimerQueue* tq);

    std::string id()const{ return _id; }

    void setRemoteStreamInfo(const StreamInfos& sinfos);
    void addRemoteEndpoint(const std::string &mid, const EndpointInfo& cinfo);

    void doNegotiation(const NegotiationCb& cb);

    void createOffer();
    std::string createAnswer();
    void gatheringCandidate();

    ///Take onwership of the Stream instance
    void addStream(Stream* s);

    std::vector<Stream *> addStreams(
            const StreamInfos& sinfos, const TransportInfos &tinfos, const EndpointCb &cb
    );

    Stream* getStreamFromIdx(int stream_idx);
    Stream* getStreamFromId(const std::string&  stream_id);


    void subscribe(peer_session_ptr listener);


private:
    std::string _id;
    using stream_ptr=std::unique_ptr<Stream>;
    std::vector<stream_ptr> _media_streams;
    std::vector<peer_session_ptr> _listeners;
    ThreadScope* _thread_scope=nullptr;
    TimerQueue* _timer_queue=nullptr;
};



/////////////////////////////////////////////////////////////////
class PeerSessionManager
{
public:
    PeerSession *createSession(const std::string& session_id);
    peer_session_ptr getSession(const std::string& session_id);

private:
    std::map<std::string, peer_session_ptr> _sessions;
};

}//rtcgt

#endif // PEER_SESSION_H
