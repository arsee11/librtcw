#include "videorender.h"
#include <QDebug>
#include <QVideoSurfaceFormat>

void YUV420P_TO_RGB24(const unsigned char *data, unsigned char *rgb, int width, int height) {
    int index = 0;
    const unsigned char *ybase = data;
    const unsigned char *ubase = &data[width * height];
    const unsigned char *vbase = &data[width * height * 5 / 4];
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            //YYYYYYYYUUVV
            u_char Y = ybase[x + y * width];
            u_char U = ubase[y / 2 * width / 2 + (x / 2)];
            u_char V = vbase[y / 2 * width / 2 + (x / 2)];


            rgb[index++] = Y + 1.4075 * (V - 128); //R
            rgb[index++] = Y - 0.3455 * (U - 128) - 0.7169 * (V - 128); //G
            rgb[index++] = Y + 1.779 * (U - 128); //B
        }
    }
}

VideoRender::VideoRender()
    : width_(0),
      height_(0)
{
}

VideoRender::~VideoRender() {
}

void VideoRender::SetSize(int width, int height) {

  if (width_ == width && height_ == height) {
    return;
  }

  width_ = width;
  height_ = height;
  image_.reset(new uint8_t[width * height * 3]);
}

int decode(const void* data, int size, uint8_t* out, int& out_size)
{

}

void VideoRender::onSubscribeFrame(const rtcgw::MediaFrame &frame)
{
    qDebug()<<__FUNCTION__;
    uint8_t decoded_data[640*480*3];
    int decoded_size=640*480*3;
    decode(frame.data, frame.size, decoded_data, decoded_size);
    if(decoded_size > 0){
        this->onFrame(decoded_data, frame.w, frame.h);
    }
}



void VideoRender::onSubscribePacket(const rtcgw::RtpPacket &packet)
{
    qDebug()<<__FUNCTION__<<" packet size:"<<packet.size()<<"mark:"<<packet.Marker();

}

//YUV420P fromate
void VideoRender::onFrame(const uint8_t* data, int w, int h)
{
    //qDebug()<<__FUNCTION__<<" frame("<<w<<","<<h<<")";
    SetSize(w, h);
    YUV420P_TO_RGB24(data, image_.get(), w, h);

    if(rgba_frame_cb != nullptr){
        rgba_frame_cb(image_.get(), width_*height_*3);
    }
}
