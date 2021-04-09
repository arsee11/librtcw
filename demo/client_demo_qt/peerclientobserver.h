#ifndef PEERCLIENTOBSERVER_H
#define PEERCLIENTOBSERVER_H

//#include <examples/peerconnection/client/peer_connection_client.h>

#include <QObject>


class PeerInfoList;

class PeerClientObserver : public QObject
        /*,public PeerConnectionClientObserver*/

{
    Q_OBJECT
public:
    explicit PeerClientObserver(PeerInfoList* peers, QObject *parent = nullptr);

signals:

    void signedIn();
    void disconnected();
    void newPeerOnline();
    void onePeerOffline();
    void connectServerFailure();
    void messageFromPeer(int peer_id, const QString& msg);
    void messageSent(int err);

    // PeerConnectionClientObserver interface
public:
    void OnSignedIn();
    void OnDisconnected();
    void OnPeerConnected(int id, const std::string &name);
    void OnPeerDisconnected(int peer_id);
    void OnMessageFromPeer(int peer_id, const std::string &message);
    void OnMessageSent(int err);
    void OnServerConnectionFailure();

private:
    PeerInfoList* _peers_list=nullptr;
};

#endif // PEERCLIENTOBSERVER_H
