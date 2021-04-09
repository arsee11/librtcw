///stream_events.cpp
///


#include "stream_events.h"

namespace rtcgw
{

void KeyFrameRequestEvent::fire(StreamEventHandler *hdlr) const
{
    if(hdlr != nullptr){
        hdlr->onKeyFrameRequest(this);
    }
}

void PacketRTXRequestEvent::fire(StreamEventHandler *hdlr) const
{

}


///////////////////////////////////////////////////////////////////////////////////////////
void StreamEventSource::listenEvents(StreamEventHandler *hdlr){
    auto it = std::find(_evt_handlers.begin(), _evt_handlers.end(), hdlr);
    if(it != _evt_handlers.end()){
        _evt_handlers.push_back(hdlr);
    }
}

void StreamEventSource::unlistenEvents(StreamEventHandler *hdlr){
    auto it = std::find(_evt_handlers.begin(), _evt_handlers.end(), hdlr);
    if(it != _evt_handlers.end()){
        _evt_handlers.erase(it);
    }
}

}
