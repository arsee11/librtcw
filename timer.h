#ifndef TIMER_H
#define TIMER_H

#include <functional>
#include <memory>
#include <map>
#include <list>
#include "threads.h"

namespace rtcgw{

using TimeoutCb = std::function<void()>;

class CycleTimer
{
public:
    CycleTimer(TimeoutCb cb)
    {
        _timeout_cbs.push_back(cb);
    }

    virtual ~CycleTimer(){}

    virtual void start(int ms)=0;

    void addCb(TimeoutCb cb){ _timeout_cbs.push_back(cb); }

protected:
    std::list<TimeoutCb> _timeout_cbs;
};


class TimerMS : public CycleTimer
{

public:
    TimerMS(ThreadScope* exs, TimeoutCb cb);
    void start(int ms)override;

private:
    void expiresAfter(int ms);
    void asyncWait(int ms);

    uint64_t _wait_untill=0;
    ThreadScope* _thread_scope=nullptr;
};


using timer_ptr=std::unique_ptr<CycleTimer>;
timer_ptr getDefaultTimer(ThreadScope* exs, TimeoutCb cb);

class TimerQueue
{
public:
    TimerQueue();
    ~TimerQueue();
    void startTimer(ThreadScope* callback_scope, int ms, TimeoutCb callback);

private:
    std::map<int, timer_ptr> _timers;
    ThreadScope _thread_scope;

};

}//rtcgw

#endif // TIMER_H
