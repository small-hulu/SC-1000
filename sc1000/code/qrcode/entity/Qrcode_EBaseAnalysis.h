#ifndef EQRCODEANALYSIS_H_1722577595
#define EQRCODEANALYSIS_H_1722577595

#include <QByteArray>
#include <QSettings>

namespace QRCODE {
class EBaseAnalysis {
protected:
    QByteArray             m_bytes;
    QSettings::SettingsMap m_map;

public:
    EBaseAnalysis();
    EBaseAnalysis(const QByteArray& bytes);
    virtual ~EBaseAnalysis() = default;

public:
    virtual QString to_string() const;
    void            Reset(const QByteArray& bytes);
    QString         list_to_string(const QStringList& stringList) const;

public:
    QVariant operator[](const QString& key) const {
        return m_map[key];
    }
    QVariant Get_value(const QString& key) const {
        return m_map[key];
    }
    QByteArray Get_odata() const {
        return m_bytes;
    }
};
}  // namespace QRCODE
#endif  // EQRCODEANALYSIS_H_1722577595
