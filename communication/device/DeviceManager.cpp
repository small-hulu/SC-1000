#include "DeviceManager.h"

#include <QDebug>
#include <QHash>
#include <QMetaEnum>
#include <QSet>

#include "../config/CKeyJson.hpp"

namespace COM {
namespace {
DeviceManager::ownership_type table[(int)DeviceManager::atomic_flag::Enum_Count];
}

QString DeviceManager::to_string() {
    QString   s = " ";
    QDebug    os(&s);
    QMetaEnum qenum = QMetaEnum::fromType<atomic_flag>();
    for (int i = 0; i < (int)atomic_flag::Enum_Count; i += 1) {
        os << &table[i] << qenum.key(i) << "\t = " << table[i].load() << "\n";
    }
    return s;
}

DeviceManager::ownership_type &DeviceManager::enum_to_owner(DeviceManager::atomic_flag flag) {
    return table[(int)flag];
}
//设备名转成设备的原子flag
DeviceManager::atomic_flag DeviceManager::string_to_enum(const QString &name) {
    using table_type        = QHash<QString, atomic_flag>;
    static table_type table = [] {
        table_type table;
        table[KJ::Gripper]            = atomic_flag::Gripper;
        table[KJ::PipettingNeedle]    = atomic_flag::PipettingNeedle;
        table[KJ::ReagentBin]         = atomic_flag::ReagentBin;
        table[KJ::MagneticSeparation] = atomic_flag::MagneticSeparation;
        table[KJ::Mixing]             = atomic_flag::Mixing;
        table[KJ::Photoelectric]      = atomic_flag::Photoelectric;
        table[KJ::Incubate]           = atomic_flag::Incubate;
        table[KJ::Cup]                = atomic_flag::Cup;
        table[KJ::Load]               = atomic_flag::Load;
        table[KJ::Dilute]             = atomic_flag::Dilute;
        table[KJ::Throw]              = atomic_flag::Throw;
        table[KJ::DoubleSuction]      = atomic_flag::PipettingNeedle;
        table[""]                     = atomic_flag::UnKnow;
        return table;
    }();

    return table[name];
}

QList<DeviceManager::atomic_flag> DeviceManager::Analyze_flagList(const QJsonObject &json) {
    QSet<atomic_flag> deviceList;
    QString           name = json[KJ::deviceName].toString();
    //字符串映射成枚举atomic_flag
    deviceList << string_to_enum(name);
    for (auto op : {KJ::op, KJ::from, KJ::to}) {
        auto value = json[op];
        if (value.isObject() == false) {
            continue;
        }
        auto obj = value.toObject();
        value    = obj[KJ::target];
        if (value.isString() == false) {
            continue;
        }
        deviceList << string_to_enum(value.toString());
    }

    /// 夹爪和移液针的互斥判断
    /// 1. 移液针无脑考虑
    if (deviceList.contains(atomic_flag::PipettingNeedle)) {
        deviceList << atomic_flag::CriticalZone;
        deviceList << atomic_flag::ReagentBin;
    }

    /// 2. 夹爪特殊考虑
    if (deviceList.contains(atomic_flag::Gripper)) {
        /// 分杯位
        if (deviceList.contains(atomic_flag::Cup)) {
            deviceList << atomic_flag::CriticalZone;
        }
        /// 上料位
        if (deviceList.contains(atomic_flag::Load)) {
            deviceList << atomic_flag::CriticalZone;
        }
        /// 混匀位
        if (deviceList.contains(atomic_flag::Dilute)) {
            deviceList << atomic_flag::CriticalZone;
        }
        /// 混匀位置
        if (deviceList.contains(atomic_flag::Mixing)) {
            deviceList << atomic_flag::CriticalZone;
        }
    }

    /// 3. 稀释位
    if (deviceList.contains(atomic_flag::Dilute)) {
        deviceList << atomic_flag::CriticalZone;
        deviceList << atomic_flag::PipettingNeedle;
        deviceList << atomic_flag::Gripper;
        deviceList << atomic_flag::Mixing;
    }

    /// 4. 稀释位
    if (deviceList.contains(atomic_flag::Mixing)) {
        deviceList << atomic_flag::CriticalZone;
        deviceList << atomic_flag::PipettingNeedle;
        deviceList << atomic_flag::Gripper;
    }

    /// 清除 unknow
    deviceList.remove(atomic_flag::UnKnow);
    return QList<DeviceManager::atomic_flag>(deviceList.begin(), deviceList.end());
}

QList<DeviceManager::ownership_type *> DeviceManager::Analyze_ownerList(const QJsonObject &json) {
    auto flagList = Analyze_flagList(json);

    QList<ownership_type *> res;
    for (auto flag : flagList) {
        res << &enum_to_owner(flag);
    }
    return res;
}
}  // namespace COM
