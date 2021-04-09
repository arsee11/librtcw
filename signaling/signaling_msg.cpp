#include "signaling_msg.h"

#include <iostream>

namespace rtcgw {

bool GetValueFromJsonObject(const Json::Value& in,
                            const std::string& k,
                            Json::Value* out) {
  if (!in.isObject() || !in.isMember(k)) {
    return false;
  }

  *out = in[k];
  return true;
}

bool GetStringFromJson(const Json::Value& in, std::string* out) {
  if (!in.isString()) {
    if (in.isBool()) {
      *out = in.asBool()?"true":"false";
    } else if (in.isInt()) {
      *out = std::to_string(in.asInt());
    } else if (in.isUInt()) {
      *out = std::to_string(in.asUInt());
    } else if (in.isDouble()) {
      *out = std::to_string(in.asDouble());
    } else {
      return false;
    }
  } else {
    *out = in.asString();
  }
  return true;
}

bool GetStringFromJsonObject(const Json::Value& in,
                             const std::string& k,
                             std::string* out) {
  Json::Value x;
  return GetValueFromJsonObject(in, k, &x) && GetStringFromJson(x, out);
}


SignalingMsg::SignalingMsg(const std::string &action, const std::string &params)
    :_action(action)
    ,_params(params)
    ,_is_valid(true)
{

}

SignalingMsg SignalingMsg::parse(const std::string &str)
{
    Json::Reader reader;
    Json::Value jmessage;
    if (!reader.parse(str, jmessage)) {
      std::cout << "Received unknown message. " << str<<std::endl;
      return SignalingMsg();
    }

    std::string action;
    GetStringFromJsonObject(jmessage, "action", &action);
    Json::Value params_obj;
    GetValueFromJsonObject(jmessage, "params", &params_obj);
    Json::FastWriter writer;
    std::string params = writer.write(params_obj);
    return SignalingMsg(action, params);
}

std::string SignalingMsg::serialize() const
{
    std::string str = "{\"action\":\""+_action+"\",";
                str+= "\"params\":"+_params;
                str+= "}";

    return str;
}


bool SiginParams::parse(const std::string str, SiginParams &params)
{
    Json::Reader reader;
    Json::Value jmessage;
    if (!reader.parse(str, jmessage)) {
      std::cout << "Received unknown message. " << str<<std::endl;
      return false;
    }

    GetStringFromJsonObject(jmessage, "user", &params.user);
    GetStringFromJsonObject(jmessage, "pwd", &params.pwd);
    GetStringFromJsonObject(jmessage, "agent", &params.agent);
    return true;
}

std::string SiginParams::serialize() const
{
    std::string str = "{\"user\":\""+user+"\",";
                str+= "\"pwd\":\""+pwd+"\",";
                str+= "\"agent\":\""+agent+"\"";
                str+= "}";

    return str;
}

bool OfferParams::parse(const std::string str, OfferParams &params)
{
    Json::Reader reader;
    Json::Value jmessage;
    if (!reader.parse(str, jmessage)) {
      std::cout << "Received unknown message. " << str<<std::endl;
      return false;
    }

    GetStringFromJsonObject(jmessage, "dialog_id", &params.dialog_id);
    GetStringFromJsonObject(jmessage, "from", &params.from);
    GetStringFromJsonObject(jmessage, "to", &params.to);
    GetStringFromJsonObject(jmessage, "sdp", &params.sdp);
    return true;
}

std::string OfferParams::serialize() const
{
    std::string str = "{\"dialog_id\":\""+dialog_id+"\",";
                str+= "\"from\":\""+from+"\",";
                str+= "\"to\":\""+to+"\",";
                str+= "\"sdp\":\""+sdp+"\"";
                str+= "}";

    return str;
}

bool CandidateParams::parse(const std::string str, CandidateParams &params)
{
    Json::Reader reader;
    Json::Value jmessage;
    if (!reader.parse(str, jmessage)) {
      std::cout << "Received unknown message. " << str<<std::endl;
      return false;
    }

    GetStringFromJsonObject(jmessage, "from", &params.from);
    GetStringFromJsonObject(jmessage, "to", &params.to);
    GetStringFromJsonObject(jmessage, "mid", &params.mid);
    GetStringFromJsonObject(jmessage, "candidate", &params.candidate);
    return true;
}

std::string CandidateParams::serialize() const
{
    std::string str = "{\"from\":\""+from+"\",";
                str+= "\"to\":\""+to+"\",";
                str+= "\"mid\":\""+mid+"\",";
                str+= "\"candidate\":\""+candidate+"\"";
                str+= "}";

    return str;
}

std::string ResponseMsg::serialize()const
{
    std::string str = "{\"code\":\""+code+"\",";
                str+= "\"reason\":\""+reason+"\"";
                str+= "}";

                return str;
}

bool ResponseMsg::parse(const std::string &str, ResponseMsg &msg)
{
    Json::Reader reader;
    Json::Value jmessage;
    if (!reader.parse(str, jmessage)) {
      std::cout << "Received unknown message. " << str<<std::endl;
      return false;
    }

    GetStringFromJsonObject(jmessage, "code", &msg.code);
    GetStringFromJsonObject(jmessage, "reason", &msg.reason);

    return !msg.code.empty();
}

SignalingMsg buildMsg(const OfferParams &params)
{
    return SignalingMsg("offer", params.serialize());
}

SignalingMsg buildMsg(const AnswerParams &params)
{
    return SignalingMsg("answer", params.serialize());
}

SignalingMsg buildMsg(const CandidateParams &params)
{
    return SignalingMsg("candidate", params.serialize());
}

void Streamer::append(const uint8_t *data, int size)
{
    _buf.push(data, size);
}

bool Streamer::parse(SignalingMsg &smsg, ResponseMsg& rsmg)
{
    if(_buf.size() >= 4){
        int len = *(int*)_buf.begin();
        if( (int)_buf.size() >= len+4 ){
            std::string str((char*)(_buf.begin()+4), len);
            std::cout<<"msg body:"<<str<<std::endl;
            if(!ResponseMsg::parse(str, rsmg)){
                smsg = SignalingMsg::parse(str);
            }
            _buf.consume(len+4);
            return true;
        }
    }
    return false;
}


}//rtcgw
