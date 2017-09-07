#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMutex>
#include <QMutexLocker>

namespace Ui {
class MainWindow;
}

class MuxThread;
class QCamera;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void onAudioFrameAvailable(const QByteArray &data);

    int check_pcm_size(int size);
    void getPcmData(char *buf, int size);
    QImage getImage();

private slots:
    void on_start_clicked();

    void on_open_audio_clicked();

    void on_stop_clicked();

    void onCurrentCameraChanged(int index);

    void on_open_video_clicked();

    void onImage(const QImage &img);

private:
    Ui::MainWindow *ui;

    QByteArray m_pcm;
    QMutex mutex;
    QMutex mutex_video;
    QList<QImage> m_imgs;

    MuxThread* m_muxThread;
    QCamera* m_camera;
};

#endif // MAINWINDOW_H
