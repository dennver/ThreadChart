#ifndef CUSTOMTHREAD_H
#define CUSTOMTHREAD_H

#include <QImage>
#include <QMutex>
#include <QWaitCondition>
#include <QThread>

// https://mayaposch.wordpress.com/2011/11/01/how-to-really-truly-use-qthreads-the-full-explanation/
// lots of ways to use qthread, it is the fastest

class CustomThread : public QThread
{
    Q_OBJECT

public:
    CustomThread(QObject *parent = nullptr);
    ~CustomThread();

signals:
    void sendData(QPointF);

public slots:
    void processData();
    void pauseProcess();
    void stopProcess();

protected:
    void run();

private:
    bool m_abort;
    bool m_pause;
    QMutex m_mutex;
    QWaitCondition m_pauseCond;

};

#endif
