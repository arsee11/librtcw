///source.cpp

#include "source.h"
#include "observer.h"
#include <algorithm>
#include <assert.h>

namespace rtcgw
{

void SourceImpl::subscribe(Observer *o)
{
    assert( o!=nullptr );

    auto it = std::find(_observers.begin(), _observers.end(), o);
    if(it == _observers.end()){
        _observers.push_back(o);
        o->attached_source_name(this->source_name());
    }
}

void SourceImpl::pushFrame(const MediaFrame &frame)
{
    for(auto o : _observers){
        o->onSubscribeFrame(frame);
    }
}

void SourceImpl::pushPacket(const RtpPacket &packet)
{
    for(auto o : _observers){
        o->onSubscribePacket(packet);
    }
}

}
