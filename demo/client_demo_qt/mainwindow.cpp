#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "third_party/libyuv/include/libyuv/convert_from.h"
#include "api/video/i420_buffer.h"
#include <QImage>
#include <QPixmap>

#include <signaling/json_signaling_client.h>
#include <session/stream_factory.h>
#include <QCameraInfo>
#include <QCameraViewfinder>
#include <QVideoSurfaceFormat>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    list_model = new QStandardItemModel;
    ui->listView->setModel(list_model);
    connect(this, &MainWindow::signalLocalVideoFrame, this, &MainWindow::onLocalVideoFrame);
    connect(this, &MainWindow::signalRemoteVideoFrame, this, &MainWindow::onRemoteVideoFrame);

    _signal_thread.reset(new rtcgw::ThreadScopePolling(&_evt_queue));
    rtcgw::TransportLayer::instance().startNThreads();
    _remote_render = new VideoRender();
    _remote_render->rgba_frame_cb = [this](uint8_t* data, int size){
        memcpy(_remote_video_data, data, size);
        _remote_video_size = size;
        emit signalRemoteVideoFrame();
    };

    _local_render = new VideoRender();
    _local_render->rgba_frame_cb = [this](uint8_t* data, int size){
        memcpy(_local_video_data, data, size);
        _local_video_size = size;
        //qDebug()<<"local frame came";
        emit signalLocalVideoFrame();
    };
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_signedIn()
{

}

void MainWindow::on_disconnected()
{

}

void MainWindow::on_newPeerOnline()
{
    list_model->clear();
    for(int i=0; i<_peer_list.size(); i++){
        list_model->setItem(i, 0, new QStandardItem(QString(_peer_list[i].name.c_str())));
    }
}

void MainWindow::on_onePeerOffline()
{

}

void MainWindow::on_connectServerFailure()
{

}

void MainWindow::on_messageFromPeer(int peer_id, const QString &msg)
{
    qDebug()<<__FUNCTION__<<":"<<msg;
}

void MainWindow::on_messageSent(int err)
{
    qDebug()<<__FUNCTION__<<":"<<err;
}

void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{
    qDebug()<<__FUNCTION__<<"index:"<<index;
    qDebug()<<":"<<index.data().toString();
    PeersInfo peer = _peer_list[index.row()];
    qDebug()<<"peer:{"<<peer.id<<","<<peer.name.c_str()<<"}";

//    rtcgw::Dialog* dlg = _signal_client->createDialog();

//    rtcgw::TransportInfo tinfo = rtcgw::TransportLayer::getDefaultParams(/*trans_type*/);
//    rtcgw::transport_ptr trans = rtcgw::TransportLayer::instance().createTransport(tinfo);

//    std::string stream_id="video0";
//    rtcgw::Stream* stream = rtcgw::StreamFactory::createStream(rtcgw::STREAM_VIDEO, stream_id, nullptr);
//    rtcgw::StreamInfo sinfo = rtcgw::Stream::getDefaultParams(rtcgw::STREAM_VIDEO, stream_id);
//    stream->setLocalParams(sinfo);
//    stream->transport(trans);

//    std::string session_id = "session0";
//    rtcgw::PeerSession* sess = _session_mgr.createSession(session_id);
//    sess->addStream(stream);
//    dlg->setSession(sess);
//    dlg->inviteTo();

//    trans->listenOnEndpointInfo([stream_id, dlg](rtcgw::Transport*, const rtcgw::EndpointInfo& ep){
//        dlg->addLocalStreamEndpoint(stream_id, ep);
//    });
//    trans->open();
}

void MainWindow::onLocalVideoFrame()
{
    if(_local_render !=nullptr && _local_video_size > 0){
        QImage image2 = QImage(_local_video_data, _local_render->width(), _local_render->height(), QImage::Format_RGB888);
        ui->label_local->setPixmap(QPixmap::fromImage(image2));
        _local_video_size=0;
    }

}

void MainWindow::onRemoteVideoFrame()
{
    //qDebug()<<__FUNCTION__;
    if(_remote_render != nullptr && _remote_video_size >0){
        QImage image1 = QImage(_remote_video_data, _remote_render->width(), _remote_render->height(), QImage::Format_RGB888);
        ui->label_remote->setPixmap(QPixmap::fromImage(image1));
        _remote_video_size=0;
    }
}

void MainWindow::on_btnConnectToSvr_clicked()
{
    QString str_svr = ui->lineEdit->text();
    QString str_port = ui->lineEdit_2->text();
    if(_signal_client != nullptr){
        _signal_client->close();
    }
    _signal_client.reset(new rtcgw::JsonSignalingClient(this, str_svr.toStdString(), str_port.toUInt(), &_evt_queue, _signal_thread.get()) );
    _signal_client->open();
    _signal_client->signIn("A");

    _video_dev = new VideoCapture( {"VP9"} );
                _video_dev->setRender(_local_render);
                _video_dev->open(_video_dev_name);

}

static rtcgw::TimerQueue timer_queue;
static rtcgw::ThreadScope session_thread;
bool MainWindow::onCreateSession(const std::string& dialog_id, const std::string& session_id)
{
    auto s = std::make_shared<rtcgw::PeerSession>(session_id, &session_thread, &timer_queue);
    _dialog.reset(new rtcgw::Dialog(dialog_id, this));
    _dialog->setSession(s);

    return true;
}

bool MainWindow::onRemoteStreamEndpoint(const std::string &session_id, const std::string &mid
                                    , const rtcgw::EndpointInfo &cinfo)
{
    _dialog->setRemoteStreamEndpoint(session_id, mid, cinfo);

    return true;
}

void MainWindow::onAddStream(const std::string &session_id, const rtcgw::StreamInfos &sinfos, const rtcgw::TransportInfos &tinfos)
{
    std::vector<rtcgw::Stream *> streams = _dialog->addStreams(session_id, sinfos, tinfos);
    for(auto istream : streams){
        if(istream->type() == rtcgw::STREAM_VIDEO){
//            _video_dev = new VideoCapture( istream->sendCodecs() );
//            _video_dev->subscribe(istream);
//            _video_dev->setRender(_local_render);
//            _video_dev->open(_video_dev_name);

            istream->subscribe(_remote_render);
        }
    }
}

bool MainWindow::onSessionBind(const std::string &, const std::string &)
{
}

void MainWindow::onSetRemoteStream(const std::string& session_id, const rtcgw::StreamInfos &sinfos)
{
    _dialog->setRemoteStreamInfo(session_id, sinfos);
}

void MainWindow::onDelStream()
{

}

void MainWindow::onUpdateStream()
{

}

