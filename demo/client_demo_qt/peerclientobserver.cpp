#include "peerclientobserver.h"
#include <QDebug>
#include "third_party/jsoncpp/json.h"
#include "peersinfo.h"

PeerClientObserver::PeerClientObserver(PeerInfoList *peers, QObject *parent) \
    : QObject(parent)
    ,_peers_list(peers)
{

}
//
// PeerConnectionClientObserver implementation.
//

void PeerClientObserver::OnSignedIn()
{
    qDebug() << __FUNCTION__;
    emit signedIn();
}

void PeerClientObserver::OnDisconnected()
{
    qDebug() << __FUNCTION__;
    emit disconnected();
}

void PeerClientObserver::OnPeerConnected(int id, const std::string& name)
{
    qDebug() << __FUNCTION__;

    _peers_list->add(id, name);

    emit newPeerOnline();
}

void PeerClientObserver::OnPeerDisconnected(int id)
{
    qDebug() << __FUNCTION__;
    _peers_list->del(id);
    emit onePeerOffline();
}

void PeerClientObserver::OnMessageFromPeer(int peer_id, const std::string& message)
{
    qDebug() << __FUNCTION__;
    emit messageFromPeer(peer_id, QString(message.c_str()));
}

void PeerClientObserver::OnMessageSent(int err)
{
  // Process the next pending message if any.
    emit messageSent(err);
}

void PeerClientObserver::OnServerConnectionFailure()
{
    qDebug() << __FUNCTION__;
    emit connectServerFailure();
}
