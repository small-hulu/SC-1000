#include "LOG_SLog.h"

#include <QTextCodec>

namespace LOG {
QFile                                   *SLog::file_ptr = nullptr;
QList<std::function<void(const SLog *)>> SLog::m_taskList;

void SLog::Init_console(bool onoff) {
    if (onoff) {
        Append_task(&log_in_console);
    }
}

void SLog::Init_file(const QString &path) {
    QString dirPath = QFileInfo(path).absoluteDir().absolutePath();
    QDir    dir(dirPath);
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            qWarning() << dir << "mkpath-error";
            return;
        }
    }

    file_ptr = new QFile(path);
    if (file_ptr->open(QIODevice::WriteOnly | QIODevice::Append)) {
        Append_task(&log_in_file);
    } else {
        qWarning() << path << "file open-error";
        delete file_ptr;
        file_ptr = nullptr;
    }
}

void SLog::Append_task(std::function<void(const SLog *)> task) {
    m_taskList << std::move(task);
}

void SLog::log_in_console(const SLog *self) {
    QString str;
    str += QString(self->merge_message() + "\r\n");
    str += QString(self->message + "\r\n");

    static std::mutex           mtx;
    std::lock_guard<std::mutex> lock(mtx);
    auto                        ss = QTextStream(stdout);
    ss.setCodec("system");
    ss << (str);
}

void SLog::log_in_file(const SLog *self) {
    QString str;
    str += QString(self->merge_message() + "\r\n");
    str += QString(self->message + "\r\n");

    static std::mutex           mtx;
    std::lock_guard<std::mutex> lock(mtx);
    auto                        ss = QTextStream(file_ptr);
    ss.setCodec("utf-8");
    ss << (str);
}

}  // namespace LOG
