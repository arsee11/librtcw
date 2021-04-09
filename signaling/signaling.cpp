///signaling.cpp

#include "signaling.h"

namespace rtcgw {

void Signaling::responseCreateSession(int error, const StreamInfos &stream_infos, const TransportInfos& tinfos)
{
    if(_thread_scope != nullptr){
        _thread_scope->post(&Signaling::do_responseCreateSession, this, error, stream_infos, tinfos);
    }
    else{
        do_responseCreateSession(error, stream_infos, tinfos);
    }
}

void Signaling::responseStreamEndpointInfo(const std::string &stream_id, const EndpointInfo &ep)
{
    if(_thread_scope != nullptr){
        _thread_scope->post(&Signaling::do_responseStreamEndpointInfo,
                this, stream_id, ep
        );
    }
    else{
        do_responseStreamEndpointInfo(stream_id, ep);
    }

}

void Signaling::do_responseCreateSession(int error, const StreamInfos &stream_infos, const TransportInfos &tinfos)
{
    if(_session_rsp != nullptr){
        _session_rsp(error, stream_infos, tinfos);
    }
}

void Signaling::do_responseStreamEndpointInfo(const string &stream_id, const EndpointInfo &ep)
{
    if(_endpoit_rsp != nullptr){
        _endpoit_rsp(stream_id, ep);
    }
}

}///rtcgw
