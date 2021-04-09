#ifndef VIDEOCAPTURE_H
#define VIDEOCAPTURE_H

#include "videorender.h"

#include <QAbstractVideoSurface>
#include <QCamera>

#include <source.h>
#include <rtputils/rtp_packetizer.h>



class VideoCapture : public rtcgw::SourceImpl,
        public QAbstractVideoSurface
{
public:
    VideoCapture(){};
    explicit VideoCapture(const std::vector<std::string>& request_codecs);
    bool open(const std::string& dev);
    void setRender(VideoRender *render);

    // QAbstractVideoSurface interface
public:
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType type) const override;
    bool isFormatSupported(const QVideoSurfaceFormat &format) const override;
    QVideoSurfaceFormat nearestFormat(const QVideoSurfaceFormat &format) const override;
    bool present(const QVideoFrame &frame)override;

private:
    QCamera* _camera=nullptr;
    VideoRender* _render=nullptr;
    void sendFrame(const uint8_t *data, int size, int w, int h);

};

#endif // VIDEOCAPTURE_H
