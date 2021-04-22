///signaling_svr.cpp



#include "easynet/acceptor.h"
#include "signaling/signaling_msg.h"

#include <iostream>
#include <stdio.h>
#include <algorithm>

using namespace arsee::net;
using namespace std;
using namespace std::placeholders;
using namespace rtcgw;

using  Acceptor = AcceptorC4;
using netpeer_ptr = Acceptor::netpeer_ptr;

struct Session
{
    std::string id;
    std::string user;
    std::string pwd;
    netpeer_ptr transport;
};

///@key session id
using SessionMap = std::map<std::string, Session>;

class Server
{
public:
    Server(EventQueueEpoll* eq, uint16_t port)
        :_acceptor(eq, port)
    {
        _acceptor.listenOnAccept(std::bind(&Server::onAccept, this, _1));

        _parser.setDispatcher(
            CmdDispatcher<AnswerCmd>{std::bind(&Server::onRecvAnswer, this, _1)}
        );
        _parser.setDispatcher(
            CmdDispatcher<CandidateCmd>{std::bind(&Server::onRecvCandidate, this, _1)}
        );
        _parser.setDispatcher(
            CmdDispatcher<SiginCmd>{std::bind(&Server::onRecvSigin, this, _1, _2)}
        );
        _parser.setDispatcher(
            CmdDispatcher<OfferCmd>{std::bind(&Server::onRecvOffer, this, _1)}
        );
    }

private:
    void onAccept(netpeer_ptr peer){
        cout<<"onAccept["<<peer->remote_addr().ip<<":"<<peer->remote_addr().port<<"]"<<endl;
        peer->listenOnRecv(std::bind(&Server::onInput, this, _1, _2));
        peer->listenOnClose(std::bind(&Server::onClose, this, _1));
        _peers.push_back(peer);
    }

    struct CmdContext{
        netpeer_ptr peer;
    };

    void onInput(const netpeer_ptr& peer, MsgSt msg){
        cout<<"onInput("<<msg.size()<<")from["
            <<peer->remote_addr().ip<<":"<<peer->remote_addr().port<<"]:\n"
            <<(char*)msg.rd_ptr()<<endl;
        cmd_ptr req = _parser.parse((char*)msg.rd_ptr(), msg.size());
        if(req == nullptr){
            return;
        }
        CmdContext ctxt{peer};
        cmd_ptr rsp = req->dispatch(&ctxt);
        if(rsp != nullptr){
            auto cmde = rsp->encoder();
            if(cmde->size() > 0){
                cout<<"response:"<<(char*)cmde->buf()<<endl;
            	peer->write(MsgSt(cmde->buf(), cmde->size()));
            }
        }
    }

    void onClose(const netpeer_ptr& peer){
        cout<<"onClose["<<peer->remote_addr().ip<<":"<<peer->remote_addr().port<<"]"<<endl;
        for(auto it : _ssmap){
            if(it.second.transport == peer){
                cout<<"user "<<it.second.user<<" lost connect"<<endl;
                _ssmap.erase(it.second.id);
                break;
            }
        }
        _peers.remove(peer);
    }
    cmd_ptr onRecvAnswer(Command* req)
    {
        AnswerCmd* cmd = static_cast<AnswerCmd*>(req);
        auto it = std::find_if(_ssmap.begin(), _ssmap.end(), [cmd](auto ss){
            return ss.second.user == cmd->to;
        });
        if(it != _ssmap.end()){
            auto cmde = cmd->encoder();
            it->second.transport->write(MsgSt(cmde->buf(), cmde->size()));
        }
        else{
            cout<<"client:"<<cmd->to<<" not found"<<endl;
        }
        auto rsp = new ResponseCmd;
        rsp->code="200";
        rsp->id = cmd->id;
        return cmd_ptr(rsp);
    }

    cmd_ptr onRecvCandidate(Command *req)
    {
        CandidateCmd* cmd = static_cast<CandidateCmd*>(req);

        auto it = std::find_if(_ssmap.begin(), _ssmap.end(), [cmd](auto ss){
            return ss.second.user == cmd->to;
        });
        if(it != _ssmap.end()){
            auto cmde = cmd->encoder();
            it->second.transport->write(MsgSt(cmde->buf(), cmde->size()));
        }
        else{
            cout<<"client:"<<cmd->to<<" not found"<<endl;
        }
        auto rsp = new ResponseCmd;
        rsp->code="200";
        rsp->id = cmd->id;
        return cmd_ptr(rsp);
    }

    cmd_ptr onRecvOffer(Command *req)
    {
        OfferCmd* cmd = static_cast<OfferCmd*>(req);

        auto it = std::find_if(_ssmap.begin(), _ssmap.end(), [&cmd](auto ss){
            return ss.second.user == cmd->to;
        });
        if(it != _ssmap.end()){
            auto cmde = cmd->encoder();
            it->second.transport->write(MsgSt(cmde->buf(), cmde->size()));
        }
        else{
            cout<<"client:"<<cmd->to<<" not found"<<endl;
        }
        auto rsp = new ResponseCmd;
        rsp->code="200";
        rsp->id = cmd->id;
        return cmd_ptr(rsp);
    }

    cmd_ptr onRecvSigin(Command *req, void* context)
    {
        SiginCmd* cmd = static_cast<SiginCmd*>(req);
        auto ctxt = static_cast<CmdContext*>(context);
        static int id=0;
        Session ss;
        ss.id = std::to_string(id++);
        ss.user = cmd->user;
        ss.pwd = cmd->pwd;
        ss.transport = ctxt->peer;
        _ssmap[ss.id] =  ss;
        cout<<"client:"<<ss.user<<" had sigin"<<endl;
		
        auto rsp = new SiginOKCmd;
        rsp->session_id = ss.id;
        rsp->id = cmd->id;
        return cmd_ptr(rsp);
    }

    Acceptor _acceptor;
    std::list<netpeer_ptr> _peers;
    SessionMap _ssmap;
    CommandParser _parser;
};

int main(int argc, char** argv)
{
    EventQueueEpoll eq;
    Server s(&eq, 8888);

    while(true){
        eq.process();
    }

    getchar();

}
