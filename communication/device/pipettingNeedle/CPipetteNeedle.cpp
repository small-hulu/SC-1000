#include "CPipetteNeedle.h"

#include <QJsonDocument>
#include <QJsonObject>

namespace COM {

QString CPipetteNeedle::Params::to_string() const {
    if (false == isValid) {
        return QString("Data is not valid");
    }

    QJsonObject json;
    json["yPosition"]         = yPosition;
    json["zPosition"]         = zPosition;
    json["suctionZPosition"]  = suctionZPosition;
    json["drainageZPosition"] = drainageZPosition;
    json["suctionCapacity"]   = suctionCapacity;
    json["washYPosition"]     = washYPosition;
    json["washZPosition"]     = washZPosition;
    json["suctionThreshold"]  = suctionThreshold;
    json["descentCache"]      = descentCache;
    json["delayWashTime"]     = delayWashTime;

    return QString::fromUtf8(QJsonDocument(json).toJson(QJsonDocument::Indented));
}

QString CPipetteNeedle::StateBits::to_string() const {
    if (false == isValid) {
        return QString("Data is not valid");
    }

    QJsonObject json;
    json["u1002-00-采样针运动标志"]     = u1002.b00_isrunning;
    json["u1002-01-z轴是否在最高位"]    = u1002.b01_zAxisIsMax;
    json["u1002-02-y轴坐标到达"]        = u1002.b02_yAxisReached;
    json["u1002-03-动作成功"]           = u1002.b03_actionSuccess;
    json["u1002-04-撞针"]               = u1002.b04_crash;
    json["u1002-05-试剂不足-进液异常"]  = u1002.b05_InsufficientReagent_in;
    json["u1002-06-移液针运动错误标志"] = u1002.b06_runError;
    json["u1002-07-缓冲瓶充液超时"]     = u1002.b07_fillBottleTimeout;

    json["u1002-08-模式准备完成"]      = u1002.b08_modeReady;
    json["u1002-10-动作空闲"]          = u1002.b10_isLeisure;
    json["u1002-11-试剂不足-出液异常"] = u1002.b11_InsufficientReagent_out;
    json["u1002-12-拒绝执行"]          = u1002.b12_refuse;
    json["u1002-13-堵针检测"]          = u1002.b13_NeedleBlockError;
    json["u1002-14-缓冲瓶高位传感器"]  = u1002.b14_BufferBottleUpper;
    json["u1002-15-缓冲瓶地位传感器"]  = u1002.b15_BufferBottleLower;

    json["u1003-00-自检完成标志"] = u1003.b00_selfCheck;
    json["u1003-01-部件错误"]     = u1003.b01_ErrorAssembly;
    json["u1003-09-P驱动"]        = u1003.b09_ErrorPDrive;
    json["u1003-10-Y驱动"]        = u1003.b10_ErrorYDrive;
    json["u1003-11-Z驱动"]        = u1003.b11_ErrorZDrive;
    json["u1003-12-动作超时"]     = u1003.b12_ErrorRunTimeOut;
    json["u1003-14-y堵转"]        = u1003.b14_ErrorYLockedRotor;
    json["u1003-15-z堵转"]        = u1003.b15_ErrorZLockedRotor;

    return QString::fromUtf8(QJsonDocument(json).toJson(QJsonDocument::Indented));
}

CLteErrorCode::ErrorPkg CPipetteNeedle::StateBits::get_errorString() const {
    using ErrorCode = CLteErrorCode::ErrorCode;
    if (u1003.b09_ErrorPDrive) {
        return {ErrorCode::SC1109, "P驱动"};
    }
    if (u1003.b10_ErrorYDrive) {
        return {ErrorCode::SC1110, "Y驱动"};
    }
    if (u1003.b11_ErrorZDrive) {
        return {ErrorCode::SC1111, "Z驱动"};
    }
    if (u1003.b12_ErrorRunTimeOut) {
        return {ErrorCode::SC1112, "动作超时"};
    }
    if (u1003.b14_ErrorYLockedRotor) {
        return {ErrorCode::SC1114, "y堵转"};
    }
    if (u1003.b15_ErrorZLockedRotor) {
        return {ErrorCode::SC1115, "z堵转"};
    }
    if (u1003.b01_ErrorAssembly) {
        return {ErrorCode::SC1101, "部件错误"};
    }

    if (u1002.b13_NeedleBlockError) {
        return {ErrorCode::SC1113, "堵针检测"};
    }
    return {};
}

}  // namespace COM
