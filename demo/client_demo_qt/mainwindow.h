#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dialog.h"
#include "peersinfo.h"
#include "video_capture.h"
#include "videorender.h"

#include <QAbstractVideoSurface>
#include <QCamera>
#include <QMainWindow>
#include <QStandardItemModel>
#include <QStringListModel>
#include <memory>

#include <signaling/json_signaling_client.h>
#include <signaling/signaling.h>
#include <session/peer_session.h>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow, public rtcgw::Signaling
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void signalLocalVideoFrame();
    void signalRemoteVideoFrame();

private slots:
    void on_signedIn();
    void on_disconnected();
    void on_newPeerOnline();
    void on_onePeerOffline();
    void on_connectServerFailure();
    void on_messageFromPeer(int peer_id, const QString& msg);
    void on_messageSent(int err);
    void on_listView_doubleClicked(const QModelIndex &index);
    void onLocalVideoFrame();
    void onRemoteVideoFrame();

    void on_btnConnectToSvr_clicked();

private:
    bool onCreateSession(const string &dialog_id, const std::string &session_id)override;
    bool onRemoteStreamEndpoint(const std::string &session_id, const std::string &mid, const rtcgw::EndpointInfo& cinfo)override;
    void onAddStream(const std::string& session_id, const rtcgw::StreamInfos &sinfos, const rtcgw::TransportInfos& tinfos)override;
    bool onSessionBind(const std::string&, const std::string&)override;
    void onSetRemoteStream(const std::string& session_id, const rtcgw::StreamInfos &sinfos)override;
    void onDelStream()override;
    void onUpdateStream()override;
    void onLocalStreamEndpoint(const std::string &mid, const rtcgw::EndpointInfo &cinfo);


private:
    Ui::MainWindow *ui;
    std::unique_ptr<rtcgw::JsonSignalingClient> _signal_client;
    rtcgw::NetEventQueue _evt_queue;
    std::unique_ptr<rtcgw::ThreadScopePolling> _signal_thread;
    std::unique_ptr<rtcgw::Dialog> _dialog;
    rtcgw::TransportLayer _transport_layer;

    QStandardItemModel* list_model=nullptr;
    PeerInfoList _peer_list;

    VideoCapture* _video_dev;
    std::string _video_dev_name;
    VideoRender* _remote_render=nullptr;
    VideoRender* _local_render=nullptr;
    uint8_t _remote_video_data[1280*720*3];
    int _remote_video_size=0;
    uint8_t _local_video_data[1280*720*3];
    int _local_video_size=0;

};
#endif // MAINWINDOW_H
