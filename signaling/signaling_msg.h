#ifndef SIGNALINGMSG_H
#define SIGNALINGMSG_H

#include <string>
#include <third_party/jsoncpp/json.h>
#include <arseeulib/flexible_buffer.h>
#include <string.h>

namespace rtcgw {

/*  signaling defines
request: {"action":"sigin", "params":{"user":"A", "pwd":"123456", "agent":"rtcgate"}}
response:{"code":"200", "reason":""}

request: {"action":"sigout", "params":{"user":"A", "pwd":"123456", "angent":"rtcgate"}}
response:{"code":"200", "reason":""}

request: {"action":"offer","param":{"from":"A", "to":"B", "sdp":""}}
response:{"code":"200", "reason":""}

request: {"action":"answer","param":{"from":"A", "to":"B", "sdp":""}}
response:{"code":"200", "reason":""}

push: {"topic":"candidate","param":{"candidate":"" "mid":"0"}}

request: {"action":"bye","param":{"session":""}}
response:{"code":"200", "reason":""}

*/

bool GetValueFromJsonObject(const Json::Value& in,
                            const std::string& k,
                            Json::Value* out);

bool GetStringFromJson(const Json::Value& in, std::string* out) ;
bool GetStringFromJsonObject(const Json::Value& in,
                             const std::string& k,
                             std::string* out) ;


struct SignalingMsg
{
public:
    SignalingMsg(){}

    SignalingMsg(const std::string& action, const std::string& params);
    virtual ~SignalingMsg(){}
    std::string action()const{ return _action; }
    const std::string& params()const{return _params; }
    bool is_valid()const{ return _is_valid; }
    static SignalingMsg parse(const std::string& str);
    std::string serialize()const;

private:
    std::string _action;
    std::string _params;
    bool _is_valid=false;
};


struct SiginParams
{
    static bool parse(const std::string str, SiginParams& params);
    std::string serialize()const;

    std::string user;
    std::string pwd;
    std::string agent;
};

struct OfferParams
{
    static bool parse(const std::string str, OfferParams& params);
    std::string serialize()const;
    std::string dialog_id;
    std::string from;
    std::string to;
    std::string sdp;
};

struct AnswerParams : public OfferParams
{
};

struct CandidateParams
{
    static bool parse(const std::string str, CandidateParams& params);
    std::string serialize()const;

    std::string from;
    std::string to;
    std::string mid;
    std::string candidate;
};

SignalingMsg buildMsg(const OfferParams& params);
SignalingMsg buildMsg(const AnswerParams& params);
SignalingMsg buildMsg(const CandidateParams& params);


struct ResponseMsg
{
    std::string serialize()const;
    static bool parse(const std::string& str, ResponseMsg& msg);

    std::string code;
    std::string reason;
};

using SiginResponse = ResponseMsg;
using OfferResponse = ResponseMsg;
using AnswerResponse = ResponseMsg;
using CandidateResponse = ResponseMsg;

struct Streamer
{
    Streamer():_buf(2048){}
    void append(const uint8_t *data, int size);
    bool parse(SignalingMsg& smsg, ResponseMsg &rsmg);
    template<class Msg>
    static int serialize(const Msg& msg, uint8_t *buf, int buf_size){
        std::string str = msg.serialize();
        int str_len = (int)str.size();
        if(buf_size <= str_len + 4){
            return 0;
        }

        memcpy(buf, &str_len, 4);
        memcpy(buf+4, str.c_str(), str_len);
        return str_len+4;
    }

    arsee::FlexibleBuffer<uint8_t> _buf;
};



}//rtcgw

#endif // SIGNALINGMSG_H
