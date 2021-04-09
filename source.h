#ifndef SOURCE_H
#define SOURCE_H

#include <list>
#include <string>
#include "rtputils/rtp_defines.h"
#include "media_frame.h"

namespace rtcgw {

class Observer;

class Source
{
public:
    ///The same Observer instance only subscribe one time
    virtual void subscribe(Observer* o)=0;
    virtual void pushFrame(const MediaFrame& frame)=0;
    virtual void pushPacket(const RtpPacket& packet)=0;
    virtual std::string source_name()const =0;
};


class SourceImpl:public Source
{
    // Source interface
public:
    void subscribe(Observer *o)override;
    void pushFrame(const MediaFrame& frame)override;
    void pushPacket(const RtpPacket& packet)override;
    std::string source_name()const override{ return _name; }

private:
    std::list<Observer*> _observers;
    std::string _name;

};

}

#endif // SOURCE_H
