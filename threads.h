#ifndef THREADS_H
#define THREADS_H

#include <arseeulib/threading/exescope.h>
#include <easynet/event_queue_epoll.h>

namespace rtcgw {

using ThreadScope = arsee::ExeScope;
using ThreadScopePolling = arsee::ExeScope_p<arsee::net::EventQueueEpoll>;


}//rtcgw

#endif // THREADS_H
