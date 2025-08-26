#ifndef SLOG_H_1725427787
#define SLOG_H_1725427787

#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QString>
#include <functional>
#include <mutex>

namespace LOG {
class SLog {
private:
    static inline const char* const                time_format = "yyyy-MM-dd hh:mm:ss";
    static QFile*                                  file_ptr;
    static QList<std::function<void(const SLog*)>> m_taskList;

public:
    static void Init_console(bool onoff);
    static void Init_file(const QString& path);

public:
    static void Append_task(std::function<void(const SLog*)> task);

private:
    static void log_in_console(const SLog* self);
    static void log_in_file(const SLog* self);

public:
    QString message;
    QDebug  qos;

public:
    QString time;
    QString file;
    QString func;
    int     line = -1;

public:
    int type = 0;

public:
    SLog(const QString& file, const QString& func, int line, int type) : qos(&message) {
        this->time = QDateTime::currentDateTime().toString(time_format);

        this->file = file;
        this->func = func;
        this->line = line;

        this->type = type;
    }

    ~SLog() {
        for (auto&& task : m_taskList) {
            task(this);
        }
    }

public:
    template <typename T>
    SLog& operator<<(T&& arg) {
        qos.noquote() << std::forward<T>(arg);
        return *this;
    }

private:
    QString merge_message() const {
        QString msg;
        msg += QString("Time[%1]").arg(time);
        msg += QString("File[%1]").arg(file);
        msg += QString("Func[%1]").arg(func);
        msg += QString("Line[%1]").arg(line);
        msg += QString("type[%1]").arg(type);
        return msg;
    }
};
}  // namespace LOG

/**
 * 基于项目的自定义不同级别和特诊的日志宏
 */
#define sLog_test()                                \
    LOG::SLog {                                    \
        __FILE__, __PRETTY_FUNCTION__, __LINE__, 0 \
    }

#define sLog_modbus()                              \
    LOG::SLog {                                    \
        __FILE__, __PRETTY_FUNCTION__, __LINE__, 1 \
    }

#define sLog_qrcode()                              \
    LOG::SLog {                                    \
        __FILE__, __PRETTY_FUNCTION__, __LINE__, 2 \
    }

#define sLog_emergency()                           \
    LOG::SLog {                                    \
        __FILE__, __PRETTY_FUNCTION__, __LINE__, 3 \
    }

///////////////////////////////////////////////////////////////
#define sLog_ReagentBin()                           \
    LOG::SLog {                                     \
        __FILE__, __PRETTY_FUNCTION__, __LINE__, 10 \
    }

#define sLog_PipettingNeedle()                      \
    LOG::SLog {                                     \
        __FILE__, __PRETTY_FUNCTION__, __LINE__, 11 \
    }

#define sLog_Gripper()                              \
    LOG::SLog {                                     \
        __FILE__, __PRETTY_FUNCTION__, __LINE__, 12 \
    }

#define sLog_MagneticSeparation()                   \
    LOG::SLog {                                     \
        __FILE__, __PRETTY_FUNCTION__, __LINE__, 13 \
    }

#define sLog_Mixing()                               \
    LOG::SLog {                                     \
        __FILE__, __PRETTY_FUNCTION__, __LINE__, 14 \
    }

#define sLog_Photoelectric()                        \
    LOG::SLog {                                     \
        __FILE__, __PRETTY_FUNCTION__, __LINE__, 15 \
    }

#define sLog_Thermostat()                           \
    LOG::SLog {                                     \
        __FILE__, __PRETTY_FUNCTION__, __LINE__, 16 \
    }

#define sLog_other()                                    \
    LOG::SLog {                                         \
        __FILE__, __PRETTY_FUNCTION__, __LINE__, 114514 \
    }

#endif  // SLOG_H_1725427787
