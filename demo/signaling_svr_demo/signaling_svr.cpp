///signaling_svr.cpp



#include "easynet/acceptor.h"
#include "easynet/netlisteners.h"
#include "signaling/signaling_msg.h"

#include <iostream>
#include <stdio.h>
#include <algorithm>

using namespace arsee::net;
using namespace std;
using namespace std::placeholders;
using namespace rtcgw;

struct Session
{
    std::string id;
    std::string user;
    std::string pwd;
    netpeer_ptr transport;
};

///@key session id
using SessinMap = std::map<std::string, Session>;



class SiginHanler
{
public:
    SiginHanler(SiginParams& params, SessinMap& ssmap, const netpeer_ptr& transport)
    {
        static int id=0;
        Session ss;
        ss.id = std::to_string(id++);
        ss.user = params.user;
        ss.pwd = params.pwd;
        ss.transport = transport;
        ssmap.insert({ss.id, ss});
        cout<<"client:"<<ss.user<<" had sigin"<<endl;
    }
    SiginResponse process(){
        return {"200", "sucess"};
    }
};


class OfferHanler
{
public:
    OfferHanler(OfferParams& params, SessinMap& ssmap)
    {
        auto it = std::find_if(ssmap.begin(), ssmap.end(), [&params](auto ss){
            return ss.second.user == params.to;
        });
        if(it != ssmap.end()){
            uint8_t buf[20480];
            int size =20480;
            size=Streamer::serialize(buildMsg(params), buf, size);
            it->second.transport->write(MsgSt(buf, size));
        }
        else{
            cout<<"client:"<<params.to<<" not found"<<endl;
        }
    }
    OfferResponse process(){
        return {"200", "sucess"};
    }
};


class AnswerHanler
{
public:
    AnswerHanler(AnswerParams& params, SessinMap& ssmap)
    {
        auto it = std::find_if(ssmap.begin(), ssmap.end(), [&params](auto ss){
            return ss.second.user == params.to;
        });
        if(it != ssmap.end()){
            uint8_t buf[20480];
            int size =20480;
            size=Streamer::serialize(buildMsg(params), buf, size);
            it->second.transport->write(MsgSt(buf, size));
        }
        else{
            cout<<"client:"<<params.to<<" not found"<<endl;
        }
    }
    AnswerResponse process(){
        return {"200", "sucess"};
    }
};


class CandidateHanler
{
public:
    CandidateHanler(CandidateParams& params, SessinMap& ssmap)
    {
        auto it = std::find_if(ssmap.begin(), ssmap.end(), [&params](auto ss){
            return ss.second.user == params.to;
        });
        if(it != ssmap.end()){
            uint8_t buf[20480];
            int size =20480;
            size = Streamer::serialize(buildMsg(params), buf, size);
            it->second.transport->write(MsgSt(buf, size));
        }
        else{
            cout<<"client:"<<params.to<<" not found"<<endl;
        }
    }
    CandidateResponse process(){
        return {"200", "sucess"};
    }
};

class Server
{
public:
    Server(NetEventQueue* eq)
        :_alistener(eq)
        ,_ilistener(eq)
        ,_clistener(eq)
    {
        _alistener.listen([this](const netpeer_ptr& peer){ this->onAccept(peer); });
    }

    void onAccept(const netpeer_ptr& peer){
        cout<<"onAccept["<<peer->remote_addr().ip<<":"<<peer->remote_addr().port<<"]"<<endl;
        //listen<InputCmpEvent>( std::bind(&MySession::onInput, this, _1, _2) );
        _ilistener.listen(peer->fd(), [this](const netpeer_ptr& p, MsgSt m){this->onInput(p, m);});
        _clistener.listen(peer->fd(), [this](const netpeer_ptr& peer){ this->onClose(peer); });
        _peers.push_back(peer);
    }

    void onInput(const netpeer_ptr& peer, MsgSt msg){
        cout<<"onInput(size="<<msg.size()<<")["<<peer->remote_addr().ip<<":"<<peer->remote_addr().port<<"]"<<endl;
        ResponseMsg rmsg;
        SignalingMsg smsg;
        _streamer.append(msg.begin(), msg.size());
        if( !_streamer.parse(smsg, rmsg) ){
            return;
        }

        handleSignal(smsg, peer);

    }

    void onClose(const netpeer_ptr& peer){
        cout<<"onClose["<<peer->remote_addr().ip<<":"<<peer->remote_addr().port<<"]"<<endl;
        _ilistener.unlisten(peer->fd());
        _clistener.unlisten(peer->fd());
        for(auto it : _ssmap){
            if(it.second.transport == peer){
                cout<<"user "<<it.second.user<<" lost connect"<<endl;
                _ssmap.erase(it.second.id);
                break;
            }
        }
        _peers.remove(peer);
    }
private:
    void handleSignal(const rtcgw::SignalingMsg& smsg, const netpeer_ptr& peer){
        if(smsg.action() == "sigin"){
            SiginParams params;
            SiginParams::parse(smsg.params(), params);
            SiginHanler handler(params, _ssmap, peer);
            SiginResponse rsp = handler.process();
            uint8_t buf[20480];
            int size =20480;
            size=Streamer::serialize(rsp, buf, size);
            peer->write(MsgSt(buf, size));
        }
        else if(smsg.action() == "offer"){
            OfferParams params;
            OfferParams::parse(smsg.params(), params);
            OfferHanler handler(params, _ssmap);
            OfferResponse rsp = handler.process();
            uint8_t buf[20480];
            int size =20480;
            size=Streamer::serialize(rsp, buf, size);
            peer->write(MsgSt(buf, size));
        }
        else if(smsg.action() == "answer"){
            AnswerParams params;
            AnswerParams::parse(smsg.params(), params);
            AnswerHanler handler(params, _ssmap);
            AnswerResponse rsp = handler.process();
            uint8_t buf[20480];
            int size =20480;
            size=Streamer::serialize(rsp, buf, size);
            peer->write(MsgSt(buf, size));
        }
        else if(smsg.action() == "candidate"){
            CandidateParams params;
            CandidateParams::parse(smsg.params(), params);
            CandidateHanler handler(params, _ssmap);
            CandidateResponse rsp = handler.process();
            uint8_t buf[20480];
            int size =20480;
            size=Streamer::serialize(rsp, buf, size);
            peer->write(MsgSt(buf, size));
        }
    }

    std::list<netpeer_ptr> _peers;
    AcceptCmpListener _alistener;
    InputCmpListener _ilistener;
    CloseCmpListener _clistener;
    SessinMap _ssmap;
    Streamer _streamer;
};

int main(int argc, char** argv)
{
    NetEventQueue eq;
    AcceptorC a(&eq, 8888);

    eq.attach( a.fd() );

    Server s(&eq);

    while(true){
        eq.process();
    }

    getchar();

}
