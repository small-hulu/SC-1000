#ifndef LOG_H_1725432216
#define LOG_H_1725432216

#include <QMetaEnum>
#include <QObject>

#include "EInterface_DB.hpp"

namespace ENTITY {
class LogEnum : public QObject {
    Q_OBJECT
public:
    enum LogType : int {
        Default   = 0,
        Modbus    = 1,
        Qrcode    = 2,
        Emergency = 3,

        ReagentBin         = 10,
        PipettingNeedle    = 11,
        Gripper            = 12,
        MagneticSeparation = 13,
        Mixing             = 14,
        Photoelectric      = 15,
        Thermostat         = 16,

        Other = 114514,
    };
    Q_ENUM(LogType);

    static QMetaEnum Get_qEnum() {
        QMetaEnum qenum = QMetaEnum::fromType<LogType>();
        return qenum;
    }

    static QString Get_logTypeString(int type) {
        switch ((LogType)type) {
        case Default           : return "默认单元";
        case Modbus            : return "设备控制模块";
        case Qrcode            : return "条码模块";
        case Emergency         : return "急诊模块";

        case ReagentBin        : return "试剂仓模块";
        case PipettingNeedle   : return "移液针模块";
        case Gripper           : return "夹爪模块";
        case MagneticSeparation: return "磁分离模块";
        case Mixing            : return "分杯混匀模块";
        case Photoelectric     : return "光电模块";
        case Thermostat        : return "温控模块";
        case Other             : return "其他模块";
        }
        return "";
    }
};

class Log : public EInterface_DB<::DB::TableEntity::log> {
public:
    using LogType = LogEnum::LogType;

public:
    static QString Get_logTypeString(int type) {
        return LogEnum::Get_logTypeString(type);
    }

public:
    Log() = default;

public:
    QString Get_TypeString() {
        return Get_logTypeString(table[attr::type].toInt());
    }
};
}  // namespace ENTITY

#endif  // LOG_H_1725432216
