#ifndef SIGNALING_HANDLER_H
#define SIGNALING_HANDLER_H

#include "signaling_msg.h"

namespace rtcgw {

class Signaling;

class SignalingClient
{
public:
    SignalingClient(Signaling* s):_signaling(s){}
    virtual ~SignalingClient(){}

    virtual void signIn(const std::string& user)=0;
    virtual void signOut()=0;
    virtual std::string createDialog()=0;
    virtual void joinDialog(const std::string &dialog_id)=0;
    virtual void inviteToDialog(const std::string &dialog_id)=0;
    virtual void onInvitedToDailog(const OfferParams& parmas)=0;
    virtual void leaveDialog()=0;

protected:
    Signaling* _signaling=nullptr;
};

}//rtcgw

#endif // SIGNALING_HANDLER_H
