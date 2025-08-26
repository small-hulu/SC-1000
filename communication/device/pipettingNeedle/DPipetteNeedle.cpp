#include "DPipetteNeedle.h"

#include <QJsonArray>

#include "../../config/CKeyJson.hpp"
#include "../../utility/Helper.h"
#include "../../utility/IniConfig.h"
#include "../../utility/JsonConfig.h"

namespace COM {

DPipetteNeedle::DPipetteNeedle(QObject* parent) : APipetteNeedle(parent) {
    setObjectName(QString("移液针"));

    Reset_cacheFixtureParams();

    /// init fixtable
    {
        m_fixTable[KJ::Wash]   = std::bind(&DPipetteNeedle::GetInfo_to_Wash, this);
        m_fixTable[KJ::Load]   = std::bind(&DPipetteNeedle::GetInfo_to_Load, this);
        m_fixTable[KJ::Dilute] = std::bind(&DPipetteNeedle::GetInfo_to_Dilute, this);
    }
}

void DPipetteNeedle::Reset_cacheFixtureParams() {
    auto params_copy = [](Params& params, const QJsonValue& json) -> void {
        params.isValid           = true;
        params.yPosition         = json[KJ::y].toInt();
        params.zPosition         = json[KJ::z].toInt();
        params.zPosition1        = json[KJ::z1].toInt();
        params.washYPosition     = params.yPosition;
        params.washZPosition     = params.zPosition;
        params.suctionZPosition  = params.zPosition;
        params.drainageZPosition = params.zPosition;
        params.suctionCapacity   = json[KJ::capacity].toInt();
        params.suctionThreshold  = json[KJ::threshold].toInt();
    };

    QJsonValue json = JsonConfig::instance()[KJ::PipettingNeedle];
    params_copy(fix.Wash, json[KJ::Wash]);
    params_copy(fix.Load, json[KJ::Load]);
    params_copy(fix.Dilute, json[KJ::Dilute]);
    auto binList = json[KJ::ReagentBin].toArray();

    fix.ReagentBinList.resize(binList.count());
    for (int i = 0; i < binList.count(); i += 1) {
        params_copy(fix.ReagentBinList[i], binList[i]);
    }
    fix.delayWashTime = json[KJ::fix][KJ::delayTime].toInt();
}

void DPipetteNeedle::Save_fileFixtureParams() {
    auto params_2 = [](const Params& params) -> QJsonObject {
        QJsonObject json;
        json[KJ::y] = params.yPosition;
        json[KJ::z] = params.zPosition;
        return json;
    };

    auto params_4 = [](const Params& params) -> QJsonObject {
        QJsonObject json;
        json[KJ::y]         = params.yPosition;
        json[KJ::z]         = params.zPosition;
        json[KJ::z1]        = params.zPosition1;
        json[KJ::capacity]  = params.suctionCapacity;
        json[KJ::threshold] = params.suctionThreshold;
        return json;
    };

    const QString deviceName = KJ::PipettingNeedle;

    QJsonObject jsonObj    = JsonConfig::instance().Get_json();
    QJsonObject jsonDevice = jsonObj[deviceName].toObject();

    /// [洗针时间]
    QJsonObject jsonSub;
    jsonSub[KJ::delayTime] = fix.delayWashTime;
    jsonDevice[KJ::fix]    = jsonSub;

    /// [各个单一孔位的固定位置]
    jsonDevice[KJ::Dilute] = params_4(fix.Dilute);
    jsonDevice[KJ::Load]   = params_2(fix.Load);
    jsonDevice[KJ::Wash]   = params_2(fix.Wash);

    /// 转盘
    QJsonArray holeArr;
    for (auto&& reagentBinHole : fix.ReagentBinList) {
        holeArr << params_4(reagentBinHole);
    }
    jsonDevice[KJ::ReagentBin] = holeArr;

    jsonObj[deviceName] = jsonDevice;
    JsonConfig::instance().Save(jsonObj);
}

RetCode DPipetteNeedle::Init_muc() {
    return Set_delayWashTime(fix.delayWashTime);
}

RetCode DPipetteNeedle::Set_delayWashTime(int32_t delay) {
    QModbusDataUnit dataUnit = create_modbusData(2, 1028);
    dataUnit.setValue(0, delay);
    dataUnit.setValue(1, delay >> 16u);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }
    return RetCode::Success;
}

DPipetteNeedle::InfoPkg DPipetteNeedle::Get_fixInfo(const QString& name) const {
    if (false) {
    } else if (name.isEmpty()) {
        return {};
    } else if (m_fixTable.count(name)) {
        return m_fixTable[name]();
    } else if (name == KJ::ReagentBin) {
        return {};
    } else {
        assert(false && "移液针table解析异常键值");
        return {};
    }
}

DPipetteNeedle::InfoPkg DPipetteNeedle::GetInfo_to_Load() const {
    InfoPkg info;
    info.params = fix.Load;
    return info;
}

DPipetteNeedle::InfoPkg DPipetteNeedle::GetInfo_to_Dilute() const {
    InfoPkg info;
    info.params = fix.Dilute;
    return info;
}

DPipetteNeedle::InfoPkg DPipetteNeedle::GetInfo_to_Wash() const {
    InfoPkg info;
    info.params = fix.Wash;
    return info;
}

DPipetteNeedle::InfoPkg DPipetteNeedle::GetInfo_to_ReagentBin(int idx) const {
    assert(idx < fix.ReagentBinList.size() && "移液针到试剂仓下标映射错误");
    InfoPkg info;
    info.params = fix.ReagentBinList[idx];
    return info;
}

void DPipetteNeedle::Suction_reagentTrack(IDeviceConfig::Params& params, const int idxBinGroup,
                                          const int idxBinBack) {
    auto&& ini = IniConfig::instance();

    constexpr double step_to_mm                          = 0.00635;
    double           heights[ReagentBinHoleReagentCount] = {4.075379855967925, 3.906793288441674,
                                                            2.80195844566735, 4.0185568};
    heights[0] = ini[KI::pipetteNeedle_cavityAHeightMM].toDouble();
    heights[1] = ini[KI::pipetteNeedle_cavityBHeightMM].toDouble();
    heights[2] = ini[KI::pipetteNeedle_cavityCHeightMM].toDouble();
    heights[3] = ini[KI::pipetteNeedle_cavityDHeightMM].toDouble();
    assert(idxBinBack < (int)std::size(heights));
    int stepCavityCompensation = ini[KI::pipetteNeedle_cavityStepCompensation].toInt();

    /// 液体残留量 微升
    double liquidResidual =
        SharedMemory::instance().Get_ReagentBinHole_liquidResidual(idxBinGroup, idxBinBack);
    liquidResidual        -= motorStep_to_microlitre_PipetteNeedle(params.suctionCapacity);
    liquidResidual         = qMax(0.0, liquidResidual);
    /// 残留微升 -> 高度
    double heightResidual  = heights[idxBinBack] / 1000.0 * liquidResidual;
    /// 步数
    double zStep           = 1.0 / step_to_mm * heightResidual;

    /// 从最底的步数 - 残量的步数
    params.zPosition =
        clamp<int>(params.zPosition - zStep + stepCavityCompensation, 0, params.zPosition);
    params.suctionZPosition  = params.zPosition;
    /// 最终要把数值设置为负数
    params.suctionThreshold *= -1;
}

void DPipetteNeedle::Suction_sampleTrack(IDeviceConfig::Params& params, const int idxBinGroup,
                                         const int idxBinBack) {
    auto hole                = fix.ReagentBinList[idxBinBack];
    params.suctionZPosition  = hole.zPosition1;
    /// 最终要把数值设置为负数
    params.suctionThreshold *= -1;
}

}  // namespace COM
