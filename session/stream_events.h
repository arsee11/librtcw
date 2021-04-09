#ifndef STREAM_EVENTS_H
#define STREAM_EVENTS_H

#include <vector>
#include <type_traits>
#include <tuple>
#include <algorithm>

namespace rtcgw {


class StreamEventHandler;

class StreamEvent
{
public:
    virtual void fire(StreamEventHandler* hdlr)const=0;
};


class KeyFrameRequestEvent : public StreamEvent
{
public:
    KeyFrameRequestEvent()
    {}

    void fire(StreamEventHandler* hdlr)const override;

};


class PacketRTXRequestEvent : public StreamEvent
{
public:
    PacketRTXRequestEvent(uint32_t ssrc, const std::vector<uint16_t>& packet_numbers)
        :_ssrc(ssrc)
        ,_packet_numbers(packet_numbers)
    {}

    void fire(StreamEventHandler* hdlr)const override;

    uint32_t ssrc()const{ return _ssrc; }
    std::vector<uint16_t> packet_numbers()const{ return _packet_numbers; }

private:
    uint32_t _ssrc;
    std::vector<uint16_t> _packet_numbers;
};



///////////////////////////////////////////////////////////////////////////////////////////////////////
class StreamEventHandler
{
public:
    virtual bool isListening(const StreamEvent* evt)=0;
    virtual void onKeyFrameRequest(const KeyFrameRequestEvent* event)=0;
    virtual void onPacketRTXRequest(const PacketRTXRequestEvent* event)=0;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////
class StreamEventSource
{
public:
    void listenEvents(StreamEventHandler* hdlr);
    void unlistenEvents(StreamEventHandler* hdlr);

    template<class Event>
    void sendEvent(const Event& evt){
        for(auto hdlr : _evt_handlers){
            if( hdlr->isListening( &evt ) ){
                evt.fire(hdlr);
            }
        }
    }

private:
    std::vector<StreamEventHandler*> _evt_handlers;

};





}//rtcgw
#endif // STREAM_EVENTS_H
