#ifndef INISC1000_H_1721217008
#define INISC1000_H_1721217008

#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>

namespace CONTROLLER {

namespace KI {
const QString GMainTain = "GMainTain";

const QString maintain_homingAllDevice = GMainTain + QString("/maintain_homingAllDevice");
const QString maintain_homingAllPump   = GMainTain + QString("/maintain_homingAllPump");
const QString maintain_discardTube     = GMainTain + QString("/maintain_discardTube");
const QString maintain_ableASubstrate  = GMainTain + QString("/maintain_ableASubstrate");
const QString maintain_ableBSubstrate  = GMainTain + QString("/maintain_ableBSubstrate");
const QString maintain_ableMagneticSeparationWash =
    GMainTain + QString("/maintain_ableMagneticSeparationWash");
const QString maintain_washDrain     = GMainTain + QString("/maintain_washDrain");
const QString maintain_washFill      = GMainTain + QString("/maintain_washFill");
const QString maintain_washIntensive = GMainTain + QString("/maintain_washIntensive");
const QString maintain_washoneclick  = GMainTain + QString("/maintain_washoneclick");

const QString GQrcode    = "GQrcode";
const QString qrcode_vid = GQrcode + QString("/qrcode_vid");

const QString GPrinter     = "GPrinter";
const QString printer_name = GQrcode + QString("/printer_name");

const QString GLte    = "GLte";
const QString lte_pid = GLte + QString("/lte_pid");
const QString lte_vid = GLte + QString("/lte_vid");

const QString GMCU         = "GMCU";
const QString mcu_portName = GMCU + QString("/mcu_portName");

const QString GMachine       = "GMachine";
const QString machine_number = GMachine + QString("/machine_number");

const QString GSetting               = "GSetting";
const QString setting_level_tracking = GSetting + QString("/setting_level_tracking");

}  // namespace KI

class IniSC1000 {
public:
    using Self = IniSC1000;
    static Self &instance() {
        static Self ini{};
        return ini;
    }

private:
    QScopedPointer<QSettings> m_ini;

public:
    IniSC1000();

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
}  // namespace CONTROLLER

#endif  // INISC1000_H_1721217008
