#ifndef STREAMFACTORY_H
#define STREAMFACTORY_H

#include "stream.h"

namespace rtcgw {

class StreamFactory
{
public:
    static Stream* createStream(int stream_type, const std::string &stream_id, ThreadScope *thr);
    StreamFactory();

};

}//rtcgw

#endif // STREAMFACTORY_H
