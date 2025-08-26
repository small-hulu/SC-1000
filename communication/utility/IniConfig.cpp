#include "IniConfig.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <mutex>

#include "../config/CPath.hpp"

namespace COM {

IniConfig::IniConfig() {
    static std::once_flag flag;
    std::call_once(flag, [this]() {
        QString path = to_absoluteFilePath(Path_ComFolder);
        QDir().mkpath(path);
        path = QDir(path).filePath(Path_config);
        m_ini.reset(new QSettings(path, QSettings::IniFormat));
    });

    {
        using namespace KI;
        QSettings::SettingsMap smap;
        smap[modbus_serial_vid]      = 0x1A86;
        smap[modbus_serial_pid]      = 0x55d3;
        smap[modbus_timeout]         = 100;
        smap[modbus_numberOfRetries] = 3;
        smap[time_taskInterval]      = 10;

        smap[enable_initMCU]          = 1;
        smap[enable_loopLoadMaterial] = 1;
        smap[enable_loopEachDevice]   = 1;

        smap[pipetteNeedle_DrainWashTime]          = 30000;
        smap[magneticSeparation_spinTime]          = 15000;
        smap[ASubstrate_floodQuantity]             = 300;   //底物A的排放量
        smap[BSubstrate_floodQuantity]             = 300;   //底物B的排放量
        smap[washMagneticSeparation_floodQuantity] = 150;   //磁分离清洗液加注量
        //随量跟踪
        smap[pipetteNeedle_cavityAHeightMM]        = 4.075379855967925;
        smap[pipetteNeedle_cavityBHeightMM]        = 3.906793288441674;
        smap[pipetteNeedle_cavityCHeightMM]        = 2.80195844566735;
        smap[pipetteNeedle_cavityDHeightMM]        = 4.0185568;
        smap[pipetteNeedle_cavityStepCompensation] = 0;

        smap[json_fix] = "device-template.json";

        smap[bootloader_enable] = 1;

        smap[temp_Photoelectric]    = 0;
        smap[temp_Incubate]         = 0;
        smap[temp_ReagentBinLid]    = 0;
        smap[photoelectric_valRate] = 1.0;

        smap[time_drainage]             = 3 * 60 * 1000;
        smap[enable_fillBottle]         = 1;
        smap[enable_reagentBinSpinLoop] = 1;

        for (auto it = smap.begin(); it != smap.end(); it += 1) {
            auto key  = it.key();
            auto data = it.value();
            if (m_ini->contains(key) == false) {
                m_ini->setValue(key, data);
            }
        }
        m_ini->sync();
    }
}
}  // namespace COM
