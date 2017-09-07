#ifndef MUXTHREAD_H
#define MUXTHREAD_H

#include <QThread>

#include "MuxApi.h"

class MuxThread : public QThread
{
    Q_OBJECT
public:
    explicit MuxThread(QObject *parent = 0);

    void run();
    void stop();

    void setMuxHandle(MuxHandle *h);

signals:

public slots:

private:
    bool _stop;
    MuxHandle *m_handle;
};

#endif // MUXTHREAD_H
