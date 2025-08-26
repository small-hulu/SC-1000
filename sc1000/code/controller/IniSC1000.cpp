#include "IniSC1000.h"

#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <mutex>

#include "global/GLOBAL_Path.hpp"

namespace CONTROLLER {
IniSC1000::IniSC1000() {
    static std::once_flag flag;
    std::call_once(flag, [this]() {
        QString path = to_absoluteFilePath(Path_IniFolder);
        QDir().mkpath(path);
        path = QDir(path).filePath(Path_IniFile);
        m_ini.reset(new QSettings(path, QSettings::IniFormat));
    });

    {
        using namespace KI;
        QSettings::SettingsMap smap;
        smap[maintain_homingAllDevice];
        smap[maintain_homingAllPump];
        smap[maintain_discardTube];
        smap[maintain_ableASubstrate];
        smap[maintain_ableBSubstrate];
        smap[maintain_ableMagneticSeparationWash];
        smap[maintain_washDrain];
        smap[maintain_washFill];
        smap[maintain_washIntensive];

        smap[qrcode_vid] = 0x18d1;

        smap[printer_name] = "HP LaserJet 1020";

        smap[lte_pid] = 0x6001;
        smap[lte_vid] = 0x0403;

        smap[mcu_portName] = "COM2";

        smap[machine_number] = "AY8000xxxxx";

        smap[setting_level_tracking] = 0;

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

}  // namespace CONTROLLER
