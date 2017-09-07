#include "MuxThread.h"

MuxThread::MuxThread(QObject *parent)
    : QThread(parent)
    , m_handle(NULL)
{

}

void MuxThread::run()
{
    MuxApi *muxApi = MuxApi::instance();
    muxApi->ptr_start_mux(m_handle);
}

void MuxThread::stop()
{

}

void MuxThread::setMuxHandle(MuxHandle *h)
{
    m_handle = h;
}
