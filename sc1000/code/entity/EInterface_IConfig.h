#ifndef ICONFIG_H_1725689198
#define ICONFIG_H_1725689198

#include <QJsonDocument>
#include <QJsonObject>

namespace ENTITY {

class IConfig {
protected:
    QJsonObject m_ojson;
    QJsonObject json;

public:
    IConfig();
    virtual ~IConfig() = default;

public:
    virtual bool Save();
};
}  // namespace ENTITY

#endif  // ICONFIG_H_1725689198
