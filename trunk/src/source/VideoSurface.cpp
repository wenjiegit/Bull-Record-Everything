#include "VideoSurface.h"

#include <QDebug>

VideoSurface::VideoSurface(QObject *parent) : QAbstractVideoSurface(parent)
{

}

bool VideoSurface::present(const QVideoFrame &frame)
{
    QImage::Format fmt = QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat());
    if (fmt == QImage::Format_Invalid) {
        return true;
    }

    QVideoFrame &_frame = const_cast<QVideoFrame&> (frame);
    if (_frame.map(QAbstractVideoBuffer::ReadOnly)) {

        qDebug() << _frame.pixelFormat() << _frame.size() << fmt;

        if (m_cachedImage.size() != frame.size()) {
            m_cachedImage = QImage(_frame.size(), fmt);
        }

        memcpy(m_cachedImage.bits(), _frame.bits(), m_cachedImage.byteCount());


        _frame.unmap();

        QImage img = m_cachedImage.mirrored(false, true);
        emit image(img);
        //emit image(m_cachedImage);
    }

    return true;
}

QList<QVideoFrame::PixelFormat> VideoSurface::supportedPixelFormats(QAbstractVideoBuffer::HandleType type) const
{
    Q_UNUSED(type);

    QList<QVideoFrame::PixelFormat> px;

    px << QVideoFrame::Format_Invalid;
    px << QVideoFrame::Format_ARGB32;
    px << QVideoFrame::Format_ARGB32_Premultiplied;
    px << QVideoFrame::Format_RGB24;
    px << QVideoFrame::Format_RGB32;
    px << QVideoFrame::Format_RGB565;
    px << QVideoFrame::Format_RGB555;
    px << QVideoFrame::Format_ARGB8565_Premultiplied;
    px << QVideoFrame::Format_BGRA32;
    px << QVideoFrame::Format_BGRA32_Premultiplied;
    px << QVideoFrame::Format_BGR32;
    px << QVideoFrame::Format_BGR24;
    px << QVideoFrame::Format_BGR565;
    px << QVideoFrame::Format_BGR555;
    px << QVideoFrame::Format_BGRA5658_Premultiplied;
    px << QVideoFrame::Format_AYUV444;
    px << QVideoFrame::Format_AYUV444_Premultiplied;
    px << QVideoFrame::Format_YUV444;
    px << QVideoFrame::Format_YUV420P;
    px << QVideoFrame::Format_YV12;
    px << QVideoFrame::Format_UYVY;
    px << QVideoFrame::Format_YUYV;
    px << QVideoFrame::Format_NV12;
    px << QVideoFrame::Format_NV21;
    px << QVideoFrame::Format_IMC1;
    px << QVideoFrame::Format_IMC2;
    px << QVideoFrame::Format_IMC3;
    px << QVideoFrame::Format_IMC4;
    px << QVideoFrame::Format_Y8;
    px << QVideoFrame::Format_Y16;
    px << QVideoFrame::Format_Jpeg;
    px << QVideoFrame::Format_CameraRaw;
    px << QVideoFrame::Format_AdobeDng;

    return px;
}
