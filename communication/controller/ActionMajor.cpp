#include "ActionMajor.h"

#include <QJsonDocument>
#include <mutex>

#include "../config/CKeyJson.hpp"
#include "../device/gripper/DGripper.h"
#include "../device/incubate/DIncubate.h"
#include "../device/magneticSeparation/DMagneticSeparation.h"
#include "../device/mixing/DMixing.h"
#include "../device/photoelectric/DPhotoelectric.h"
#include "../device/pipettingNeedle/DPipetteNeedle.h"
#include "../device/reagentBin/DReagentBin.h"
#include "../device/thermostat/DThermostat.h"
#include "../utility/IniConfig.h"
#include "../utility/UnitConv.h"
#include "Parse.h"

namespace COM {

namespace {
constexpr int Try_Again_Cnt = 0;
}

/**
 * 注意：这里要与配置文件进行制表
 */
TaskNode ActionMajor::Gen_TaskType(const QJsonObject& json) {
    static QVector<TaskStepFactory> table;
    {
        static std::once_flag flag;
        std::call_once(flag, []() {
            table.resize(Mode_Count);
            table[Mode_ReadMCUMessage]        = &Major_for_ReadMCUMessage;
            table[Mode_HomingAllDevice]       = &Major_for_HomingAllDevice;
            table[Mode_HomingAllPump]         = &Major_for_HomingAllPump;
            table[Mode_TubleDisCard]          = &Major_for_TubleDisCard;
            table[AbleMagneticSeparationWash] = &Major_for_AbleMagneticSeparationWash;
            table[Mode_AbleASubstrate]        = &Major_for_AbleASubstrate;
            table[Mode_AbleBSubstrate]        = &Major_for_AbleBSubstrate;
            table[Mode_DrainWash]             = &Major_for_DrainWash;
            table[Mode_Sleep]                 = &Major_for_Sleep;
            table[Mode_TODO]                  = &Major_for_TODO;
        });
    }

    const int mode = json[KJ::deviceMode].toInt();
    if (mode >= table.size()) {
        return Parse::parsing_failing(json, __PRETTY_FUNCTION__);
    }
    return table[mode](json);
}

/**
 * 一般来说打到日志中
 * 或者再存到全局的信息对象中，方便ui读出来
 */
TaskNode ActionMajor::Major_for_ReadMCUMessage(const QJsonObject& json) {
    auto get_mucVersion = []() -> QString {
        QJsonObject json;
        auto        record = [&json](auto&& device) {
            json[device.objectName()] = device.Get_mcuVersion();
        };
        record(DGripper::instance());
        record(DMagneticSeparation::instance());
        record(DMixing::instance());
        record(DPhotoelectric::instance());
        record(DReagentBin::instance());
        record(DPipetteNeedle::instance());
        record(DThermostat::instance());

        return QString::fromUtf8(QJsonDocument(json).toJson(QJsonDocument::Indented));
    };

    auto task = [get_mucVersion](Tube::entity_tube) {
        qInfo().noquote().nospace() << get_mucVersion();
        return TaskNode::return_type::Success;
    };
    return {task, json};
}

/**
 * 所有部件的归位
 * 注意，每次exec是使用 parallel 还是 sequenced
 * 部件homing默认success（目前）
 * 如果部件连homing都做不到，那么这个机器别跑了
 * ======================================
 * TODO
 * 暂时先把需要放管homing的注释
 */
TaskNode ActionMajor::Major_for_HomingAllDevice(const QJsonObject& json) {
    Q_UNUSED(json);

    auto task = [](Tube::entity_tube) {
        /// 移液针
        DPipetteNeedle::instance().Set_Config(DPipetteNeedle::Mode_Homing, {});
        DPipetteNeedle::instance().Exec_sequencedTask();

        /// 夹爪
        DGripper::instance().Set_Config(DGripper::Mode_Homing, {});
        DGripper::instance().Exec_sequencedTask();

        /// 试剂仓
        DReagentBin::instance().Set_Config(DReagentBin::Mode_Homing, {});
        DReagentBin::instance().Exec_sequencedTask();

        /// 光电位
        DPhotoelectric::instance().Set_Config(DPhotoelectric::Mode_HomingDoor, {});
        DPhotoelectric::instance().Exec_sequencedTask();

        /// 磁分离
        /// + 转盘
        /// + 清洗池
        /// + 底物A
        DMagneticSeparation::instance().Set_Config(DMagneticSeparation::Mode_HomingZ, {});
        DMagneticSeparation::instance().Exec_sequencedTask();
        DMagneticSeparation::instance().Set_Config(DMagneticSeparation::Mode_HomingTray, {});
        DMagneticSeparation::instance().Exec_sequencedTask();

        /// 混匀板
        /// + 混匀电机
        /// + 上杯管
        /// + 分杯位
        DMixing::instance().Set_Config(DMixing::Mode_HomingMix, {});
        DMixing::instance().Exec_sequencedTask();
        DMixing::instance().Set_Config(DMixing::Mode_HomingLoadMaterial, {});
        DMixing::instance().Exec_sequencedTask();
        DMixing::instance().Set_Config(DMixing::Mode_HomingDivideCup, {});
        DMixing::instance().Exec_sequencedTask();
        DMixing::instance().Set_Config(DMixing::Mode_EnableDivideCup, {});
        DMixing::instance().Exec_sequencedTask();

        /// 温控和风扇
        DThermostat::instance();

        /// 移液针洗针
        DPipetteNeedle::instance().Set_Config(DPipetteNeedle::Mode_Wash,
                                              DPipetteNeedle::instance().GetInfo_to_Wash().params);
        DPipetteNeedle::instance().Exec_sequencedTask();
        DPipetteNeedle::instance().Set_Config(DPipetteNeedle::Mode_Homing, {});
        DPipetteNeedle::instance().Exec_sequencedTask();

        return TaskNode::return_type::Success;
    };
    return {task, json};
}

/**
 * 由于只是泵归位，不是把液路排空
 * 因此只需要一个管即可
 */
TaskNode ActionMajor::Major_for_HomingAllPump(const QJsonObject& json) {
    auto task = [](Tube::entity_tube) -> TaskNode::return_type {
        //! 移液针先避让
        DPipetteNeedle::instance().Set_Config(DPipetteNeedle::Mode_Homing, {});
        DPipetteNeedle::instance().Exec_sequencedTask();

        if (DMixing::instance().check_cup() == false) {
            return RetCode::CupError;
        }

        //! 磁分离
        auto&                       magneticSeparation = DMagneticSeparation::instance();
        DMagneticSeparation::Params magParams;
        magneticSeparation.Set_Config(DMagneticSeparation::Mode_HomingTray, magParams);
        magneticSeparation.Exec_sequencedTask();

        /// 磁分离-接待夹爪
        magParams = magneticSeparation.GetParams_to_Grapper();
        magneticSeparation.Set_Config(DMagneticSeparation::Mode_AbleTrayMove, magParams);
        magneticSeparation.Exec_sequencedTask();

        //! 夹爪
        auto&            gripper = DGripper::instance();
        DGripper::Params griParams;
        gripper.Set_Config(DGripper::Mode_Homing, griParams);
        gripper.Exec_sequencedTask();

        /// 夹爪去分杯位，拿一个空管
        griParams = gripper.GetInfo_to_Cup().params;
        gripper.Set_Config(DGripper::Mode_MoveGrab, griParams);
        gripper.Exec_sequencedTask();

        /// 夹爪移动到磁分离，并放管
        griParams = gripper.GetInfo_to_MagneticSeparation().params;
        gripper.Set_Config(DGripper::Mode_MoveRelease, griParams);
        gripper.Exec_sequencedTask();

        /// 磁分离-底物A
        magParams = magneticSeparation.GetParams_to_ASubstrate();
        magneticSeparation.Set_Config(DMagneticSeparation::Mode_AbleTrayMove, magParams);
        magneticSeparation.Exec_sequencedTask();
        magneticSeparation.Set_Config(DMagneticSeparation::Mode_HomingASubstratePump, magParams);
        magneticSeparation.Exec_sequencedTask();

        /// 磁分离-清洗泵
        magParams = magneticSeparation.GetParams_to_Wash();
        magneticSeparation.Set_Config(DMagneticSeparation::Mode_AbleTrayMove, magParams);
        magneticSeparation.Exec_sequencedTask();
        magneticSeparation.Set_Config(DMagneticSeparation::Mode_HomingWashPump, magParams);
        magneticSeparation.Exec_sequencedTask();

        /// 转动磁分离模块到和夹爪配合的位置
        magParams = magneticSeparation.GetParams_to_Grapper();
        magneticSeparation.Set_Config(DMagneticSeparation::Mode_AbleTrayMove, magParams);
        magneticSeparation.Exec_sequencedTask();

        /// 夹爪拿管（还在磁分离的上空）
        griParams = gripper.GetInfo_to_MagneticSeparation().params;
        gripper.Set_Config(DGripper::Mode_MoveGrab, griParams);
        gripper.Exec_sequencedTask();

        //! 光电仓
        auto&                  photoelectric = DPhotoelectric::instance();
        DPhotoelectric::Params phoParams{};
        photoelectric.Set_Config(DPhotoelectric::Mode_HomingDoor, phoParams);
        photoelectric.Exec_sequencedTask();

        /// 光电位打开
        photoelectric.Set_Config(DPhotoelectric::Mode_DoorOpen, phoParams);
        photoelectric.Exec_sequencedTask();

        /// 夹爪释放
        /// 夹爪移动到光电位
        griParams = gripper.GetInfo_to_Photoelectric().params;
        gripper.Set_Config(DGripper::Mode_MoveRelease, griParams);
        gripper.Exec_sequencedTask();

        /// 光电的底物B泵，归位
        photoelectric.Set_Config(DPhotoelectric::Mode_HomingPump, phoParams);
        photoelectric.Exec_sequencedTask();
        photoelectric.Set_Config(DPhotoelectric::Mode_DoorOpen, phoParams);
        photoelectric.Exec_sequencedTask();

        /// 夹爪取走管
        gripper.Set_Config(DGripper::Mode_MoveGrab, griParams);
        gripper.Exec_sequencedTask();

        /// 光电关门
        photoelectric.Set_Config(DPhotoelectric::Mode_DoorClose, phoParams);
        photoelectric.Exec_sequencedTask();

        Gripper_delayWashTime_Guard gurad;
        griParams = gripper.GetInfo_to_Throw().params;
        gripper.Set_Config(DGripper::Mode_MoveRelease, griParams);
        gripper.Exec_sequencedTask();
        gripper.Set_Config(DGripper::Mode_Homing, griParams);
        gripper.Exec_sequencedTask();

        return TaskNode::return_type::Success;
    };

    return {task, json};
}

/**
 * >>> 不分先后
 * >>> 请保证各个部件已经homing了
 * >>>
 * + 分杯位（不用管）
 * + 温裕盘
 *  - 加样位
 *  - 温裕位（暂不考虑）
 * + 混匀位
 * + 光电位
 * + 磁分离
 *  - 暂时只考虑一个位置
 */
TaskNode ActionMajor::Major_for_TubleDisCard(const QJsonObject& json) {
    Q_UNUSED(json);

    //! 到抛料位，并抛料
    auto check_to_throw = [](auto params) -> void {
        auto& gripper = DGripper::instance();
        gripper.Set_Config(DGripper::Mode_MoveGrab, params);
        gripper.Exec_sequencedTask();

        auto state = gripper.Get_mcuStateBits();
        if (0 == state.u1002.b07_itemExist) {
            return;
        }
        if (1 == state.u1002.b04_crash) {
            gripper.Set_Config(DGripper::Mode_Homing, {});
            gripper.Exec_sequencedTask();
            return;
        }
        if (1 == state.u1003.b01_ErrorAssembly) {
            gripper.Set_Config(DGripper::Mode_RecoveryError, {});
            gripper.Exec_sequencedTask();
            gripper.Set_Config(DGripper::Mode_Homing, {});
            gripper.Exec_sequencedTask();
            return;
        }
        params = gripper.GetInfo_to_Throw().params;
        gripper.Set_Config(DGripper::Mode_MoveRelease, params);
        gripper.Exec_sequencedTask();
    };

    auto task = [check_to_throw](Tube::entity_tube) {
        DPipetteNeedle::instance().Set_Config(DPipetteNeedle::Mode_Homing, {});
        DPipetteNeedle::instance().Exec_parallelTask();

        Gripper_delayWashTime_Guard gurad;
        auto&                       gripper = DGripper::instance();
        DGripper::Params            gparams{};

        /// 先homg归位
        gripper.Set_Config(DGripper::Mode_Homing, {});
        gripper.Exec_sequencedTask();

        /// 检查抛料位，本身是否有管
        gparams = gripper.GetInfo_to_Throw().params;
        check_to_throw(gparams);

        /// 检查光电位置
        auto& photoelectric = DPhotoelectric::instance();
        photoelectric.Set_Config(DPhotoelectric::Mode_DoorOpen, {});
        photoelectric.Exec_sequencedTask();
        gparams = gripper.GetInfo_to_Photoelectric().params;
        check_to_throw(gparams);
        photoelectric.Set_Config(DPhotoelectric::Mode_DoorClose, {});
        photoelectric.Exec_sequencedTask();

        /// 检查混匀位
        /// TODO
        /// 后续考虑每个孔位
        /// TODO
        gparams = gripper.GetInfo_to_Mixing().params;
        check_to_throw(gparams);

        /// 检查加样位置
        gparams = gripper.GetInfo_to_Load().params;
        check_to_throw(gparams);

        /// 检查稀释位
        gparams = gripper.GetInfo_to_Dilute().params;
        check_to_throw(gparams);

        /// 检查磁分离
        gparams                  = gripper.GetInfo_to_MagneticSeparation().params;
        auto& magneticSeparation = DMagneticSeparation::instance();
        magneticSeparation.Set_Config(DMagneticSeparation::Mode_HomingTray, {});
        magneticSeparation.Exec_sequencedTask();
        for (int i = 0; i < DMagneticSeparation::MCell_Count; i += 1) {
            auto mparams = magneticSeparation.GetParams_to_Grapper();
            mparams      = magneticSeparation.Offset_byIndex(mparams, i);
            magneticSeparation.Set_Config(DMagneticSeparation::Mode_AbleTrayMove, mparams);
            magneticSeparation.Exec_sequencedTask();
            check_to_throw(gparams);
        }
        /// 全部结束后再homing
        gripper.Set_Config(DGripper::Mode_Homing, {});
        gripper.Exec_sequencedTask();

        /// 孵育盘
        auto incTable = DIncubate::instance().Get_CU().Get_allPoint();
        for (auto pii : incTable) {
            gparams = gripper.GetInfo_to_Incubate_byXY(pii.first, pii.second).params;
            check_to_throw(gparams);
        }
        /// 全部结束后再homing
        gripper.Set_Config(DGripper::Mode_Homing, {});
        gripper.Exec_sequencedTask();

        return TaskNode::return_type::Success;
    };

    return {task, json};
}

TaskNode ActionMajor::Major_for_AbleMagneticSeparationWash(const QJsonObject& json) {
    auto check = [](RetCode& ret) -> RetCode {
        auto& gripper = DGripper::instance();
        for (int n = Try_Again_Cnt; n > 0 && ret != RetCode::Success; n += -1) {
            auto oMode   = gripper.get_currentMode();
            auto oParams = gripper.Get_mcuParams();
            gripper.Set_Config(DGripper::Mode_Homing, oParams);
            gripper.Exec_sequencedTask();
            gripper.Set_Config(DGripper::Mode_XYPosition, oParams);
            gripper.Exec_sequencedTask();
            gripper.Set_Config(oMode, oParams);
            ret = gripper.Exec_sequencedTask();
        }
        return ret;
    };

    auto task = [=](Tube::entity_tube) -> RetCode {
        //! 移液针先避让
        DPipetteNeedle::instance().Set_Config(DPipetteNeedle::Mode_Homing, {});
        DPipetteNeedle::instance().Exec_sequencedTask();

        Gripper_delayWashTime_Guard gurad;
        constexpr int               Try_Cnt = 2;
        int                         defaultCapacity =
            IniConfig::instance()[KI::washMagneticSeparation_floodQuantity].toInt();
        defaultCapacity = microlitre_to_motorStep_ASubstrate(defaultCapacity);

        RetCode ret;
        for (int tryCnt = 0; tryCnt < Try_Cnt; tryCnt += 1) {
            //! 夹爪
            auto&            gripper = DGripper::instance();
            DGripper::Params griParams;
            gripper.Set_Config(DGripper::Mode_Homing, griParams);
            gripper.Exec_sequencedTask();

            if (DMixing::instance().check_cup() == false) {
                continue;
            }

            /// 从上料位取管
            griParams = gripper.GetInfo_to_Cup().params;
            gripper.Set_Config(DGripper::Mode_MoveGrab, griParams);
            ret = gripper.Exec_sequencedTask();
            if (check(ret) != RetCode::Success) {
                return ret;
            }

            //! 磁分离
            auto&                       magneticSeparation = DMagneticSeparation::instance();
            DMagneticSeparation::Params magParams;
            magneticSeparation.Set_Config(DMagneticSeparation::Mode_HomingTray, magParams);
            magneticSeparation.Exec_sequencedTask();

            /// 磁分离-接待夹爪
            magParams = magneticSeparation.GetParams_to_Grapper();
            magneticSeparation.Set_Config(DMagneticSeparation::Mode_AbleTrayMove, magParams);
            magneticSeparation.Exec_sequencedTask();

            griParams = gripper.GetInfo_to_MagneticSeparation().params;
            gripper.Set_Config(DGripper::Mode_MoveRelease, griParams);
            ret = gripper.Exec_sequencedTask();
            if (check(ret) != RetCode::Success) {
                return ret;
            }

            magParams = magneticSeparation.GetParams_to_Wash();
            magneticSeparation.Set_Config(DMagneticSeparation::Mode_AbleTrayMove, magParams);
            magneticSeparation.Exec_sequencedTask();
            magParams.washCapacity = defaultCapacity;
            magneticSeparation.Set_Config(DMagneticSeparation::Mode_AbleWash, magParams);
            magneticSeparation.Exec_sequencedTask();

            magParams = magneticSeparation.GetParams_to_Grapper();
            magneticSeparation.Set_Config(DMagneticSeparation::Mode_AbleTrayMove, magParams);
            magneticSeparation.Exec_sequencedTask();

            /// 夹爪取管
            griParams = gripper.GetInfo_to_MagneticSeparation().params;
            gripper.Set_Config(DGripper::Mode_MoveGrab, griParams);
            ret = gripper.Exec_sequencedTask();
            if (check(ret) != RetCode::Success) {
                return ret;
            }

            /// 夹爪仍管
            griParams = gripper.GetInfo_to_Throw().params;
            gripper.Set_Config(DGripper::Mode_MoveRelease, griParams);
            ret = gripper.Exec_sequencedTask();
            if (check(ret) != RetCode::Success) {
                return ret;
            }

            gripper.Set_Config(DGripper::Mode_Homing, {});
            gripper.Exec_sequencedTask();
        }

        return TaskNode::return_type::Success;
    };
    return {task, json};
}

TaskNode ActionMajor::Major_for_AbleASubstrate(const QJsonObject& json) {
    auto check = [](RetCode& ret) -> RetCode {
        auto& gripper = DGripper::instance();
        for (int n = Try_Again_Cnt; n > 0 && ret != RetCode::Success; n += -1) {
            auto oMode   = gripper.get_currentMode();
            auto oParams = gripper.Get_mcuParams();
            gripper.Set_Config(DGripper::Mode_Homing, oParams);
            gripper.Exec_sequencedTask();
            gripper.Set_Config(DGripper::Mode_XYPosition, oParams);
            gripper.Exec_sequencedTask();
            gripper.Set_Config(oMode, oParams);
            ret = gripper.Exec_sequencedTask();
        }
        return ret;
    };

    auto task = [=](Tube::entity_tube) -> RetCode {
        //! 移液针先避让
        DPipetteNeedle::instance().Set_Config(DPipetteNeedle::Mode_Homing, {});
        DPipetteNeedle::instance().Exec_sequencedTask();

        Gripper_delayWashTime_Guard gurad;
        constexpr int               Try_Cnt  = 2;
        constexpr int               Able_Cnt = 3;
        int defaultCapacity = IniConfig::instance()[KI::ASubstrate_floodQuantity].toInt();
        defaultCapacity     = microlitre_to_motorStep_ASubstrate(defaultCapacity);

        RetCode ret;
        for (int tryCnt = 0; tryCnt < Try_Cnt; tryCnt += 1) {
            //! 夹爪
            auto&            gripper = DGripper::instance();
            DGripper::Params griParams;
            gripper.Set_Config(DGripper::Mode_Homing, griParams);
            gripper.Exec_sequencedTask();

            if (DMixing::instance().check_cup() == false) {
                continue;
            }

            /// 从上料位取管
            griParams = gripper.GetInfo_to_Cup().params;
            gripper.Set_Config(DGripper::Mode_MoveGrab, griParams);
            ret = gripper.Exec_sequencedTask();
            if (check(ret) != RetCode::Success) {
                return ret;
            }

            //! 磁分离
            auto&                       magneticSeparation = DMagneticSeparation::instance();
            DMagneticSeparation::Params magParams;
            magneticSeparation.Set_Config(DMagneticSeparation::Mode_HomingTray, magParams);
            magneticSeparation.Exec_sequencedTask();

            /// 磁分离-接待夹爪
            magParams = magneticSeparation.GetParams_to_Grapper();
            magneticSeparation.Set_Config(DMagneticSeparation::Mode_AbleTrayMove, magParams);
            magneticSeparation.Exec_sequencedTask();

            griParams = gripper.GetInfo_to_MagneticSeparation().params;
            gripper.Set_Config(DGripper::Mode_MoveRelease, griParams);
            ret = gripper.Exec_sequencedTask();
            if (check(ret) != RetCode::Success) {
                return ret;
            }

            magParams = magneticSeparation.GetParams_to_ASubstrate();
            magneticSeparation.Set_Config(DMagneticSeparation::Mode_AbleTrayMove, magParams);
            magneticSeparation.Exec_sequencedTask();
            for (int ableCnt = 0; ableCnt < Able_Cnt; ableCnt += 1) {
                magParams.aSubstrateCapacity = defaultCapacity;
                magneticSeparation.Set_Config(DMagneticSeparation::Mode_AbleASubstratePump,
                                              magParams);
                magneticSeparation.Exec_sequencedTask();
            }

            magParams = magneticSeparation.GetParams_to_Grapper();
            magneticSeparation.Set_Config(DMagneticSeparation::Mode_AbleTrayMove, magParams);
            magneticSeparation.Exec_sequencedTask();

            /// 夹爪取管
            griParams = gripper.GetInfo_to_MagneticSeparation().params;
            gripper.Set_Config(DGripper::Mode_MoveGrab, griParams);
            ret = gripper.Exec_sequencedTask();
            if (check(ret) != RetCode::Success) {
                return ret;
            }

            /// 夹爪仍管
            griParams = gripper.GetInfo_to_Throw().params;
            gripper.Set_Config(DGripper::Mode_MoveRelease, griParams);
            ret = gripper.Exec_sequencedTask();
            if (check(ret) != RetCode::Success) {
                return ret;
            }

            gripper.Set_Config(DGripper::Mode_Homing, {});
            gripper.Exec_sequencedTask();
        }

        return TaskNode::return_type::Success;
    };
    return {task, json};
}

TaskNode ActionMajor::Major_for_AbleBSubstrate(const QJsonObject& json) {
    auto check = [](RetCode& ret) -> RetCode {
        auto& gripper = DGripper::instance();
        for (int n = Try_Again_Cnt; n > 0 && ret != RetCode::Success; n += -1) {
            auto oMode   = gripper.get_currentMode();
            auto oParams = gripper.Get_mcuParams();
            gripper.Set_Config(DGripper::Mode_Homing, oParams);
            gripper.Exec_sequencedTask();
            gripper.Set_Config(DGripper::Mode_XYPosition, oParams);
            gripper.Exec_sequencedTask();
            gripper.Set_Config(oMode, oParams);
            ret = gripper.Exec_sequencedTask();
        }
        return ret;
    };

    auto task = [=](Tube::entity_tube) -> RetCode {
        //! 移液针先避让
        DPipetteNeedle::instance().Set_Config(DPipetteNeedle::Mode_Homing, {});
        DPipetteNeedle::instance().Exec_sequencedTask();

        Gripper_delayWashTime_Guard gurad;
        constexpr int               Try_Cnt  = 4;
        constexpr int               Able_Cnt = 2;
        int defaultCapacity = IniConfig::instance()[KI::BSubstrate_floodQuantity].toInt();
        defaultCapacity     = microlitre_to_motorStep_BSubstrate(defaultCapacity);

        RetCode ret;
        for (int tryCnt = 0; tryCnt < Try_Cnt; tryCnt += 1) {
            //! 夹爪
            auto&            gripper = DGripper::instance();
            DGripper::Params griParams;
            gripper.Set_Config(DGripper::Mode_Homing, griParams);
            gripper.Exec_sequencedTask();

            if (DMixing::instance().check_cup() == false) {
                continue;
            }

            /// 从上料位取管
            griParams = gripper.GetInfo_to_Cup().params;
            gripper.Set_Config(DGripper::Mode_MoveGrab, griParams);
            ret = gripper.Exec_sequencedTask();
            if (check(ret) != RetCode::Success) {
                return ret;
            }

            auto&                  pho = DPhotoelectric::instance();
            DPhotoelectric::Params phoParams;
            pho.Set_Config(DPhotoelectric::Mode_HomingDoor, phoParams);
            pho.Exec_sequencedTask();
            pho.Set_Config(DPhotoelectric::Mode_DoorOpen, phoParams);
            pho.Exec_sequencedTask();

            griParams = gripper.GetInfo_to_Photoelectric().params;
            gripper.Set_Config(DGripper::Mode_MoveRelease, griParams);
            ret = gripper.Exec_sequencedTask();
            if (check(ret) != RetCode::Success) {
                return ret;
            }

            pho.Set_Config(DPhotoelectric::Mode_DoorClose, phoParams);
            pho.Exec_sequencedTask();
            for (int ableCnt = 0; ableCnt < Able_Cnt; ableCnt += 1) {
                phoParams.capacity = defaultCapacity;
                pho.Set_Config(DPhotoelectric::Mode_OnlyInjection, phoParams);
                pho.Exec_sequencedTask();
            }
            pho.Set_Config(DPhotoelectric::Mode_DoorOpen, phoParams);
            pho.Exec_sequencedTask();

            /// 夹爪取管
            griParams = gripper.GetInfo_to_Photoelectric().params;
            gripper.Set_Config(DGripper::Mode_MoveGrab, griParams);
            ret = gripper.Exec_sequencedTask();
            if (check(ret) != RetCode::Success) {
                return ret;
            }

            pho.Set_Config(DPhotoelectric::Mode_DoorClose, phoParams);
            pho.Exec_sequencedTask();

            /// 夹爪仍管
            griParams = gripper.GetInfo_to_Throw().params;
            gripper.Set_Config(DGripper::Mode_MoveRelease, griParams);
            ret = gripper.Exec_sequencedTask();
            if (check(ret) != RetCode::Success) {
                return ret;
            }

            gripper.Set_Config(DGripper::Mode_Homing, {});
            gripper.Exec_sequencedTask();
        }

        return TaskNode::return_type::Success;
    };
    return {task, json};
}

TaskNode ActionMajor::Major_for_DrainWash(const QJsonObject& json) {
    auto task = [json](Tube::entity_tube) {
        using Device          = DPipetteNeedle;
        auto&&         device = Device::instance();
        Device::Params params;

        device.Set_Config(Device::Mode_Homing, params);
        device.Exec_sequencedTask();

        PipetteNeedle_delayWashTime_Guard duard(
            IniConfig::instance()[KI::pipetteNeedle_DrainWashTime].toInt());

        params = device.GetInfo_to_Wash().params;
        device.Set_Config(Device::Mode_Wash, params);
        device.Exec_sequencedTask();

        device.Set_Config(Device::Mode_Homing, params);
        device.Exec_sequencedTask();
        return TaskNode::return_type::Success;
    };
    return {task, json};
}

TaskNode ActionMajor::Major_for_IntensiveWash(const QJsonObject& json) {
    auto check = [](RetCode& ret) -> RetCode {
        auto& gripper = DGripper::instance();
        for (int n = Try_Again_Cnt; n > 0 && ret != RetCode::Success; n += -1) {
            auto oMode   = gripper.get_currentMode();
            auto oParams = gripper.Get_mcuParams();
            gripper.Set_Config(DGripper::Mode_Homing, oParams);
            gripper.Exec_sequencedTask();
            gripper.Set_Config(DGripper::Mode_XYPosition, oParams);
            gripper.Exec_sequencedTask();
            gripper.Set_Config(oMode, oParams);
            ret = gripper.Exec_sequencedTask();
        }
        return ret;
    };

    auto task = [=](Tube::entity_tube) -> RetCode {
        //! 移液针清洗
        {
            auto&&                 device = DPipetteNeedle::instance();
            DPipetteNeedle::Params param  = device.GetInfo_to_Wash().params;
            device.Set_Config(DPipetteNeedle::Mode_Homing, param);
            device.Exec_sequencedTask();
            device.Set_Config(DPipetteNeedle::Mode_YPosition, param);
            device.Exec_sequencedTask();
            device.Set_Config(DPipetteNeedle::Mode_Wash, param);
            device.Exec_sequencedTask();
            device.Set_Config(DPipetteNeedle::Mode_Homing, param);
            device.Exec_sequencedTask();
        }

        Gripper_delayWashTime_Guard gurad;
        int                         defaultCapacity =
            IniConfig::instance()[KI::washMagneticSeparation_floodQuantity].toInt();
        defaultCapacity = microlitre_to_motorStep_ASubstrate(defaultCapacity);

        RetCode ret;
        {
            //! 夹爪
            auto&            gripper = DGripper::instance();
            DGripper::Params griParams;
            gripper.Set_Config(DGripper::Mode_Homing, griParams);
            gripper.Exec_sequencedTask();

            if (DMixing::instance().check_cup() == false) {
                return TaskNode::return_type::ItemExistError;
            }

            /// 从上料位取管
            griParams = gripper.GetInfo_to_Cup().params;
            gripper.Set_Config(DGripper::Mode_MoveGrab, griParams);
            ret = gripper.Exec_sequencedTask();
            if (check(ret) != RetCode::Success) {
                return ret;
            }

            //! 磁分离
            auto&                       magneticSeparation = DMagneticSeparation::instance();
            DMagneticSeparation::Params magParams;
            magneticSeparation.Set_Config(DMagneticSeparation::Mode_HomingTray, magParams);
            magneticSeparation.Exec_sequencedTask();

            /// 磁分离-接待夹爪
            magParams = magneticSeparation.GetParams_to_Grapper();
            magneticSeparation.Set_Config(DMagneticSeparation::Mode_AbleTrayMove, magParams);
            magneticSeparation.Exec_sequencedTask();

            griParams = gripper.GetInfo_to_MagneticSeparation().params;
            gripper.Set_Config(DGripper::Mode_MoveRelease, griParams);
            ret = gripper.Exec_sequencedTask();
            if (check(ret) != RetCode::Success) {
                return ret;
            }

            magParams = magneticSeparation.GetParams_to_Wash();
            magneticSeparation.Set_Config(DMagneticSeparation::Mode_AbleTrayMove, magParams);
            magneticSeparation.Exec_sequencedTask();
            magParams.washCapacity = defaultCapacity;
            magneticSeparation.Set_Config(DMagneticSeparation::Mode_AbleWash, magParams);
            magneticSeparation.Exec_sequencedTask();

            magParams = magneticSeparation.GetParams_to_Needle0();
            magneticSeparation.Set_Config(DMagneticSeparation::Mode_AbleTrayMove, magParams);
            magneticSeparation.Exec_sequencedTask();
            magneticSeparation.Set_Config(DMagneticSeparation::Mode_AbleDrainNeedle, magParams);
            magneticSeparation.Exec_sequencedTask();

            magParams = magneticSeparation.GetParams_to_Wash();
            magneticSeparation.Set_Config(DMagneticSeparation::Mode_AbleTrayMove, magParams);
            magneticSeparation.Exec_sequencedTask();
            magParams.washCapacity = defaultCapacity;
            magneticSeparation.Set_Config(DMagneticSeparation::Mode_AbleWash, magParams);
            magneticSeparation.Exec_sequencedTask();

            magParams = magneticSeparation.GetParams_to_Needle1();
            magneticSeparation.Set_Config(DMagneticSeparation::Mode_AbleTrayMove, magParams);
            magneticSeparation.Exec_sequencedTask();
            magneticSeparation.Set_Config(DMagneticSeparation::Mode_AbleDrainNeedle, magParams);
            magneticSeparation.Exec_sequencedTask();

            magParams = magneticSeparation.GetParams_to_Wash();
            magneticSeparation.Set_Config(DMagneticSeparation::Mode_AbleTrayMove, magParams);
            magneticSeparation.Exec_sequencedTask();
            magParams.washCapacity = defaultCapacity;
            magneticSeparation.Set_Config(DMagneticSeparation::Mode_AbleWash, magParams);
            magneticSeparation.Exec_sequencedTask();

            magParams = magneticSeparation.GetParams_to_Needle2();
            magneticSeparation.Set_Config(DMagneticSeparation::Mode_AbleTrayMove, magParams);
            magneticSeparation.Exec_sequencedTask();
            magneticSeparation.Set_Config(DMagneticSeparation::Mode_AbleDrainNeedle, magParams);
            magneticSeparation.Exec_sequencedTask();

            magParams = magneticSeparation.GetParams_to_Grapper();
            magneticSeparation.Set_Config(DMagneticSeparation::Mode_AbleTrayMove, magParams);
            magneticSeparation.Exec_sequencedTask();

            /// 夹爪取管
            griParams = gripper.GetInfo_to_MagneticSeparation().params;
            gripper.Set_Config(DGripper::Mode_MoveGrab, griParams);
            ret = gripper.Exec_sequencedTask();
            if (check(ret) != RetCode::Success) {
                return ret;
            }

            /// 夹爪仍管
            griParams = gripper.GetInfo_to_Throw().params;
            gripper.Set_Config(DGripper::Mode_MoveRelease, griParams);
            ret = gripper.Exec_sequencedTask();
            if (check(ret) != RetCode::Success) {
                return ret;
            }

            gripper.Set_Config(DGripper::Mode_Homing, {});
            gripper.Exec_sequencedTask();
        }
        return TaskNode::return_type::Success;
    };

    return {task, json};
}

/**
 * 纯睡觉
 */
TaskNode ActionMajor::Major_for_Sleep(const QJsonObject& json) {
    auto task = [json](Tube::entity_tube) {
        QJsonObject op  = json[KJ::op].toObject();
        const int   ms  = op[KJ::sleepTime].toInt();
        QString     msg = QString("SleepStartTime[%1] SleepFor[%2]ms")
                          .arg(QDateTime::currentDateTime().toString())
                          .arg(ms);
        qDebug() << msg;

        QThread::msleep(ms);
        return TaskNode::return_type::Success;
    };
    return {task, json};
}

/**
 * 用于紧急添加临时功能的函数
 */
TaskNode ActionMajor::Major_for_TODO(const QJsonObject& json) {
    auto task = [json](Tube::entity_tube) {
        qDebug() << __PRETTY_FUNCTION__;
        return TaskNode::return_type::Success;
    };
    return {task, json};
}

}  // namespace COM
