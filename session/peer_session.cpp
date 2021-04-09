///peer_session.cpp
///


#include "peer_session.h"
#include "transport/transport.h"
#include <algorithm>
#include <iostream>

#include "stream_factory.h"
#include "stream.h"

namespace rtcgw {

PeerSession::PeerSession(const std::string &session_id, ThreadScope *thr, TimerQueue *tq)
    :_id(session_id)
    ,_thread_scope(thr)
    ,_timer_queue(tq)
{

}

void PeerSession::setRemoteStreamInfo(const StreamInfos &sinfos)
{
    for(auto sinfo : sinfos){
        Stream* stream = getStreamFromId(sinfo.id);
        if(stream != nullptr){
            stream->setRemoteParams(sinfo);
        }
        else{
            //LogE()<<"stream [] is not created"<<endl;
        }
    }

}

void PeerSession::addRemoteEndpoint(const std::string &mid, const EndpointInfo &cinfo)
{
    Stream* s = getStreamFromId(mid);
    if(s != nullptr){
        if(s->transport() != nullptr){
            s->transport()->addRemoteEndpoint(cinfo);
        }
        else{
            //LogE()<<"Transport for stream ["<<mid<<" ] not existed"<<endl
        }

    }
    else{
        //LogE()<<"stream ["<<mid<<" ] not existed"<<endl;
    }
}

void PeerSession::doNegotiation(const PeerSession::NegotiationCb &cb)
{
    StreamInfos sinfos;
    TransportInfos tinfos;
    for(auto& stream : _media_streams){
        StreamInfo sinfo;
        TransportInfo tinfo;
        std::tie(sinfo, tinfo) = stream->doNegotiation();
        sinfos.push_back(sinfo);
        tinfos.push_back(tinfo);
    }

    if(cb != nullptr){
        cb(sinfos, tinfos);
    }

    for(auto& stream : _media_streams){
        stream->transport()->open();
    }
}

void PeerSession::createOffer()
{
    //create Streams if not exist
    //set recive params
}

std::string PeerSession::createAnswer()
{
    //create Streams if not exist
    //set recive params

    return "";
}

void PeerSession::gatheringCandidate()
{

}

void PeerSession::addStream(Stream *s)
{
    _media_streams.push_back(stream_ptr(s));

    //if has listeners, so all the listners try to subcribe the new stream.
    for(auto l : _listeners){
        int idx = static_cast<int>(_media_streams.size()-1);
        _media_streams[idx]->subscribe( l->getStreamFromIdx(idx) );
    }
}

std::vector<Stream *> PeerSession::addStreams(
        const StreamInfos &sinfos, const TransportInfos &tinfos, const EndpointCb& cb
)
{
    assert(sinfos.size() == tinfos.size());

    std::vector<Stream *> streams;
    for(size_t i=0; i<sinfos.size(); i++){
        StreamInfo sinfo=sinfos[i];
        TransportInfo tinfo=tinfos[i];
        Stream* stream = StreamFactory::createStream(sinfo.stream_type, sinfo.id, _thread_scope);
        if(stream != nullptr){
            transport_ptr trans = TransportLayer::instance().createTransport(tinfo);
            trans->listenOnEndpointInfo([cb](Transport* t, const EndpointInfo& ep){
                cb(t->stream_id(), ep);
            });
            _timer_queue->startTimer(_thread_scope, 10, std::bind(&Stream::onTimerEvent, stream));
            stream->transport(trans);
            stream->setLocalParams(sinfo);
            addStream(stream);
            streams.push_back(stream);
        }
        else{
            //LogE()<<"stream [] create failed"<<endl;
        }
    }

    return streams;
}

Stream *PeerSession::getStreamFromIdx(int stream_idx)
{
    return _media_streams[stream_idx].get();
}

Stream *PeerSession::getStreamFromId(const std::string&  stream_id)
{
    for(const auto& i : _media_streams){
        if(i->id() == stream_id){
            return i.get();
        }
    }

    return nullptr;
}

void PeerSession::subscribe(peer_session_ptr listener)
{
    for(size_t i=0; i<_media_streams.size(); i++){
        _media_streams[i]->subscribe( listener->getStreamFromIdx(i) );
    }

    _listeners.push_back(listener);
}

static TimerQueue timer_queue;
static ThreadScope session_thread;
PeerSession* PeerSessionManager::createSession(const std::string &session_id)
{
    auto s = std::make_shared<PeerSession>(session_id, &session_thread, &timer_queue);
    _sessions[session_id] = s;
    return s.get();
}

peer_session_ptr PeerSessionManager::getSession(const std::string &session_id)
{
    auto it=_sessions.find(session_id);
    if(it != _sessions.end()){
        return it->second;
    }

    return nullptr;
}

}//rtcgt
