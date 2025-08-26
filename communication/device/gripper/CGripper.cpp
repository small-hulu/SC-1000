#include "CGripper.h"

#include <QJsonDocument>
#include <QJsonObject>

namespace COM {

QString CGripper::Params::to_string() const {
    if (false == isValid) {
        return QString("Data is not valid");
    }

    QJsonObject json;
    json["xPosition"]   = xPosition;
    json["yPosition"]   = yPosition;
    json["zPosition"]   = zPosition;
    json["deepGrab"]    = deepGrab;
    json["deepRelease"] = deepRelease;
    json["openRange"]   = openRange;
    json["rangeMax"]    = rangeMax;
    json["rangeMin"]    = rangeMin;

    return QString::fromUtf8(QJsonDocument(json).toJson(QJsonDocument::Indented));
}

QString CGripper::StateBits::to_string() const {
    if (false == isValid) {
        return QString("Data is not valid");
    }

    QJsonObject json;
    json["u1002-00-夹爪运动标志"]     = u1002.b00_run;
    json["u1002-01-z轴高位标志"]      = u1002.b01_zAxisIsRuning;
    json["u1002-02-xy位置到达标志"]   = u1002.b02_xyAxisReach;
    json["u1002-03-拒绝执行标志"]     = u1002.b03_refuse;
    json["u1002-04-碰撞标志"]         = u1002.b04_crash;
    json["u1002-05-动作成功标志"]     = u1002.b05_success;
    json["u1002-06-夹爪运动错误标志"] = u1002.b06_runError;
    json["u1002-07-夹取物存在标志"]   = u1002.b07_itemExist;
    json["u1002-08-模式准备完成"]     = u1002.b08_modeReady;
    json["u1002-12-动作空闲"]         = u1002.b12_leisure;

    json["u1003-00-自检完成标志"] = u1003.b00_selfCheck;
    json["u1003-01-部件错误"]     = u1003.b01_ErrorAssembly;
    json["u1003-07-g驱动"]        = u1003.b07_ErrorGDrive;
    json["u1003-08-g堵转"]        = u1003.b08_ErrorGLockedRotor;
    json["u1003-09-x驱动"]        = u1003.b09_ErrorXDrive;
    json["u1003-10-y驱动"]        = u1003.b10_ErrorYDrive;
    json["u1003-11-z驱动"]        = u1003.b11_ErrorZDrive;
    json["u1003-12-动作超时"]     = u1003.b12_ErrorRunTimeOut;
    json["u1003-13-x堵转"]        = u1003.b13_ErrorXLockedRotor;
    json["u1003-14-y堵转"]        = u1003.b14_ErrorYLockedRotor;
    json["u1003-15-z堵转"]        = u1003.b15_ErrorZLockedRotor;

    return QString::fromUtf8(QJsonDocument(json).toJson(QJsonDocument::Indented));
}

CLteErrorCode::ErrorPkg CGripper::StateBits::get_errorString() const {
    using ErrorCode = CLteErrorCode::ErrorCode;
    if (u1003.b07_ErrorGDrive) {
        return {ErrorCode::SC1207, "g驱动"};
    }
    if (u1003.b08_ErrorGLockedRotor) {
        return {ErrorCode::SC1208, "g堵转"};
    }
    if (u1003.b09_ErrorXDrive) {
        return {ErrorCode::SC1209, "x驱动"};
    }
    if (u1003.b10_ErrorYDrive) {
        return {ErrorCode::SC1210, "y驱动"};
    }
    if (u1003.b11_ErrorZDrive) {
        return {ErrorCode::SC1211, "z驱动"};
    }
    if (u1003.b12_ErrorRunTimeOut) {
        return {ErrorCode::SC1212, "动作超时"};
    }
    if (u1003.b13_ErrorXLockedRotor) {
        return {ErrorCode::SC1213, "x堵转"};
    }
    if (u1003.b14_ErrorYLockedRotor) {
        return {ErrorCode::SC1214, "y堵转"};
    }
    if (u1003.b15_ErrorZLockedRotor) {
        return {ErrorCode::SC1215, "z堵转"};
    }
    if (u1003.b01_ErrorAssembly) {
        return {ErrorCode::SC1201, "部件错误"};
    }
    return {};
}

}  // namespace COM
