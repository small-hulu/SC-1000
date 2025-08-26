#include "CMagneticSeparation.h"

#include <QJsonDocument>
#include <QJsonObject>

namespace COM {

QString CMagneticSeparation::Params::to_string() const {
    if (false == isValid) {
        return QString("Data is not valid");
    }

    QJsonObject json;
    json["position"]           = position;
    json["washCapacity"]       = washCapacity;
    json["aSubstrateCapacity"] = aSubstrateCapacity;
    json["drainDeep"]          = drainDeep;
    json["mixHeight"]          = mixHeight;
    json["zAxisHeight"]        = zAxisHeight;

    return QString::fromUtf8(QJsonDocument(json).toJson(QJsonDocument::Indented));
}

QString CMagneticSeparation::StateBits::to_string() const {
    if (false == isValid) {
        return QString("Data is not valid");
    }

    QJsonObject json;
    json["u1002-00-磁分离运动标志"] = u1002.b00_run;
    json["u1002-01-z轴位置标志"]    = u1002.b01_zAxisRun;
    json["u1002-03-执行成功标志"]   = u1002.b03_success;
    json["u1002-04-拒绝执行标志"]   = u1002.b04_refuse;
    json["u1002-05-碰撞标志"]       = u1002.b05_crash;
    json["u1002-08-模式准备完成"]   = u1002.b08_modeReady;
    json["u1002-12-动作空闲"]       = u1002.b12_leisure;

    json["u1003-00-自检完成标志"] = u1003.b00_selfCheck;
    json["u1003-01-部件错误"]     = u1003.b01_ErrorAssembly;
    json["u1003-07-S驱动"]        = u1003.b07_ErrorSDrive;
    json["u1003-08-W驱动"]        = u1003.b08_ErrorWDrive;
    json["u1003-09-Z驱动"]        = u1003.b09_ErrorZDrive;
    json["u1003-10-R驱动"]        = u1003.b10_ErrorRDrive;
    json["u1003-11-P驱动"]        = u1003.b11_ErrorPDrive;
    json["u1003-12-动作超时"]     = u1003.b12_ErrorRunTimeOut;
    json["u1003-13-Z堵转"]        = u1003.b13_ErrorZLockedRotor;
    json["u1003-14-R堵转"]        = u1003.b14_ErrorRLockedRotor;
    json["u1003-15-P堵转"]        = u1003.b15_ErrorPLockedRotor;

    return QString::fromUtf8(QJsonDocument(json).toJson(QJsonDocument::Indented));
}

CLteErrorCode::ErrorPkg CMagneticSeparation::StateBits::get_errorString() const {
    using ErrorCode = CLteErrorCode::ErrorCode;
    if (u1003.b07_ErrorSDrive) {
        return {ErrorCode::SC1307, "S驱动"};
    }
    if (u1003.b08_ErrorWDrive) {
        return {ErrorCode::SC1308, "W驱动"};
    }
    if (u1003.b09_ErrorZDrive) {
        return {ErrorCode::SC1309, "Z驱动"};
    }
    if (u1003.b10_ErrorRDrive) {
        return {ErrorCode::SC1310, "R驱动"};
    }
    if (u1003.b11_ErrorPDrive) {
        return {ErrorCode::SC1311, "P驱动"};
    }
    if (u1003.b12_ErrorRunTimeOut) {
        return {ErrorCode::SC1312, "动作超时"};
    }
    if (u1003.b13_ErrorZLockedRotor) {
        return {ErrorCode::SC1313, "Z堵转"};
    }
    if (u1003.b14_ErrorRLockedRotor) {
        return {ErrorCode::SC1314, "R堵转"};
    }
    if (u1003.b15_ErrorPLockedRotor) {
        return {ErrorCode::SC1315, "P堵转"};
    }
    if (u1003.b01_ErrorAssembly) {
        return {ErrorCode::SC1301, "部件错误"};
    }
    return {};
}

}  // namespace COM
