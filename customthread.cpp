#include "customthread.h"

#include <QRandomGenerator>

CustomThread::CustomThread(QObject *parent)
    : QThread(parent)
{
    m_abort = false;
    m_pause = false;
}

CustomThread::~CustomThread()
{
    m_mutex.lock();
    m_abort = true;
    m_mutex.unlock();

    wait();
}

void CustomThread::processData()
{
    if (isRunning() && m_pause) {
        m_mutex.lock();
        m_pause = false;
        m_mutex.unlock();
        m_pauseCond.wakeAll();
    }

    m_abort = false;
    start();
}

void CustomThread::pauseProcess()
{
    m_mutex.lock();
    m_pause = true;
    m_mutex.unlock();
}

void CustomThread::run()
{
    while (!m_abort) {
        m_mutex.lock();
        if (m_pause)
            m_pauseCond.wait(&m_mutex);
        m_mutex.unlock();
        QPointF xy(QRandomGenerator::global()->bounded(50000), QRandomGenerator::global()->bounded(50000));
        emit sendData(xy);
        msleep(500);
    }
}

void CustomThread::stopProcess()
{
    m_mutex.lock();
    m_abort = true;
    if (m_pause)
        m_pauseCond.wakeAll();
    m_mutex.unlock();
}
