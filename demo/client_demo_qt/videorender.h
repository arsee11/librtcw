#ifndef VIDEORENDER_H
#define VIDEORENDER_H

#include <QAbstractVideoSurface>
#include <observer.h>



class VideoRender : public rtcgw::Observer
{
    using RecvRgbaFrameCb = std::function<void(uint8_t* data, int size)>;

public:
    VideoRender();
    virtual ~VideoRender();

    void onFrame(const uint8_t *data, int w, int h);

    const uint8_t* image() const { return image_.get(); }

    int width() const { return width_; }

    int height() const { return height_; }

    RecvRgbaFrameCb rgba_frame_cb=nullptr;

protected:
    void SetSize(int width, int height);
    std::unique_ptr<uint8_t[]> image_;
    int width_;
    int height_;

    // Observer interface
public:
    void onSubscribeFrame(const rtcgw::MediaFrame& frame) override;
    void onSubscribePacket(const rtcgw::RtpPacket &packet)override;

};
#endif // VIDEORENDER_H
