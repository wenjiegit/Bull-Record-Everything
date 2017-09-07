#ifndef AUDIOSOURCE_HPP
#define AUDIOSOURCE_HPP

#include "RtAudio.h"

#include <QAudioDeviceInfo>
#include <QThread>

#define AUDIO_DEVICE_STATE_ERROR    ( -1 )
#define AUDIO_DEVICE_STATE_OK       ( 0 )

class QTimer;
class AudioInfo;

struct AudioDeviceInfo {

    AudioDeviceInfo()
    {
        deviceId = -1;
    }

    bool operator==(const AudioDeviceInfo & other) const
    {
        return other.deviceId == deviceId
                && other.deviceName == deviceName;
    }

    QString deviceName;
    int deviceId;
};

class AudioCapture : public QThread
{
    Q_OBJECT
public:
    AudioCapture(QObject *parent = 0);
    ~AudioCapture();

    void setAudioDevice(const AudioDeviceInfo &info);
    void run();
    void stop();

    static QList<AudioDeviceInfo> getAudioDevice();

public:
    void stateChanged(int state);
    void audioFrameAvailable(void *inputBuffer, unsigned int nBufferFrames);

private:
    int getDeviceIdByInfo(const AudioDeviceInfo &info);

signals:
    void audioStateChanged(int state);
    void audioFrameAvailable(const QByteArray &data);
    void audioVolumeChanged(quint32 value, quint32 max);

private:
    AudioInfo *m_audioInfo;
    QAudioFormat m_format;
    bool m_stopThread;
    AudioDeviceInfo m_audioDeviceInfo;
    QByteArray m_cachedData;
};

class AudioInfo : public QObject
{
    Q_OBJECT

public:
    AudioInfo(const QAudioFormat &format, QObject *parent);
    ~AudioInfo();

    inline quint32 getAmplitude() const { return m_maxAmplitude; }
    inline quint32 getValue() const { return m_maxValue; }

    qint64 writeData(const char *data, qint64 len);

private:
    const QAudioFormat m_format;
    quint32 m_maxAmplitude;
    quint32 m_maxValue;
};

#endif // AUDIOSOURCE_HPP
