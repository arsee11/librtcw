#include "signaling_msg.h"

#include <iostream>

namespace rtcgw {

SiginCmd::Encoder::Encoder(SiginCmd *c)
    :cmd(c)
{

}

void SiginCmd::Encoder::encode()
{
    stream = "{\"method\":\"sigin\",\"transaction_id\":\""+cmd->id+"\",";
    stream+= "\"params\":{";
    stream+= "\"user\":\""+cmd->user+"\",";
    stream+= "\"pwd\":\""+cmd->pwd+"\",";
    stream += "\"agent\":\""+cmd->agent+"\"";
    stream+= "}}";
}

void SiginCmd::Decoder::decode(void *params){
    auto& jo = *static_cast<boost::json::object*>(params);
    try{
        cmd->user = jo["user"].as_string().c_str();
        cmd->pwd = jo["pwd"].as_string().c_str();
        cmd->agent = jo["agent"].as_string().c_str();
    }catch(std::invalid_argument& e){
    }
}

SiginOKCmd::Encoder::Encoder(SiginOKCmd *c)
    :cmd(c)
{
}

void SiginOKCmd::Encoder::encode()
{
    stream = "{\"method\":\"siginok\",\"transaction_id\":\""+cmd->id+"\",";
    stream+= "\"params\":{";
    stream+= "\"session_id\":\""+cmd->session_id+"\"";
    stream+= "}}";
}

void SiginOKCmd::Decoder::decode(void *params)
{
    auto& jo = *static_cast<boost::json::object*>(params);
    try{
        cmd->session_id = jo["session_id"].as_string().c_str();
    }catch(std::invalid_argument& e){
    }
}

OfferCmd::Encoder::Encoder(OfferCmd *c)
    :cmd(c)
{

}

void OfferCmd::Encoder::encode()
{
    stream = "{\"method\":\"offer\",\"transaction_id\":\""+cmd->id+"\",";
    stream+= "\"params\":{";
    stream+= "\"from\":\""+cmd->from+"\",";
    stream+= "\"to\":\""+cmd->to+"\",";
    stream += "\"sdp\":\""+cmd->sdp+"\"";
    stream+= "}}";
}

void OfferCmd::Decoder::decode(void *params){
    auto& jo = *static_cast<boost::json::object*>(params);
    try{
        cmd->from = jo["from"].as_string().c_str();
        cmd->to = jo["to"].as_string().c_str();
        cmd->sdp = jo["sdp"].as_string().c_str();
    }catch(std::invalid_argument& e){

    }
}

CandidateCmd::Encoder::Encoder(CandidateCmd *c)
    :cmd(c)
{

}

void CandidateCmd::Encoder::encode()
{
    stream = "{\"method\":\"candidateUpdate\",\"transaction_id\":\""+cmd->id+"\",";
    stream+= "\"parms\":{";
    stream+= "\"candidate\":\""+cmd->from+"\",";
    stream += "\"mid\":\""+cmd->mid+"\"";
    stream+= "}}";
}

void CandidateCmd::Decoder::decode(void *params){
    auto& jo = *static_cast<boost::json::object*>(params);
    try{
        cmd->candidate = jo["candidate"].as_string().c_str();
        cmd->mid = jo["mid"].as_string().c_str();
    }catch(std::invalid_argument& e){
    }
}

ResponseCmd::Encoder::Encoder(ResponseCmd *c)
    :cmd(c)
{

}

void ResponseCmd::Encoder::encode()
{
    stream = "{\"method\":\"candidateUpdate\",\"transaction_id\":\""+cmd->id+"\",";
    stream+= "\"parms\":{";
    stream+= "\"code\":\""+cmd->code+"\",";
    stream += "\"reason\":\""+cmd->reason+"\"";
    stream+= "}}";
}

void ResponseCmd::Decoder::decode(void *params){
    auto& jo = *static_cast<boost::json::object*>(params);
    try{
        cmd->code = jo["code"].as_string().c_str();
        cmd->reason = jo["reason"].as_string().c_str();
    }catch(std::invalid_argument& e){
    }
}








}//rtcgw
