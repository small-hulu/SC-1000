#include "ToString.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

namespace COM {
QString to_string(const QModbusDataUnit &data) {
    QJsonObject json;
    json["registerType"] = data.registerType();
    json["startAddress"] = data.startAddress();
    json["valueCount"]   = (int)data.valueCount();

    QString buffer;
    QDebug  os(&buffer);
    os << data.values();
    json["values"] = buffer;

    return QString::fromUtf8(QJsonDocument(json).toJson(QJsonDocument::Indented));
}

QString to_string(const QSerialPortInfo &info) {
    QJsonObject json;
    json["isValid"]           = info.isValid();
    json["isBusy"]            = info.isBusy();
    json["isNull"]            = info.isNull();
    json["portName"]          = info.portName();
    json["systemLocation"]    = info.systemLocation();
    json["description"]       = info.description();
    json["manufacturer"]      = info.manufacturer();
    json["serialNumber"]      = info.serialNumber();
    json["productIdentifier"] = info.productIdentifier();
    json["vendorIdentifier"]  = info.vendorIdentifier();

    return QString::fromUtf8(QJsonDocument(json).toJson(QJsonDocument::Indented));
}
}  // namespace COM
