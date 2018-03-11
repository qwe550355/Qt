#ifndef FRESHTHREAD_H
#define FRESHTHREAD_H

#include <QObject>
#include"QThread"
#include"QMutex"

class FreshThread : public QThread
{
    Q_OBJECT
public:
    FreshThread( QObject *parent = 0);
    ~FreshThread();
    void run();
    void Pause();
    void Resume();
private:
    QMutex m_mutex;
    int tPause=0;
signals:
    void refreshPLC();
    void refreshLabel();
    void refreshChart();
};

#endif // FRESHTHREAD_H
