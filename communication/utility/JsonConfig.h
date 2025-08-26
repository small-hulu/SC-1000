#ifndef JSONCONFIG_H_1716433604
#define JSONCONFIG_H_1716433604

#include <QJsonArray>
#include <QJsonObject>

#include "../config/CKeyJson.hpp"

namespace COM {
class JsonConfig {
public:
    /// 是否有必要作为一个单例，有待决议
    using Self = JsonConfig;
    static Self& instance() {
        static Self obj{};
        return obj;
    }

private:
    QString     m_path;
    QJsonObject m_json;

public:
    JsonConfig();

public:
    /// 读取文件的内容到程序的缓存
    bool Refresh();
    /// 修改配置后，重新回写到文件中
    bool Save(const QJsonObject& json = {});

public:
    QJsonObject Get_json() {
        return m_json;
    }
    void Set_json(const QJsonObject& json) {
        m_json = json;
    }

public:
    void Set_value(const QString& key, const QJsonObject& json) {
        m_json[key] = json;
    }

public:
    QJsonValue operator[](const QString& key) {
        return m_json[key];
    }

    QJsonValue Get_value(const QString& key) {
        return m_json[key];
    }

    QJsonValue value(const QString& key) {
        return m_json[key];
    }
};
}  // namespace COM
#endif  // JSONCONFIG_H_1716433604
