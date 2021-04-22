#ifndef SIGNALINGMSG_H
#define SIGNALINGMSG_H

#include <string>
#include <boost/json.hpp>
#include <string.h>
#include <iostream>
using namespace std;

namespace rtcgw {

/*  signaling defines
request: {"action":"sigin", "Cmd":{"user":"A", "pwd":"123456", "agent":"rtcgate"}}
response:{"code":"200", "reason":""}

request: {"action":"sigout", "Cmd":{"user":"A", "pwd":"123456", "angent":"rtcgate"}}
response:{"code":"200", "reason":""}

request: {"action":"offer","param":{"from":"A", "to":"B", "sdp":""}}
response:{"code":"200", "reason":""}

request: {"action":"answer","param":{"from":"A", "to":"B", "sdp":""}}
response:{"code":"200", "reason":""}

push: {"topic":"candidate","param":{"candidate":"" "mid":"0"}}

request: {"action":"bye","param":{"session":""}}
response:{"code":"200", "reason":""}

*/

#include <boost/json.hpp>
#include <tuple>

class CommandEncoder
{
public:
    virtual void encode()=0;

   const char* buf()const { return stream.c_str(); }
   int size()const{ return stream.size(); }

protected:
    std::string stream;
};

class Command;
using cmd_ptr = std::shared_ptr<Command>;;

class CommandDecoder
{
public:
    virtual void decode(void* params)=0;
};

struct NullDecoder : public CommandDecoder{
        NullDecoder(Command* cmd){
        }

        void decode(void* params)override{
        }
};


struct Command
{
    //push req cmd and return a rsp cmd
    using Dispatcher = std::function<cmd_ptr(Command* req, void* context)>;
    std::string id;//transaction id
    Dispatcher dispatcher;

    cmd_ptr dispatch(void* context=nullptr){
        if(dispatcher != nullptr){
            return dispatcher(this, context);
        }
        return nullptr;
    }

    virtual CommandEncoder* encoder()=0;
    virtual CommandDecoder* decoder()=0;
    virtual ~Command(){};


};

template<class Cmd>
struct CmdDispatcher
{
    Command::Dispatcher dispatcher;
};

//{"method":"sigin", "Cmd":{"user":"A", "pwd":"123456", "agent":"rtcgate"}}
struct SiginCmd:public Command
{
    SiginCmd()
        :_encoder(this)
        ,_decoder(this)
    {}

    SiginCmd(const std::string& u, const std::string& p, const std::string& a)
        :_encoder(this)
        ,_decoder(this)
        ,user(u)
        ,pwd(p)
        ,agent(a)
    {}

    ~SiginCmd(){
    }

    struct Encoder : public CommandEncoder{
        Encoder(SiginCmd* c);
        void encode()override;
        SiginCmd* cmd;
    };

    struct Decoder : public CommandDecoder{
        Decoder(SiginCmd* c)
            :cmd(c)
        {
        }

        void decode(void* params)override;
        SiginCmd* cmd;
    };

    CommandEncoder* encoder()override{
        _encoder.encode();
        return &_encoder;
    }

    CommandDecoder* decoder()override{
        return &_decoder;
    };

    Encoder _encoder;
    Decoder _decoder;

    std::string user;
    std::string pwd;
    std::string agent;
};

//{"method":"sigin", "Cmd":{"user":"A", "pwd":"123456", "agent":"rtcgate"}}
struct SiginOKCmd:public Command
{
    SiginOKCmd()
        :_encoder(this)
        ,_decoder(this)
    {}

    SiginOKCmd(const std::string& sid)
        :_encoder(this)
        ,_decoder(this)
        ,session_id(sid)
    {}

    struct Encoder : public CommandEncoder{
        Encoder(SiginOKCmd* c);
        void encode()override;
        SiginOKCmd* cmd;
    };

    struct Decoder : public CommandDecoder{
        Decoder(SiginOKCmd* c)
            :cmd(c)
        {
        }

        void decode(void* params)override;
        SiginOKCmd* cmd;
    };

    CommandEncoder* encoder()override{
        _encoder.encode();
        return &_encoder;
    }

    CommandDecoder* decoder()override{
        return &_decoder;
    };

    Encoder _encoder;
    Decoder _decoder;

    std::string session_id;
};

//{"method":"offer","param":{"from":"A", "to":"B", "sdp":""}}
struct OfferCmd :public Command
{
    OfferCmd()
        :_encoder(this)
        ,_decoder(this)
    {}

    struct Encoder : public CommandEncoder{
        Encoder(OfferCmd* c);
        void encode()override;
        OfferCmd* cmd;
    };

    struct Decoder : public CommandDecoder{
        Decoder(OfferCmd* c)
            :cmd(c)
        {
        }

        void decode(void* params)override;
        OfferCmd* cmd;
    };

    CommandEncoder* encoder()override{
        _encoder.encode();
        return &_encoder;
    }

    CommandDecoder* decoder()override{
        return &_decoder;
    };

    Encoder _encoder;
    Decoder _decoder;

    std::string dialog_id;
    std::string from;
    std::string to;
    std::string sdp;
};

struct AnswerCmd : public OfferCmd
{
};


//{"method":"candidateUpdate","param":{"candidate":"", "mid":"0"}}
struct CandidateCmd:public Command
{
    CandidateCmd()
        :_encoder(this)
        ,_decoder(this)
    {}

    struct Encoder : public CommandEncoder{
        Encoder(CandidateCmd* c);
        void encode()override;
        CandidateCmd* cmd;
    };

    struct Decoder : public CommandDecoder{
        Decoder(CandidateCmd* c)
            :cmd(c)
        {
        }

        void decode(void* params)override;
        CandidateCmd* cmd;
    };

    CommandEncoder* encoder()override{
        _encoder.encode();
        return &_encoder;
    }

    CommandDecoder* decoder()override{
        return &_decoder;
    };

    Encoder _encoder;
    Decoder _decoder;

    std::string from;
    std::string to;
    std::string mid;
    std::string candidate;
};

//{"method":"response","param":{"code":"200", "reason":""}
struct ResponseCmd:public Command
{
    ResponseCmd()
        :_encoder(this)
        ,_decoder(this)
    {}

    struct Encoder : public CommandEncoder{
        Encoder(ResponseCmd* c);
        void encode()override;
        ResponseCmd* cmd;
    };

    struct Decoder : public CommandDecoder{
        Decoder(ResponseCmd* c)
            :cmd(c)
        {
        }

        void decode(void* params)override;
        ResponseCmd* cmd;
    };

    CommandEncoder* encoder()override{
        _encoder.encode();
        return &_encoder;
    }

    CommandDecoder* decoder()override{
        return &_decoder;
    };

    Encoder _encoder;
    Decoder _decoder;

    std::string code;
    std::string reason;
};



template<class ...CmdDispatchers>
class CommandParserT
{
public:
    CommandParserT(){
        _dispatchers = std::make_tuple(CmdDispatchers()...);
    }

    template<class Dispatcher>
    void setDispatcher(Dispatcher d){
        auto& h = std::get<Dispatcher>(_dispatchers);
        h= d;
    }

    cmd_ptr parse(const char* stream, int size){
        cmd_ptr cmd;
        boost::json::error_code ec;
        json_parser.reset();
        json_parser.write(stream, size, ec);
        if(ec){
            std::cout<<"cmd decode failed:"<<ec.message()<<std::endl;
            return nullptr;
        }
        auto jv = json_parser.release();
        auto jo = jv.as_object();
        boost::json::string method = jo["method"].as_string();
        if(method == "sigin"){
            cmd.reset(new SiginCmd());
            auto h = std::get<CmdDispatcher<SiginCmd>>(_dispatchers);
            cmd->dispatcher = h.dispatcher;
        }
        else if(method == "siginok"){
            cmd.reset(new SiginOKCmd);
            auto h = std::get<CmdDispatcher<SiginOKCmd>>(_dispatchers);
            cmd->dispatcher = h.dispatcher;
        }
        else if(method == "offer"){
            cmd.reset(new OfferCmd);
            auto h = std::get<CmdDispatcher<OfferCmd>>(_dispatchers);
            cmd->dispatcher = h.dispatcher;
        }
        else if(method == "answer"){
            cmd.reset(new AnswerCmd);
            auto h = std::get<CmdDispatcher<AnswerCmd>>(_dispatchers);
            cmd->dispatcher = h.dispatcher;
        }
        else if(method == "candidate"){
            cmd.reset(new CandidateCmd);
            auto h = std::get<CmdDispatcher<CandidateCmd>>(_dispatchers);
            cmd->dispatcher = h.dispatcher;
        }
        else if(method == "response"){
            cmd.reset(new ResponseCmd);
            auto h = std::get<CmdDispatcher<ResponseCmd>>(_dispatchers);
            cmd->dispatcher = h.dispatcher;
        }
        else{
            std::cout<<"not support this cmd\n";
            return nullptr;
        }

        auto jp = jo["params"];
        if(!jp.is_object()){
            std::cout<<"parse error params not found\n";
            return nullptr;
        }
        cmd->decoder()->decode(&jo["params"]);

        if(cmd != nullptr){
            cmd->id = std::string(jo["transaction_id"].as_string().c_str());
        }
        return cmd;
    }

private:
    boost::json::parser json_parser;
    std::tuple<CmdDispatchers...> _dispatchers;
};

using CommandParser = CommandParserT<
            CmdDispatcher<SiginCmd>,
            CmdDispatcher<SiginOKCmd>,
            CmdDispatcher<OfferCmd>,
            CmdDispatcher<AnswerCmd>,
            CmdDispatcher<CandidateCmd>,
            CmdDispatcher<ResponseCmd>
       >;


}//rtcgw

#endif // SIGNALINGMSG_H
