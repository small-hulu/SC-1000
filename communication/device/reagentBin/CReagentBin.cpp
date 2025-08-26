#include "CReagentBin.h"

#include <QJsonDocument>
#include <QJsonObject>

namespace COM {
QString CReagentBin::Params::to_string() const {
    if (false == isValid) {
        return QString("Data is not valid");
    }

    QJsonObject json;
    json["position"] = position;
    json["velocity"] = velocity;
    json["LED_R"]    = LED_R;
    json["LED_G"]    = LED_G;
    json["LED_B"]    = LED_B;

    return QString::fromUtf8(QJsonDocument(json).toJson(QJsonDocument::Indented));
}

QString CReagentBin::StateBits::to_string() const {
    if (false == isValid) {
        return QString("Data is not valid");
    }

    QJsonObject json;
    json["u1002-00-转盘运动标志"]     = u1002.b00_run;
    json["u1002-02-是否达到目标位置"] = u1002.b02_isReachPosition;
    json["u1002-03-是否达到目标速度"] = u1002.b03_isReachVelocity;
    json["u1002-06-转盘运动错误"]     = u1002.b06_runningError;
    json["u1002-07-试剂盘保温盖"]     = u1002.b07_hasLid;

    json["u1003-00-自检完成"] = u1003.b00_selfCheck;
    json["u1003-01-部件错误"] = u1003.b01_ErrorAssembly;
    json["u1003-07-归位完成"] = u1003.b07_homing;
    json["u1003-09-x驱动"]    = u1003.b09_ErrorXDrive;
    json["u1003-12-动作超时"] = u1003.b12_ErrorRunTimeOut;
    json["u1003-13-x堵转"]    = u1003.b13_ErrorXLockedRotor;

    return QString::fromUtf8(QJsonDocument(json).toJson(QJsonDocument::Indented));
}

CLteErrorCode::ErrorPkg CReagentBin::StateBits::get_errorString() const {
    using ErrorCode = CLteErrorCode::ErrorCode;
    if (u1003.b09_ErrorXDrive) {
        return {ErrorCode::SC1009, "x驱动"};
    }
    if (u1003.b12_ErrorRunTimeOut) {
        return {ErrorCode::SC1012, "动作超时"};
    }
    if (u1003.b13_ErrorXLockedRotor) {
        return {ErrorCode::SC1013, "x堵转"};
    }
    if (u1003.b01_ErrorAssembly) {
        return {ErrorCode::SC1001, "部件错误"};
    }

    return {};
}
}  // namespace COM
