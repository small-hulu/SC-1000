#include "CThermostat.h"

#include <QJsonDocument>
#include <QJsonObject>

namespace COM {

QString CThermostat::Params::to_string() const {
    if (false == isValid) {
        return QString("Data is not valid");
    }

    QJsonObject json;
    json["光电组件able"] = photoelectricAble;
    json["光电组件temp"] = photoelectricTemp;
    json["光电组件elec"] = photoelectricElectric;

    json["温裕盘able"] = incubateAble;
    json["温裕盘temp"] = incubateTemp;
    json["温裕盘elec"] = incubateElectric;

    json["试剂仓盖able"] = reagentBinLidAble;
    json["试剂仓盖temp"] = reagentBinLidTemp;
    json["试剂仓盖elec"] = reagentBinLidElectric;

    json["试剂仓able"]           = reagentBinAble;
    json["试剂仓temp"]           = reagentBinTemp;
    json["试剂仓帕尔贴正面temp"] = reagentBinPeltierFrontTemp;
    json["试剂仓帕尔贴背面temp"] = reagentBinPeltierBackTemp;
    json["试剂仓加热电流 PEL1"]  = reagentBinPEL1Electric;
    json["试剂仓加热电流 PEL2"]  = reagentBinPEL2Electric;
    json["试剂仓设备外温度"]     = reagentBinOutElectric;

    json["风扇0able"] = fan0Able;
    json["风扇1able"] = fan1Able;
    json["风扇2able"] = fan2Able;
    json["风扇3able"] = fan3Able;

    return QString::fromUtf8(QJsonDocument(json).toJson(QJsonDocument::Indented));
}

QString CThermostat::StateBits::to_string() const {
    if (false == isValid) {
        return QString("Data is not valid");
    }

    QJsonObject json;

    /// [u1002_光电组件]
    json["u1002_光电组件-00-异常标志"]         = u1002_photoelectric.b00_isError;
    json["u1002_光电组件-01-变温标志"]         = u1002_photoelectric.b01_isRunning;
    json["u1002_光电组件-02-加热制冷状态异常"] = u1002_photoelectric.b02_isChangeError;
    json["u1002_光电组件-03-变温超时"]         = u1002_photoelectric.b03_isChangeTimeOutWaring;
    json["u1002_光电组件-04-超温警告"]         = u1002_photoelectric.b04_isChangeTempOutWaring;
    /// [u1002_光电组件]

    /// [u1007_温裕盘]
    json["u1007_温裕盘-00-异常标志"]         = u1007_thermal.b00_isError;
    json["u1007_温裕盘-01-变温标志"]         = u1007_thermal.b01_isRunning;
    json["u1007_温裕盘-02-加热制冷状态异常"] = u1007_thermal.b02_isChangeError;
    json["u1007_温裕盘-03-变温超时"]         = u1007_thermal.b03_isChangeTimeOutWaring;
    json["u1007_温裕盘-04-超温警告"]         = u1007_thermal.b04_isChangeTempOutWaring;
    /// [u1007_温裕盘]

    /// [u1014_试剂仓盖]
    json["u1014_试剂仓盖-00-异常标志"]         = u1014_reagentBinLid.b00_isError;
    json["u1014_试剂仓盖-01-变温标志"]         = u1014_reagentBinLid.b01_isRunning;
    json["u1014_试剂仓盖-02-加热制冷状态异常"] = u1014_reagentBinLid.b02_isChangeError;
    json["u1014_试剂仓盖-03-变温超时"]         = u1014_reagentBinLid.b03_isChangeTimeOutWaring;
    json["u1014_试剂仓盖-04-超温警告"]         = u1014_reagentBinLid.b04_isChangeTempOutWaring;
    /// [u1014_试剂仓盖]

    /// [u1018_试剂仓]
    json["u1020_试剂仓-00-异常标志"]         = u1020_reagentBin.b00_isError;
    json["u1020_试剂仓-01-变温标志"]         = u1020_reagentBin.b01_isRunning;
    json["u1020_试剂仓-02-加热制冷状态异常"] = u1020_reagentBin.b02_isChangeError;
    json["u1020_试剂仓-03-变温超时"]         = u1020_reagentBin.b03_isChangeTimeOutWaring;
    json["u1020_试剂仓-04-超温警告"]         = u1020_reagentBin.b04_isChangeTempOutWaring;
    /// [u1018_试剂仓]

    return QString::fromUtf8(QJsonDocument(json).toJson(QJsonDocument::Indented));
}

CLteErrorCode::ErrorPkg CThermostat::StateBits::get_errorString() const {
    using ErrorCode = CLteErrorCode::ErrorCode;

    /// u1002_光电组件
    if (u1002_photoelectric.b02_isChangeError) {
        return {ErrorCode::SC16100202,
                "光电组件"
                "加热制冷状态异常"};
    }
    if (u1002_photoelectric.b03_isChangeTimeOutWaring) {
        return {ErrorCode::SC16100203,
                "光电组件"
                "变温超时"};
    }
    if (u1002_photoelectric.b04_isChangeTempOutWaring) {
        return {ErrorCode::SC16100204,
                "光电组件"
                "超温警告"};
    }
    if (u1002_photoelectric.b00_isError) {
        return {ErrorCode::SC16100200,
                "光电组件"
                "异常标志"};
    }

    /// u1007_温裕盘
    if (u1007_thermal.b02_isChangeError) {
        return {ErrorCode::SC16100702,
                "温裕盘"
                "加热制冷状态异常"};
    }
    if (u1007_thermal.b03_isChangeTimeOutWaring) {
        return {ErrorCode::SC16100703,
                "温裕盘"
                "变温超时"};
    }
    if (u1007_thermal.b04_isChangeTempOutWaring) {
        return {ErrorCode::SC16100704,
                "温裕盘"
                "超温警告"};
    }
    if (u1007_thermal.b00_isError) {
        return {ErrorCode::SC16100700,
                "温裕盘"
                "异常标志"};
    }

    /// u1014_试剂仓盖
    if (u1014_reagentBinLid.b02_isChangeError) {
        return {ErrorCode::SC16101402,
                "试剂仓盖"
                "加热制冷状态异常"};
    }
    if (u1014_reagentBinLid.b03_isChangeTimeOutWaring) {
        return {ErrorCode::SC16101403,
                "试剂仓盖"
                "变温超时"};
    }
    if (u1014_reagentBinLid.b04_isChangeTempOutWaring) {
        return {ErrorCode::SC16101404,
                "试剂仓盖"
                "超温警告"};
    }
    if (u1014_reagentBinLid.b00_isError) {
        return {ErrorCode::SC16101400,
                "试剂仓盖"
                "异常标志"};
    }

    /// u1018_试剂仓
    if (u1020_reagentBin.b02_isChangeError) {
        return {ErrorCode::SC16102002,
                "试剂仓"
                "加热制冷状态异常"};
    }
    if (u1020_reagentBin.b03_isChangeTimeOutWaring) {
        return {ErrorCode::SC16102003,
                "试剂仓"
                "变温超时"};
    }
    if (u1020_reagentBin.b04_isChangeTempOutWaring) {
        return {};
        return {ErrorCode::SC16102004,
                "试剂仓"
                "超温警告"};
    }
    if (u1020_reagentBin.b00_isError) {
        return {ErrorCode::SC16102000,
                "试剂仓"
                "异常标志"};
    }

    return {};
}

}  // namespace COM
