#ifndef DEVICEMANAGER_H_1718689556
#define DEVICEMANAGER_H_1718689556

#include <QDebug>
#include <QJsonObject>
#include <QMetaEnum>
#include <QObject>
#include <atomic>
#include <mutex>

namespace COM {

constexpr int DeviceStartAddress = 1000;

constexpr int DeviceAddress_ReagentBin         = 10;
constexpr int DeviceAddress_PipettingNeedle    = 11;
constexpr int DeviceAddress_Gripper            = 12;
constexpr int DeviceAddress_MagneticSeparation = 13;
constexpr int DeviceAddress_Mixing             = 14;
constexpr int DeviceAddress_Photoelectric      = 15;
constexpr int DeviceAddress_Thermostat         = 16;

struct DeviceManager : public QObject {
    Q_OBJECT
public:
    using ownership_type = std::atomic_bool;

    enum class atomic_flag : int {
        UnKnow = 0,          // [未知]
        CriticalZone,        // 夹爪和移液针的临界区
        Gripper,             // 夹爪
        PipettingNeedle,     // 移液针
        ReagentBin,          // 试剂仓
        MagneticSeparation,  // 磁分离
        Mixing,              // 混匀
        Photoelectric,       // 光电
        Incubate,            // 孵育盘
        Cup,                 // 分杯位
        Load,                // 上料位
        Dilute,              // 稀释位
        Throw,               // 抛料位

        Enum_Count,
    };
    Q_ENUM(atomic_flag)

    static QString to_string();

    /// no copy-move
    /// only ref
    static ownership_type& enum_to_owner(atomic_flag flag);
    static atomic_flag     string_to_enum(const QString& name);

    /// 保证了都是有效flag
    /// 无 UnKnow
    static QList<atomic_flag>     Analyze_flagList(const QJsonObject& json);
    static QList<ownership_type*> Analyze_ownerList(const QJsonObject& json);
};

inline uint qHash(const COM::DeviceManager::atomic_flag& t, uint seed = 0) {
    return (uint)t ^ seed;
}

inline QDebug operator<<(QDebug os, const COM::DeviceManager::atomic_flag& flag) {
    QMetaEnum qenum = QMetaEnum::fromType<COM::DeviceManager::atomic_flag>();
    return os << qenum.key(static_cast<int>(flag));
}

}  // namespace COM

#endif  // DEVICEMANAGER_H_1718689556
