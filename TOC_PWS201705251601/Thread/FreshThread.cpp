#include "FreshThread.h"

FreshThread::FreshThread( QObject *parent)
    :QThread(parent)
{
    m_mutex.lock();
}
//---------------------------------------------------------------------------
FreshThread::~FreshThread()
{
    m_mutex.unlock();
}
//---------------------------------------------------------------------------
void FreshThread::run()
{
    while(1)
    {
        emit refreshPLC();
        emit refreshLabel();
        if(tPause==10)
        {
            emit refreshChart();
            tPause=0;
        }
        tPause++;
        msleep(100);

    }
    exec();
}
//---------------------------------------------------------------------------
void FreshThread::Pause()
{
    m_mutex.lock();
}
//---------------------------------------------------------------------------
void FreshThread::Resume()
{
    m_mutex.unlock();
}
