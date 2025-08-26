#include "CMixing.h"

#include <QJsonDocument>
#include <QJsonObject>

namespace COM {

QString CMixing::Params::to_string() const {
    if (false == isValid) {
        return QString("Data is not valid");
    }

    QJsonObject json;
    json["cupTarPosition"] = cupTarPosition;
    json["cupInPosition"]  = cupInPosition;
    json["cupOutPosition"] = cupOutPosition;

    return QString::fromUtf8(QJsonDocument(json).toJson(QJsonDocument::Indented));
}

QString CMixing::StateBits::to_string() const {
    if (false == isValid) {
        return QString("Data is not valid");
    }

    QJsonObject json;
    json["a-u1000_Mixing"]             = "分杯混匀状态寄存器";
    json["a-u1001_Mixing"]             = "分杯混匀状态寄存器";
    json["a-u1003_LoadMaterial_LMoto"] = "上料状态寄存器";
    json["a-u1005_DivideCup_PMoto"]    = "分杯状态寄存器";
    json["a-u1007_Mix_VMoto"]          = "混匀状态寄存器";

    /// u1000
    json["u1000_Mixing"]                 = "----------";
    json["u1000_Mixing-00-料斗非空"]     = u1000_Mixing.b00_hoppeFull;
    json["u1000_Mixing-01-滑道非空"]     = u1000_Mixing.b01_slideFull;
    json["u1000_Mixing-02-杯孔非空"]     = u1000_Mixing.b02_cupholeFull;
    json["u1000_Mixing-08-废液桶非空"]   = u1000_Mixing.b08_wasteLiquidTankFull;
    /// u1001
    json["u1001_Mixing"]                 = "----------";
    json["u1001_Mixing-00-自检完成标志"] = u1001_Mixing.b00_selfCheck;

    /// u1003
    json["u1003_LoadMaterial_LMoto"]               = "----------";
    json["u1003_LoadMaterial_LMoto-00-动作空闲"]   = u1003_LoadMaterial_LMoto.b00_leisure;
    json["u1003_LoadMaterial_LMoto-08-部件错误"]   = u1003_LoadMaterial_LMoto.b08_ErrorAssembly;
    json["u1003_LoadMaterial_LMoto-13-动作超时"]   = u1003_LoadMaterial_LMoto.b13_ErrorRunTimeOut;
    json["u1003_LoadMaterial_LMoto-14-驱动器错误"] = u1003_LoadMaterial_LMoto.b14_ErrorDrive;
    json["u1003_LoadMaterial_LMoto-15-电机堵转"]   = u1003_LoadMaterial_LMoto.b15_ErrorLockedRotor;

    /// u1005
    json["u1005_DivideCup_PMoto"]               = "----------";
    json["u1005_DivideCup_PMoto-00-动作空闲"]   = u1005_DivideCup_PMoto.b00_leisure;
    json["u1005_DivideCup_PMoto-08-部件错误"]   = u1005_DivideCup_PMoto.b08_ErrorAssembly;
    json["u1005_DivideCup_PMoto-13-动作超时"]   = u1005_DivideCup_PMoto.b13_ErrorRunTimeOut;
    json["u1005_DivideCup_PMoto-14-驱动器错误"] = u1005_DivideCup_PMoto.b14_ErrorDrive;
    json["u1005_DivideCup_PMoto-15-电机堵转"]   = u1005_DivideCup_PMoto.b15_ErrorLockedRotor;

    /// u1007
    json["u1007_Mix_VMoto"]               = "----------";
    json["u1007_Mix_VMoto-00-动作空闲"]   = u1007_Mix_VMoto.b00_leisure;
    json["u1007_Mix_VMoto-08-部件错误"]   = u1007_Mix_VMoto.b08_ErrorAssembly;
    json["u1007_Mix_VMoto-13-动作超时"]   = u1007_Mix_VMoto.b13_ErrorRunTimeOut;
    json["u1007_Mix_VMoto-14-驱动器错误"] = u1007_Mix_VMoto.b14_ErrorDrive;
    json["u1007_Mix_VMoto-15-电机堵转"]   = u1007_Mix_VMoto.b15_ErrorLockedRotor;

    return QString::fromUtf8(QJsonDocument(json).toJson(QJsonDocument::Indented));
}

CLteErrorCode::ErrorPkg CMixing::StateBits::get_errorString() const {
    using ErrorCode = CLteErrorCode::ErrorCode;

    if (u1003_LoadMaterial_LMoto.b13_ErrorRunTimeOut) {
        return {ErrorCode::SC14100313, "LMoto-动作超时"};
    }
    if (u1003_LoadMaterial_LMoto.b14_ErrorDrive) {
        return {ErrorCode::SC14100314, "LMoto-驱动器错误"};
    }
    if (u1003_LoadMaterial_LMoto.b15_ErrorLockedRotor) {
        return {ErrorCode::SC14100315, "LMoto-电机堵转"};
    }

    if (u1005_DivideCup_PMoto.b13_ErrorRunTimeOut) {
        return {ErrorCode::SC14100513, "PMoto-动作超时"};
    }
    if (u1005_DivideCup_PMoto.b14_ErrorDrive) {
        return {ErrorCode::SC14100514, "PMoto-驱动器错误"};
    }
    if (u1005_DivideCup_PMoto.b15_ErrorLockedRotor) {
        return {ErrorCode::SC14100515, "PMoto-电机堵转"};
    }

    if (u1007_Mix_VMoto.b13_ErrorRunTimeOut) {
        return {ErrorCode::SC14100713, "VMoto-动作超时"};
    }
    if (u1007_Mix_VMoto.b14_ErrorDrive) {
        return {ErrorCode::SC14100714, "VMoto-驱动器错误"};
    }
    if (u1007_Mix_VMoto.b15_ErrorLockedRotor) {
        return {ErrorCode::SC14100715, "VMoto-电机堵转"};
    }

    if (u1003_LoadMaterial_LMoto.b08_ErrorAssembly) {
        return {ErrorCode::SC14100308, "LMoto-部件错误"};
    }
    if (u1005_DivideCup_PMoto.b08_ErrorAssembly) {
        return {ErrorCode::SC14100508, "PMoto-部件错误"};
    }
    if (u1007_Mix_VMoto.b08_ErrorAssembly) {
        return {ErrorCode::SC14100708, "VMoto-部件错误"};
    }

    return {};
}

}  // namespace COM
