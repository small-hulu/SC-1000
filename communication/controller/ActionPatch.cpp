#include "ActionPatch.h"

#include "../config/CKeyJson.hpp"
#include "../config/SharedMemory.h"
#include "../device/gripper/DGripper.h"
#include "../device/magneticSeparation/DMagneticSeparation.h"
#include "../device/mixing/DMixing.h"
#include "../device/photoelectric/DPhotoelectric.h"
#include "../device/pipettingNeedle/DPipetteNeedle.h"
#include "../device/reagentBin/DReagentBin.h"
#include "../device/thermostat/DThermostat.h"
#include "Parse.h"
#include "log/LOG.h"

namespace COM {

/**
 * Object
 * 线程函数执行策略
 */
static RetCode (IDeviceThread::*execTaskFun)() = &IDeviceThread::Exec_sequencedTask;

/**
 * 函数地址映射表
 * ++++++++++++++++++++++++++++++++++++++++++
 * 注意：这里请务必根据json对应key进行编写
 * 当使用错误时：
 * 会由于operator[]产生空的函数对象，会导致程序崩溃
 * 因此需要外部判断
 */
TaskNode ActionPatch::Gen_TaskType(const QJsonObject &json) {
    static QHash<QString, TaskStepFactory> table;
    {
        static std::once_flag flag;
        std::call_once(flag, [] {
            table[KJ::Gripper]            = &patch_for_Gripper;
            table[KJ::MagneticSeparation] = &patch_for_MagneticSeparation;
            table[KJ::PipettingNeedle]    = &patch_for_PipettingNeedle;
            table[KJ::ReagentBin]         = &patch_for_ReagentBin;
            table[KJ::Mixing]             = &patch_for_Mixing;
            table[KJ::Photoelectric]      = &patch_for_Photoelectric;
            table[KJ::Incubate]           = &patch_for_Incubate;
        });
    }

    const QString device  = json[KJ::deviceName].toString();
    bool          flag    = true;
    flag                 &= table.count(device) != 0;
    flag                 &= json[KJ::op].type() == QJsonValue::Object;
    flag                 &= json[KJ::op][KJ::deviceMode].isDouble();
    if (false == flag) {
        return Parse::parsing_failing(json, __PRETTY_FUNCTION__);
    }
    return table[device](json);
}
TaskNode ActionPatch::patch_for_Gripper(const QJsonObject &json) {
    auto task = [json](Tube::entity_tube tube) -> RetCode {
        auto     &&device = DGripper::instance();
        QJsonValue op     = json[KJ::op];
        assert(op.isObject());

        /// 最重要，运行类型
        int              modeInt = op[KJ::deviceMode].toInt();
        DGripper::Params params;
        /// 目标器件
        if (auto sub = op[KJ::target]; sub.type() == QJsonValue::String) {
            params = device.Get_fixInfo(sub.toString()).params;
            if (sub.toString() == KJ::Incubate) {
                const int i    = op[KJ::i].toInt();
                const int j    = op[KJ::j].toInt();
                auto      info = device.GetInfo_to_Incubate_byXY(i, j);
                params         = info.params;

                /// 根据试管中是否设置判断具体孔位
                auto pos = tube->paramPkg;
                if (pos.incubateX >= 0 && pos.incubateY >= 0) {
                    info   = device.GetInfo_to_Incubate_byXY(pos.incubateX, pos.incubateY);
                    params = info.params;
                }
            }
        }

        /// 显示的写出具体数值，还是优先级最高
        if (auto sub = op[KJ::x]; sub.type() == QJsonValue::Double) {
            params.xPosition = sub.toInt();
        }
        if (auto sub = op[KJ::y]; sub.type() == QJsonValue::Double) {
            params.yPosition = sub.toInt();
        }
        if (auto sub = op[KJ::z]; sub.type() == QJsonValue::Double) {
            params.zPosition   = sub.toInt();
            params.deepGrab    = params.zPosition;
            params.deepRelease = params.zPosition;

        }

        device.Set_Config(modeInt, params);
        return (device.*execTaskFun)();
    };

    return {task, json};
}

TaskNode ActionPatch::patch_for_MagneticSeparation(const QJsonObject &json) {
    auto task = [json](Tube::entity_tube) -> RetCode {
        auto     &&device = DMagneticSeparation::instance();
        QJsonValue op     = json[KJ::op];
        assert(op.isObject());

        int                         modeInt = op[KJ::deviceMode].toInt();
        DMagneticSeparation::Params params;
        if (auto sub = op[KJ::target]; sub.type() == QJsonValue::String) {
            params = device.Get_fixParams(sub.toString());
        }
        if (auto sub = op[KJ::idx]; sub.type() == QJsonValue::Double) {
            //! 请保证，在操作这里之前已经设置过pos了
            params.position = device.Offset_byIndex(params, sub.toInt()).position;
        }
        if (auto sub = op[KJ::capacity]; sub.type() == QJsonValue::Double) {
            params.washCapacity       = sub.toInt();
            params.aSubstrateCapacity = sub.toInt();
        }
        /// 终极强制数据
        if (auto sub = op[KJ::position]; sub.type() == QJsonValue::Double) {
            params.position = sub.toInt();
        }

        device.Set_Config(modeInt, params);
        return (device.*execTaskFun)();
    };

    return {task, json};
}

TaskNode ActionPatch::patch_for_PipettingNeedle(const QJsonObject &json) {
    auto task = [json](Tube::entity_tube tube) -> RetCode {
        auto     &&device = DPipetteNeedle::instance();
        QJsonValue op     = json[KJ::op];
        assert(op.isObject());

        int                    modeInt = op[KJ::deviceMode].toInt();
        DPipetteNeedle::Params params;
        if (auto sub = op[KJ::target]; sub.type() == QJsonValue::String) {
            const QString target = sub.toString();
            params               = device.Get_fixInfo(target).params;
            /// 如果是试剂仓
            if (target == KJ::ReagentBin) {
                /// 如果是到试剂仓，则对应后转的第几号位
                if (auto sub = op[KJ::idxBinBack]; sub.type() == QJsonValue::Double) {
                    params = device.GetInfo_to_ReagentBin(sub.toInt()).params;
                }
            }
        }

        /// 下面是绝对参数设置
        if (auto sub = op[KJ::y]; sub.type() == QJsonValue::Double) {
            params.yPosition     = sub.toInt();
            params.washYPosition = params.yPosition;
        }
        if (auto sub = op[KJ::z]; sub.type() == QJsonValue::Double) {
            params.zPosition         = sub.toInt();
            params.washZPosition     = params.zPosition;
            params.suctionZPosition  = params.zPosition;
            params.drainageZPosition = params.zPosition;
        }
        if (auto sub = op[KJ::capacity]; sub.type() == QJsonValue::Double) {
            params.suctionCapacity = sub.toInt();
        }
        if (auto sub = op[KJ::threshold]; sub.type() == QJsonValue::Double) {
            params.suctionThreshold = sub.toInt();
        }

        /// 0 默认的液面检测
        /// 1 试剂的随量跟踪
        /// 2 固定下探位置
        if (auto sub = op[KJ::suctionMode]; sub.toInt() == 0) {
            /// pass
            /// 保持原样，默认就是液面检测
        } else if (sub.toInt() == 1) {
            const int idxBinGroup = op[KJ::idxBinGroup].toInt();
            const int idxBinBack  = op[KJ::idxBinBack].toInt();
            device.Suction_reagentTrack(params, idxBinGroup, idxBinBack);
        } else if (sub.toInt() == 2) {
            const int idxBinGroup = op[KJ::idxBinGroup].toInt();
            const int idxBinBack  = op[KJ::idxBinBack].toInt();
            device.Suction_sampleTrack(params, idxBinGroup, idxBinBack);
        } else {
            sLog_PipettingNeedle() << QString("[suctionMode] 设置异常");
        }

        if (tube) {
            /// 吸取时，上传吸取量的信号
            switch (modeInt) {
            case DPipetteNeedle::Mode_Suction:
            case DPipetteNeedle::Mode_MoveSuction: {
                tube->signal_pipetteNeedleSuction(op[KJ::idxBinGroup].toInt(),
                                                  op[KJ::idxBinBack].toInt(),
                                                  op[KJ::capacity].toInt());

            } break;
            }
        }

        device.Set_Config(modeInt, params);
        auto ret = (device.*execTaskFun)();

        if (modeInt == DPipetteNeedle::Mode_MoveSuction ||
            modeInt == DPipetteNeedle::Mode_Suction) {
            sLog_PipettingNeedle()
                << QString("移液针测试log idxBinGroup=[%1] idxBinBack=[%2] zpos=[%3] 锁存=[%4]")
                       .arg(op[KJ::idxBinGroup].toInt())
                       .arg(op[KJ::idxBinBack].toInt())
                       .arg(params.suctionZPosition)
                       .arg(device.Get_mcuParams().descentCache);
        }
        return ret;
    };

    return {task, json};
}

TaskNode ActionPatch::patch_for_ReagentBin(const QJsonObject &json) {
    auto task = [json](Tube::entity_tube) -> RetCode {
        auto     &&device = DReagentBin::instance();
        QJsonValue op     = json[KJ::op];
        assert(op.isObject());

        int                 modeInt = op[KJ::deviceMode].toInt();
        DReagentBin::Params params{};
        /// [0, 6]孔到后方
        if (auto sub = op[KJ::idxBinBack]; sub.type() == QJsonValue::Double) {
            params = device.GetInfo_backHole(sub.toInt());
        }
        /// [0, 6]孔到前方
        if (auto sub = op[KJ::idxBinFront]; sub.type() == QJsonValue::Double) {
            params = device.GetInfo_frontHole(sub.toInt());
        }
        /// 第几个仓
        if (auto sub = op[KJ::idxBinGroup]; sub.type() == QJsonValue::Double) {
            params = device.Offset_byIndex(params, sub.toInt());
        }
        /// 绝对的手动定位，最后判断
        if (auto sub = op[KJ::position]; sub.type() == QJsonValue::Double) {
            params.position = sub.toInt();
        }
        if (auto sub = op[KJ::velocity]; sub.type() == QJsonValue::Double) {
            params.velocity = sub.toInt();
        }

        device.Offset_byProximity(params);
        device.Set_Config(modeInt, params);
        return (device.*execTaskFun)();
    };

    return {task, json};
}

TaskNode ActionPatch::patch_for_Mixing(const QJsonObject &json) {
    auto task = [json](Tube::entity_tube) -> RetCode {
        QJsonValue op = json[KJ::op];
        assert(op.isObject());

        int modeInt = op[KJ::deviceMode].toInt();

        auto &&device = DMixing::instance();
        device.Set_Config(modeInt, {});
        return (device.*execTaskFun)();
    };

    return {task, json};
}

TaskNode ActionPatch::patch_for_Photoelectric(const QJsonObject &json) {
    auto task = [json](Tube::entity_tube) -> RetCode {
        auto     &&device = DPhotoelectric::instance();
        QJsonValue op     = json[KJ::op];
        assert(op.isObject());

        int                    modeInt = op[KJ::deviceMode].toInt();
        DPhotoelectric::Params params{};
        params = device.GetParams_defaultCapacity();
        if (auto sub = op[KJ::capacity]; sub.isDouble()) {
            params.capacity = sub.toInt();
        }

        device.Set_Config(modeInt, params);
        return (device.*execTaskFun)();
    };

    return {task, json};
}

TaskNode ActionPatch::patch_for_Incubate(const QJsonObject &json) {
    auto task = [json](Tube::entity_tube) -> RetCode { return RetCode::Success; };

    return {task, json};
}

}  // namespace COM
