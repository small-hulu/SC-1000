#include "CPhotoelectric.h"

#include <QJsonDocument>
#include <QJsonObject>

namespace COM {

QString CPhotoelectric::Params::to_string() const {
    if (false == isValid) {
        return QString("Data is not valid");
    }

    QJsonObject json;
    json["底物B容量"] = capacity;
    json["检测位置"]  = positionIn;
    json["debugPos"]  = position;

    return QString::fromUtf8(QJsonDocument(json).toJson(QJsonDocument::Indented));
}

QString CPhotoelectric::StateBits::to_string() const {
    if (false == isValid) {
        return QString("Data is not valid");
    }

    QJsonObject json;
    json["u1001-00-动作空闲"]         = u1001.b00_leisure;
    json["u1001-01-动作成功"]         = u1001.b01_success;
    json["u1001-02-检测失败"]         = u1001.b02_detectError;
    json["u1001-03-泵空"]             = u1001.b03_pumpIsEmpty;
    json["u1001-07-计数器初始化正常"] = u1001.b07_counterInit;

    json["u1001-08-部件错误"]  = u1001.b08_ErrorAssembly;
    json["u1001-12-动作超时"]  = u1001.b12_ErrorRunTimeOut;
    json["u1001-13-s驱动错误"] = u1001.b13_ErrorSDrive;
    json["u1001-14-x驱动错误"] = u1001.b14_ErrorXDrive;
    json["u1001-15-x电机堵转"] = u1001.b15_ErrorXLockedRotor;

    return QString::fromUtf8(QJsonDocument(json).toJson(QJsonDocument::Indented));
}

CLteErrorCode::ErrorPkg CPhotoelectric::StateBits::get_errorString() const {
    using ErrorCode = CLteErrorCode::ErrorCode;
    if (u1001.b12_ErrorRunTimeOut) {
        return {ErrorCode::SC1512, "动作超时"};
    }
    if (u1001.b13_ErrorSDrive) {
        return {ErrorCode::SC1513, "s驱动错误"};
    }
    if (u1001.b14_ErrorXDrive) {
        return {ErrorCode::SC1514, "x驱动错误"};
    }
    if (u1001.b15_ErrorXLockedRotor) {
        return {ErrorCode::SC1515, "x电机堵转"};
    }
    if (u1001.b08_ErrorAssembly) {
        return {ErrorCode::SC1508, "部件错误"};
    }
    return {};
}

}  // namespace COM
