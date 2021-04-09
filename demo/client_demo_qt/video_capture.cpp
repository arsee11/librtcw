#include "video_capture.h"

#include <QCameraInfo>
#include <QVideoSurfaceFormat>
#include <time_clocks.h>

VideoCapture::VideoCapture(const std::vector<std::string> &request_codecs)
{
    for(auto codec : request_codecs){
        if(codec == "VP9"){
        }
    }
}

bool VideoCapture::open(const std::string &dev)
{
    auto cams = QCameraInfo::availableCameras();
    for(auto icam : cams){
        qDebug()<<"camera:"<<icam.deviceName();
    }
    _camera = new QCamera(cams[0], this);
    _camera->setViewfinder(this);
    _camera->setCaptureMode(QCamera::CaptureMode::CaptureViewfinder);
    _camera->start();

    return true;
}

void VideoCapture::setRender(VideoRender *render)
{
    _render = render;
}

QList<QVideoFrame::PixelFormat> VideoCapture::supportedPixelFormats(QAbstractVideoBuffer::HandleType type) const
{
    return QList<QVideoFrame::PixelFormat>({QVideoFrame::Format_YUV420P});
}

bool VideoCapture::isFormatSupported(const QVideoSurfaceFormat &format) const
{
    return true;
}

QVideoSurfaceFormat VideoCapture::nearestFormat(const QVideoSurfaceFormat &format) const
{
    return QVideoSurfaceFormat(QSize(640, 480), QVideoFrame::PixelFormat::Format_YUV420P);
}

bool VideoCapture::present(const QVideoFrame &frame)
{
    QVideoFrame::PixelFormat format = frame.pixelFormat();
    int w = frame.width();
    int h = frame.height();
    //qDebug()<<__FUNCTION__<<" frame("<<format<<","<<w<<","<<h<<")";
    QVideoFrame frame_c = frame;
    frame_c.map(QAbstractVideoBuffer::MapMode::ReadOnly);
    if(_render != nullptr){
        _render->onFrame(frame_c.bits(), w, h);
    }
    int bytes_perline =0;
    for(int i=0; i<frame_c.planeCount(); i++){
        bytes_perline +=frame_c.bytesPerLine(i);
    }
    sendFrame(frame_c.bits(),bytes_perline*h, w, h);
    return true;
}

int encode(const void* data, int size, uint8_t* out, int& out_size)
{

}

void VideoCapture::sendFrame(const uint8_t* data, int size, int w, int h)
{
    uint8_t encoded_frame[640*480];
    int encoded_size=640*480;
    encode(data, size, encoded_frame, encoded_size);
    if(encoded_size > 0){
        rtcgw::MediaFrame frame;
        frame.data = (void*)data;
        frame.size = size;
        frame.w = w;
        frame.h = h;
        frame.timestamp_ms = rtcgw::Clock::timestamp_ms();
        //this->pushFrame(frame);
    }
}
