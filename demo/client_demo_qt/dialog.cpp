#include "dialog.h"

namespace rtcgw{


Dialog::Dialog(const string &id, Signaling *s)
    :_id(id)
    ,_signaling(s)
{

}

void Dialog::setSession(peer_session_ptr s)
{
    _session = s;
}

bool Dialog::setRemoteStreamEndpoint(const std::string &session_id, const std::string &mid
                                    , const EndpointInfo &cinfo)
{
    if(_session == nullptr){
        //Log()<< session["<<session_id<<"] not found";
        return false;
    }
    _session->addRemoteEndpoint(mid, cinfo);


    return true;
}


std::vector<Stream *> Dialog::addStreams(const std::string &session_id, const StreamInfos &sinfos, const TransportInfos &tinfos)
{
    std::vector<Stream*> streams;
    if(_session != nullptr){
        streams = _session->addStreams(sinfos, tinfos, [this](const std::string& stream_id, const EndpointInfo& ep){
            if(_signaling != nullptr){
                _signaling->responseStreamEndpointInfo(stream_id, ep);
            }
        });
    }

    return streams;
}

void Dialog::setRemoteStreamInfo(const std::string& session_id, const StreamInfos &sinfos)
{
    if(_session != nullptr){
        _session->setRemoteStreamInfo(sinfos);
        _session->doNegotiation([this](const StreamInfos& stream_info, const TransportInfos& tinfos){
            if(_signaling != nullptr){
                _signaling->responseCreateSession(0, stream_info, tinfos);
            }
        });
    }

    //Log()<< session["<<session_id<<"] not found";
}
}///rtcgw

