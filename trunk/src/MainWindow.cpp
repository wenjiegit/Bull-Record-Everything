#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QLibrary>
#include <QPainter>
#include <QCameraInfo>
#include <QMutexLocker>
#include <QDateTime>
#include <QDebug>

#include "AudioSource.h"
#include "MuxApi.h"
#include "MuxThread.h"
#include "VideoSurface.h"

Q_DECLARE_METATYPE(AudioDeviceInfo)
Q_DECLARE_METATYPE(QCameraInfo)

#define VIDEO_WIDTH  ( 640 )
#define VIDEO_HEIGHT ( 480 )

static int check_video(int bytes, void *user_data)
{
    Q_UNUSED(bytes);
    Q_UNUSED(user_data);

    return 0;
}

static int check_audio(int bytes, void *user_data)
{
    MainWindow *user = (MainWindow*)(user_data);
    return user->check_pcm_size(bytes);
}

static int get_video(char* rgb24, void *user_data)
{
    MainWindow *user = (MainWindow*)(user_data);
    QImage img = user->getImage();

    memcpy(rgb24, img.bits(), img.byteCount());

    return 0;
}

static int get_audio(char* s16, int frame_size, int chs, void *user_data)
{
    int size = frame_size * chs  * 2;
    MainWindow *user = (MainWindow*)(user_data);
    user->getPcmData(s16, size);

    return 0;
}

MainWindow *g_mainWindow;
MuxHandle *g_handle = NULL;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_muxThread(NULL)
    , m_camera(NULL)
{
    ui->setupUi(this);

    g_mainWindow = this;

    // reset window title
    setWindowTitle(tr("Bull-Record-Everything"));

    // add micro-phone devices
    if (true) {
        QList<AudioDeviceInfo> infos = AudioCapture::getAudioDevice();
        for (int i = 0; i < infos.size(); ++i) {
            const AudioDeviceInfo &info = infos.at(i);
            ui->audio->addItem(info.deviceName, QVariant::fromValue(info));
        }

        bool res = MuxApi::instance()->init("libmux.dll");
        // TODO add error code
        qDebug() << res;
    }

    // add camera devices
    if (true) {
        QList<QCameraInfo> infos = QCameraInfo::availableCameras();
        for (int i = 0; i < infos.size(); ++i) {
            const QCameraInfo &info = infos.at(i);
            ui->video->addItem(info.description(), QVariant::fromValue(info));
        }

        connect(ui->video, SIGNAL(currentIndexChanged(int)), this, SLOT(onCurrentCameraChanged(int)));
    }

    // add dumy image
    if (true) {
        QImage img(VIDEO_WIDTH, VIDEO_HEIGHT, QImage::Format_RGB888);
        img.fill(Qt::black);
        m_imgs << img;
    }

    ui->videoDisplayLabel->lower();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onAudioFrameAvailable(const QByteArray &data)
{
    mutex.lock();
    m_pcm.append(data);

    if (m_pcm.size() > 20480) {
        m_pcm.remove(0, 12288);
    }

    mutex.unlock();
}

int MainWindow::check_pcm_size(int size)
{
    mutex.lock();
    int ret = m_pcm.size() >= size ? 0 : 1;
    mutex.unlock();

    qDebug() << size << m_pcm.size() << ret;

    return ret;
}

void MainWindow::getPcmData(char *buf, int size)
{
    mutex.lock();

    memcpy(buf, m_pcm.data(), size);
    m_pcm.remove(0, size);

    mutex.unlock();
}

QImage MainWindow::getImage()
{
    QMutexLocker lk(&mutex_video);

    if (m_imgs.size() >= 2) {
        return m_imgs.takeFirst();
    } else {
        return m_imgs.first();
    }
}

void MainWindow::on_start_clicked()
{
    QCameraViewfinderSettings ds = ui->res->currentData().value<QCameraViewfinderSettings>();

    std::string timeStr = QString("%1.mp4").arg(QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss")).toStdString();

    MuxApi *muxApi = MuxApi::instance();
    MuxHandle *h = muxApi->ptr_create_mux();
    h->ouput_filename = qstrdup(timeStr.c_str());//const_cast<char*>("out1.mp4");
    h->user_data = this;
    h->video_width = ds.resolution().width();
    h->video_height = ds.resolution().height();

    h->video_bit_rate = 800000;
    h->audio_bit_rate = 96000;

    h->check_video = check_video;
    h->check_audio = check_audio;
    h->get_video = get_video;
    h->get_audio = get_audio;

    g_handle = h;

    m_muxThread = new MuxThread(this);
    m_muxThread->setMuxHandle(h);
    m_muxThread->start();
}

void MainWindow::on_open_audio_clicked()
{
    AudioCapture *cap = new AudioCapture;

    AudioDeviceInfo info = ui->audio->currentData().value<AudioDeviceInfo>();
    cap->setAudioDevice(info);
    cap->start();
}

void MainWindow::on_stop_clicked()
{
    MuxApi *muxApi = MuxApi::instance();

    // request
    muxApi->ptr_stop_mux(g_handle);

    // wait
    m_muxThread->wait();

    // clean
    muxApi->ptr_destroy_mux(g_handle);
}

void MainWindow::onCurrentCameraChanged(int index)
{
    QVariant _d = ui->video->itemData(index);
    QCameraInfo info = _d.value<QCameraInfo>();

    QCamera* camera = new QCamera(info);
    camera->load();

    qDebug() << camera->supportedViewfinderPixelFormats();

    ui->res->clear();
    QList<QCameraViewfinderSettings> ss = camera->supportedViewfinderSettings();
    for (int i = 0; i < ss.size(); ++i) {
        const QCameraViewfinderSettings &s = ss.at(i);

        QString text = QString("%1x%2 %3-%4 fps").arg(s.resolution().width()).arg(s.resolution().height())
                .arg(s.minimumFrameRate()).arg(s.maximumFrameRate());

        ui->res->addItem(text, QVariant::fromValue(s));
    }

    camera->deleteLater();

    qDebug() << index << ss.size();
}

void MainWindow::on_open_video_clicked()
{
    if (m_camera) {
        m_camera->stop();
        m_camera->deleteLater();
    }

    QVariant _d = ui->video->currentData();
    QCameraInfo info = _d.value<QCameraInfo>();

    QCameraViewfinderSettings viewfinderSettings;
    viewfinderSettings.setResolution(80, 60);

    m_camera = new QCamera(info);

    m_camera->load();

    QCameraViewfinderSettings ds = ui->res->currentData().value<QCameraViewfinderSettings>();
    //ds.setPixelFormat(QVideoFrame::Format_RGB24);

    VideoSurface *vs = new VideoSurface;
    m_camera->setViewfinder(vs);
    m_camera->setViewfinderSettings(ds);

    m_camera->start();

    connect(vs, SIGNAL(image(QImage)), this, SLOT(onImage(QImage)));
}

void MainWindow::onImage(const QImage &img)
{
    if (ui->videoDisplayLabel->size() != img.size()) {
        ui->videoDisplayLabel->resize(img.size());
    }

    ui->videoDisplayLabel->setPixmap(QPixmap::fromImage(img));

    mutex_video.lock();

    if (img.format() != QImage::Format_RGB888) {
        m_imgs << img.convertToFormat(QImage::Format_RGB888);
    } else {
        m_imgs << img;
    }

    if (m_imgs.size() >= 3) {
        m_imgs.removeFirst();
    }

    mutex_video.unlock();
}
