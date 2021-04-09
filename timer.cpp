#include "timer.h"
#include "time_clocks.h"

#include <chrono>

namespace rtcgw{

TimerMS::TimerMS(ThreadScope *exs, TimeoutCb cb)
    :CycleTimer(cb)
    ,_thread_scope(exs)
{

}

void TimerMS::start(int ms)
{
    expiresAfter(ms);
    asyncWait(ms);
}


void TimerMS::expiresAfter(int ms)
{
    int64_t now_ms = Clock::timestamp_ms();
    _wait_untill = now_ms + ms;
}

void wait_ms(int ms){
    struct timeval tv;
    tv.tv_sec=ms/1000;
    tv.tv_usec=(ms%1000)*1000;
    int err;
    err=select(0,NULL,NULL,NULL,&tv);
    if(err<0 && errno==EINTR){

    }
}

void TimerMS::asyncWait(int ms)
{
    _thread_scope->post([this, ms]{
        wait_ms(1);
        uint64_t now_ms = Clock::timestamp_ms();
        if(now_ms >= _wait_untill){
            for(auto icb : _timeout_cbs){
                icb();
            }
            expiresAfter(ms);
        }
        asyncWait(ms);
    });
}

timer_ptr getDefaultTimer(ThreadScope *exs, TimeoutCb cb)
{
    return std::make_unique<TimerMS>(exs, cb);
}

TimerQueue::TimerQueue()
{
    _thread_scope.name("timer_queue");
}

TimerQueue::~TimerQueue()
{
    _thread_scope.stop();
}

void TimerQueue::startTimer(ThreadScope *callback_scope, int ms, TimeoutCb callback)
{
    auto callback_i = callback;
    if(callback_scope != nullptr){
        callback_i = [callback_scope, callback]{
            callback_scope->post(callback);
        };
    }

    const auto& it = _timers.find(ms);
    if( it == _timers.end() ){
        timer_ptr timer = getDefaultTimer(&_thread_scope, callback_i);
        timer->start(ms);
        _timers[ms] = std::move(timer);
    }
    else{
        it->second->addCb(callback_i);
    }
}

}//rtcgw

