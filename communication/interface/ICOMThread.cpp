#include "ICOMThread.h"

ICOMThread::~ICOMThread() {
    qInfo() << QString("~QThread(%1)start").arg(objectName());
    Stop_thread();
    qInfo() << QString("~QThread(%1)end").arg(objectName());
}

void ICOMThread::Start_thread() {
    if (isRunning()) {
        Stop_thread();
    }

    th_isRunning = true;
    QThread::start();
}

void ICOMThread::Stop_thread() {
    th_isRunning = false;
    th_taskCV.notify_all();
    if (this->isRunning()) {
        qInfo() << QString("~QThread(%1) thread-quit").arg(objectName());
        this->quit();
        this->wait();
    }
}

void ICOMThread::Clear_threadResource() {}
