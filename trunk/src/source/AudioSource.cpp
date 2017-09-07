#include "AudioSource.h"

#include "MainWindow.h"

#include <QMessageBox>
#include <QIODevice>
#include <QtEndian>
#include <QDebug>

extern MainWindow *g_mainWindow;

#define AUDIO_OUPUT_SAMPLERATE                  ( 44100 )
#define AUDIO_OUTPUT_SAMPLETYPE                 ( 16 )
#define AUDIO_OUTPUT_CHANNELS                   ( 2 )

static void AudioDevideErrorCallback(RtError::Type type, const std::string &errorText, void *userData)
{
    Q_UNUSED(type);
    Q_UNUSED(errorText);
    AudioCapture *cap = (AudioCapture *)userData;
    cap->stateChanged(AUDIO_DEVICE_STATE_ERROR);
    cap->stop();
}

static int record(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
         double streamTime, RtAudioStreamStatus status, void *userData)
{
    Q_UNUSED(outputBuffer);
    Q_UNUSED(streamTime);

    if (status) {
        //xs_log_error("The output buffer ran low, likely causing a gap in the output sound.");
    }

    // qDebug() << nBufferFrames;
    AudioCapture *cap = (AudioCapture *)userData;
    cap->audioFrameAvailable(inputBuffer, nBufferFrames);

    return 0;
}

AudioCapture::AudioCapture(QObject *parent)
    : QThread(parent)
    , m_stopThread(false)
{
    m_format.setSampleRate(AUDIO_OUPUT_SAMPLERATE);
    m_format.setChannelCount(AUDIO_OUTPUT_CHANNELS);
    m_format.setSampleSize(AUDIO_OUTPUT_SAMPLETYPE);
    m_format.setCodec("audio/pcm");
    m_format.setByteOrder(QAudioFormat::LittleEndian);
    m_format.setSampleType(QAudioFormat::SignedInt);

    m_audioInfo = new AudioInfo(m_format, this);
}

AudioCapture::~AudioCapture()
{

}

void AudioCapture::setAudioDevice(const AudioDeviceInfo &info)
{
    m_audioDeviceInfo = info;
}

void AudioCapture::run()
{
    RtAudio adc;

    int devId = getDeviceIdByInfo(m_audioDeviceInfo);
    if (devId < 0) {
        stateChanged(AUDIO_DEVICE_STATE_ERROR);
        return;
    }

    RtAudio::StreamParameters parameters;
    parameters.deviceId = devId;
    parameters.nChannels = m_format.channelCount();
    parameters.firstChannel = 0;
    unsigned int sampleRate = m_format.sampleRate();
    unsigned int bufferFrames = 1764;   // 40ms per call

    try {
        adc.openStream(NULL, &parameters, RTAUDIO_SINT16, sampleRate, &bufferFrames, &record, this, NULL, AudioDevideErrorCallback);
        adc.startStream();

        stateChanged(AUDIO_DEVICE_STATE_OK);
    } catch (RtError& e) {
        e.printMessage();
        stateChanged(AUDIO_DEVICE_STATE_ERROR);
        return;
    }

    while (!m_stopThread) {
        msleep(50);
    }


    // clean
    try {
        adc.stopStream();
    } catch (RtError& e) {
        e.printMessage();
    }

    if ( adc.isStreamOpen() ) {
        adc.closeStream();
    }
}

void AudioCapture::stop()
{
    m_stopThread = true;
}

QList<AudioDeviceInfo> AudioCapture::getAudioDevice()
{
    QList<AudioDeviceInfo> devs;
    RtAudio adc;
    int count = adc.getDeviceCount();
    for (int i = 0; i < count; ++i) {
        RtAudio::DeviceInfo dev = adc.getDeviceInfo(i);
        if (dev.inputChannels >= 2) {
            AudioDeviceInfo info;
            info.deviceId = i;

#ifdef Q_OS_WIN
            info.deviceName = QString::fromStdString(dev.name);
#else
            QByteArray ba(dev.name.data(), dev.name.size());
            info.deviceName = Utils::gb2312ToUtf8(ba);
#endif
            devs << info;
        }
    }

    return devs;
}

void AudioCapture::stateChanged(int state)
{
    if (state == AUDIO_DEVICE_STATE_ERROR) {
        quint32 volume = 0;
        quint32 amp = m_audioInfo->getAmplitude();

        emit audioVolumeChanged(volume, amp);
    }

    emit audioStateChanged(state);
}

void AudioCapture::audioFrameAvailable(void *inputBuffer, unsigned int nBufferFrames)
{
    QByteArray data((char*)inputBuffer, nBufferFrames * 4);

#if 0
    m_cachedData.append(data);
    if (m_cachedData.size() >= CACHE_SIZE) {
        (void)m_audioInfo->writeData(m_cachedData.constData(), m_cachedData.length());
        quint32 volume = m_audioInfo->getValue();
        quint32 amp = m_audioInfo->getAmplitude();

        emit audioVolumeChanged(volume, amp);
        m_cachedData.clear();
    }
#endif

    (void)m_audioInfo->writeData(data.constData(), data.length());
    quint32 volume = m_audioInfo->getValue();
    quint32 amp = m_audioInfo->getAmplitude();

    emit audioVolumeChanged(volume, amp);
    if (!m_stopThread) {
        g_mainWindow->onAudioFrameAvailable(data);
    }
}

int AudioCapture::getDeviceIdByInfo(const AudioDeviceInfo &info)
{
    int deviceId = -1;
    QList<AudioDeviceInfo> devs = getAudioDevice();
    for (int i = 0; i < devs.size(); ++i) {
        const AudioDeviceInfo &adi = devs.at(i);
        if (adi.deviceName == info.deviceName) {
            deviceId = adi.deviceId;
            break;
        }
    }

    return deviceId;
}

AudioInfo::AudioInfo(const QAudioFormat &format, QObject *parent)
    :   QObject(parent)
    ,   m_format(format)
    ,   m_maxAmplitude(0)

{
    switch (m_format.sampleSize()) {
    case 8:
        switch (m_format.sampleType()) {
        case QAudioFormat::UnSignedInt:
            m_maxAmplitude = 255;
            break;
        case QAudioFormat::SignedInt:
            m_maxAmplitude = 127;
            break;
        default:
            break;
        }
        break;
    case 16:
        switch (m_format.sampleType()) {
        case QAudioFormat::UnSignedInt:
            m_maxAmplitude = 65535;
            break;
        case QAudioFormat::SignedInt:
            m_maxAmplitude = 32767;
            break;
        default:
            break;
        }
        break;

    case 32:
        switch (m_format.sampleType()) {
        case QAudioFormat::UnSignedInt:
            m_maxAmplitude = 0xffffffff;
            break;
        case QAudioFormat::SignedInt:
            m_maxAmplitude = 0x7fffffff;
            break;
        case QAudioFormat::Float:
            m_maxAmplitude = 0x7fffffff; // Kind of
        default:
            break;
        }
        break;

    default:
        break;
    }
}

AudioInfo::~AudioInfo()
{

}

qint64 AudioInfo::writeData(const char *data, qint64 len)
{
    if (m_maxAmplitude) {
        Q_ASSERT(m_format.sampleSize() % 8 == 0);
        const int channelBytes = m_format.sampleSize() / 8;
        const int sampleBytes = m_format.channelCount() * channelBytes;
        Q_ASSERT(len % sampleBytes == 0);
        const int numSamples = len / sampleBytes;

        quint32 maxValue = 0;
        const unsigned char *ptr = reinterpret_cast<const unsigned char *>(data);

        for (int i = 0; i < numSamples; ++i) {
            for (int j = 0; j < m_format.channelCount(); ++j) {
                quint32 value = 0;

                if (m_format.sampleSize() == 8 && m_format.sampleType() == QAudioFormat::UnSignedInt) {
                    value = *reinterpret_cast<const quint8*>(ptr);
                } else if (m_format.sampleSize() == 8 && m_format.sampleType() == QAudioFormat::SignedInt) {
                    value = qAbs(*reinterpret_cast<const qint8*>(ptr));
                } else if (m_format.sampleSize() == 16 && m_format.sampleType() == QAudioFormat::UnSignedInt) {
                    if (m_format.byteOrder() == QAudioFormat::LittleEndian)
                        value = qFromLittleEndian<quint16>(ptr);
                    else
                        value = qFromBigEndian<quint16>(ptr);
                } else if (m_format.sampleSize() == 16 && m_format.sampleType() == QAudioFormat::SignedInt) {
                    if (m_format.byteOrder() == QAudioFormat::LittleEndian)
                        value = qAbs(qFromLittleEndian<qint16>(ptr));
                    else
                        value = qAbs(qFromBigEndian<qint16>(ptr));
                } else if (m_format.sampleSize() == 32 && m_format.sampleType() == QAudioFormat::UnSignedInt) {
                    if (m_format.byteOrder() == QAudioFormat::LittleEndian)
                        value = qFromLittleEndian<quint32>(ptr);
                    else
                        value = qFromBigEndian<quint32>(ptr);
                } else if (m_format.sampleSize() == 32 && m_format.sampleType() == QAudioFormat::SignedInt) {
                    if (m_format.byteOrder() == QAudioFormat::LittleEndian)
                        value = qAbs(qFromLittleEndian<qint32>(ptr));
                    else
                        value = qAbs(qFromBigEndian<qint32>(ptr));
                } else if (m_format.sampleSize() == 32 && m_format.sampleType() == QAudioFormat::Float) {
                    value = qAbs(*reinterpret_cast<const float*>(ptr) * 0x7fffffff); // assumes 0-1.0
                }

                maxValue = qMax(value, maxValue);
                ptr += channelBytes;
            }
        }

        maxValue = qMin(maxValue, m_maxAmplitude);
        m_maxValue = maxValue;
    }

    return len;
}
