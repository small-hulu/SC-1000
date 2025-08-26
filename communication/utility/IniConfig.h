#ifndef INICONFIG_H_1716347503
#define INICONFIG_H_1716347503

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QScopedPointer>
#include <QSettings>

namespace COM {
namespace KI {
const QString GModbusConfig          = "GModbusConfig";
const QString modbus_serial_vid      = GModbusConfig + QString("/modbus_serial_vid");
const QString modbus_serial_pid      = GModbusConfig + QString("/modbus_serial_pid");
const QString modbus_timeout         = GModbusConfig + QString("/modbus_timeout");
const QString modbus_numberOfRetries = GModbusConfig + QString("/modbus_numberOfRetries");
const QString time_taskInterval      = GModbusConfig + QString("/time_taskInterval");

const QString GModbusStrategy         = "GModbusStrategy";
const QString enable_initMCU          = GModbusStrategy + QString("/enable_initMCU");
const QString enable_loopEachDevice   = GModbusStrategy + QString("/enable_loopEachDevice");
const QString enable_loopLoadMaterial = GModbusStrategy + QString("/enable_loopLoadMaterial");

const QString GFile    = "GFile";
const QString json_fix = GFile + QString("/json_fix");

const QString GDeviceFixParams = "GDeviceFixParams";
const QString pipetteNeedle_DrainWashTime =
    GDeviceFixParams + QString("/pipetteNeedle_DrainWashTime");
const QString ASubstrate_floodQuantity = GDeviceFixParams + QString("/ASubstrate_floodQuantity");
const QString BSubstrate_floodQuantity = GDeviceFixParams + QString("/BSubstrate_floodQuantity");
const QString washMagneticSeparation_floodQuantity =
    GDeviceFixParams + QString("/washMagneticSeparation_floodQuantity");
const QString magneticSeparation_spinTime =
    GDeviceFixParams + QString("/magneticSeparation_spinTime");
const QString pipetteNeedle_cavityAHeightMM =
    GDeviceFixParams + QString("/pipetteNeedle_cavityAHeightMM");
const QString pipetteNeedle_cavityBHeightMM =
    GDeviceFixParams + QString("/pipetteNeedle_cavityBHeightMM");
const QString pipetteNeedle_cavityCHeightMM =
    GDeviceFixParams + QString("/pipetteNeedle_cavityCHeightMM");
const QString pipetteNeedle_cavityDHeightMM =
    GDeviceFixParams + QString("/pipetteNeedle_cavityDHeightMM");
const QString pipetteNeedle_cavityStepCompensation =
    GDeviceFixParams + QString("/pipetteNeedle_cavityStepCompensation");

const QString GBootloader       = "GBootloader";
const QString bootloader_enable = GBootloader + QString("/bootloader_enable");

const QString GOffset               = "GOffset";
const QString temp_Photoelectric    = GOffset + QString("/temp_Photoelectric");
const QString temp_Incubate         = GOffset + QString("/temp_Incubate");
const QString temp_ReagentBinLid    = GOffset + QString("/temp_ReagentBinLid");
const QString photoelectric_valRate = GOffset + QString("/photoelectric_valRate");

const QString GWatchDog                 = "GWatchDog";
const QString time_drainage             = GWatchDog + QString("/time_drainage");
const QString enable_fillBottle         = GWatchDog + QString("/enable_fillBottle");
const QString enable_reagentBinSpinLoop = GWatchDog + QString("/enable_reagentBinSpinLoop");
}  // namespace KI

class IniConfig {
public:
    using Self = IniConfig;
    static Self &instance() {
        static Self ini{};
        return ini;
    }

private:
    QScopedPointer<QSettings> m_ini;

public:
    IniConfig();

public:
    QString to_string() const {
        QJsonObject json;
        for (auto key : m_ini->allKeys()) {
            json[key] = m_ini->value(key).toString();
        }
        return QString::fromUtf8(QJsonDocument(json).toJson(QJsonDocument::Indented));
    }

public:
    void setValue(const QString &key, QVariant var) {
        qInfo() << QString("%1 = [%2] -> [%3]")
                       .arg(key)
                       .arg(m_ini->value(key).toString())
                       .arg(var.toString());
        m_ini->setValue(key, var);
        m_ini->sync();
    }

    QVariant operator[](const QString &key) {
        return m_ini->value(key);
    }
};

}  // namespace COM

#endif  // INICONFIG_H_1716347503
