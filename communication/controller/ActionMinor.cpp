#include "ActionMinor.h"

#include "../COM_Device.hpp"
#include "../config/CKeyJson.hpp"
#include "ActionPatch.h"
#include "Parse.h"
#include "communication/COM_pch.hpp"

namespace COM {

namespace {
constexpr int Try_Again_Count = 0;

QString get_idxBinMessage(const QJsonValue& idxBinGroup, const QJsonValue& idxBinBack) {
    int group = idxBinGroup.toInt();
    int back  = idxBinBack.toInt();

    QString s1 = QString("[组号: %1]").arg(group + 1);
    QString s2;
    if (back < 4) {
        constexpr char str[] = "abcd";
        s2                   = QString("[腔体：%1]").arg(str[back]);
    } else {
        s2 = QString("[孔号：%1]").arg(group * 3 + (back - 3));
    }
    return s1 + " " + s2;
}
}  // namespace

TaskNode ActionMinor::Gen_TaskType(const QJsonObject& json) {
    static QHash<QString, TaskStepFactory> table;
    {
        static std::once_flag flag;
        std::call_once(flag, [] {
            //! 每个部件，属于自己的核心步骤
            table[KJ::Gripper]         = &Minor_for_Gripper_from_to;
            table[KJ::PipettingNeedle] = &Minor_for_PipetteNeedle_from_to;
            table[KJ::Photoelectric]   = &Minor_for_Photoelectric_detect_once;
            table[KJ::Dilute]          = &Minor_for_Dilute;
            table[KJ::Mixing]          = &Minor_for_Mixing;
            table[KJ::DoubleSuction]   = &Minor_for_PipetteNeedle_DoubleSuction;
        });
    }

    const QString device = json[KJ::deviceName].toString();
    if (table.count(device) == 0) {
        return Parse::parsing_failing(json, __PRETTY_FUNCTION__);
    }
    return table[device](json);
}

/**
 * 夹爪的本职工作就是将物体转移
 * + 移动error，先homing再移动
 * + 夹取error，重试一次
 * =======================================
 *   {
 *       "actionType": "ActionMinor",
 *       "deviceName": "Gripper",
 *       "from": {
 *           "target": "Cup"
 *       },
 *       "to": {
 *           "target": "Load"
 *       }
 *   }
 */
TaskNode ActionMinor::Minor_for_Gripper_from_to(const QJsonObject& ojson) {
    auto task = [ojson](Tube::entity_tube tube) -> TaskNode::return_type {
        using Device = DGripper;

        //封装 retry 的动作逻辑：若任务执行失败，会 homing 一次（复位），再尝试重新执行
        TaskNode task;
        auto     try_action = [&]() -> RetCode {
            auto&   gripper = DGripper::instance();
            RetCode ret     = task(tube);
            for (int n = Try_Again_Count; n > 0 && ret != RetCode::Success; n += -1) {
                gripper.Set_Config(DGripper::Mode_Homing, {});
                gripper.Exec_sequencedTask();
                ret = task(tube);
            }
            return ret;
        };

        /// 转为单步(原子)的操作
        QJsonObject json     = ojson;
        json[KJ::actionType] = KJ::ActionPatch;

        QJsonObject op;
        /// from 夹取
        op                 = json[KJ::from].toObject();
        op[KJ::deviceMode] = Device::Mode_MoveGrab;

        json[KJ::op]       = op;

        task               = ActionPatch::patch_for_Gripper(json);
        if (auto ret = try_action(); ret != RetCode::Success) {
            tube->Append_strMsg(ret.to_string());
            return ret;
        }

        /// to 释放
        op                 = json[KJ::to].toObject();
        op[KJ::deviceMode] = Device::Mode_MoveRelease;
        json[KJ::op]       = op;
        task               = ActionPatch::patch_for_Gripper(json);
        if (auto ret = try_action(); ret != RetCode::Success) {
            tube->Append_strMsg(ret.to_string());
            return ret;
        }

        return TaskNode::return_type::Success;
    };

    TaskNode obj{task, ojson};
    return obj;
}

/**
 * 移液针的本职工作就是转移试剂
 * ========================================
 *   {
 *       "actionType": "ActionMinor",
 *       "deviceName": "PipettingNeedle",
 *       "from": {
 *           "target": "ReagentBin",
 *           "idxBinBack": 6,
 *           "idxBinGroup": 0
 *       },
 *       "to": {
 *           "target": "Load"
 *       }
 *   }
 */
TaskNode ActionMinor::Minor_for_PipetteNeedle_from_to(const QJsonObject& ojson) {
    /**
     * 保证最后能去洗针
     */
    auto task = [ojson](Tube::entity_tube tube) -> TaskNode::return_type {
        using Device = DPipetteNeedle;
        QVector<TaskNode::return_type> retList;

        TaskNode task;
        auto     try_action = [&]() -> RetCode {
            auto&   needle = DPipetteNeedle::instance();
            RetCode ret    = task(tube);
            for (int n = Try_Again_Count; n > 0 && ret != RetCode::Success; n += -1) {
                needle.Set_Config(DPipetteNeedle::Mode_Homing, {});
                needle.Exec_sequencedTask();
                needle.Set_Config(DPipetteNeedle::Mode_Wash, needle.GetInfo_to_Wash().params);
                needle.Exec_sequencedTask();
                ret = task(tube);
            }
            return ret;
        };

        /// 转为单步的操作
        QJsonObject json     = ojson;
        json[KJ::actionType] = KJ::ActionPatch;
        QJsonObject op;

        op                 = ojson[KJ::from].toObject();
        op[KJ::deviceMode] = Device::Mode_MoveSuction;
        json[KJ::op]       = op;
        task               = ActionPatch::patch_for_PipettingNeedle(json);
        if (auto ret = task(tube); ret != RetCode::Success) {
            tube->Append_strMsg(ret.to_string());
            retList << ret;
            sLog_PipettingNeedle()
                << ret << QString("移液针mode[%1]").arg(op[KJ::deviceMode].toInt())
                << get_idxBinMessage(op[KJ::idxBinGroup], op[KJ::idxBinBack]);
        } else {
            retList << TaskNode::return_type::Success;
        }

        op                 = ojson[KJ::to].toObject();
        op[KJ::deviceMode] = Device::Mode_MoveDrainage;
        json[KJ::op]       = op;
        task               = ActionPatch::patch_for_PipettingNeedle(json);
        if (auto ret = try_action(); ret != RetCode::Success) {
            tube->Append_strMsg(ret.to_string());
            retList << ret;
            sLog_PipettingNeedle()
                << ret << QString("移液针mode[%1]").arg(op[KJ::deviceMode].toInt());
        } else {
            retList << TaskNode::return_type::Success;
        }

        /// 清洗时，考虑前面有特定值
        /// 就重置数据
        op                 = QJsonObject{};
        op[KJ::target]     = KJ::Wash;
        op[KJ::deviceMode] = Device::Mode_Wash;
        if (const int combine = json[KJ::actionTypeCombine].toInt(); combine == 0) {
            op[KJ::deviceMode] = Device::Mode_Wash;
        } else {
            op[KJ::deviceMode] = Device::Mode_Homing;
        }
        json[KJ::op] = op;
        task         = ActionPatch::patch_for_PipettingNeedle(json);
        if (auto ret = try_action(); ret != RetCode::Success) {
            tube->Append_strMsg(ret.to_string());
            retList << ret;
            sLog_PipettingNeedle()
                << ret << QString("移液针mode[%1]").arg(op[KJ::deviceMode].toInt());
        } else {
            retList << TaskNode::return_type::Success;
        }

        for (const auto ret : retList) {
            if (ret != TaskNode::return_type::Success) {
                return ret;
            }
        }
        return TaskNode::return_type::Success;
    };

    TaskNode obj{task, ojson};
    return obj;
}

/**
 * 同时配多组，这里配两组，中间使用清洗外壁模式
 * {
 *    "actionType": "ActionMinor",
 *    "deviceName": "DoubleSuction",
 *    "from": [
 *        {
 *            "idxBinBack": 3,
 *            "idxBinGroup": 0,
 *            "target": "ReagentBin"
 *        },
 *        {
 *            "idxBinBack": 3,
 *            "idxBinGroup": 0,
 *            "target": "ReagentBin"
 *        }
 *    ],
 *    "to": {
 *        "target": "Load"
 *    }
 *}
 */
TaskNode ActionMinor::Minor_for_PipetteNeedle_DoubleSuction(const QJsonObject& ojson) {
    auto deal_reagentBin = [](const QJsonObject& op) {
        using Device   = DReagentBin;
        Device& device = Device::instance();

        const int idxBin  = op[KJ::idxBinGroup].toInt();
        const int idxBack = op[KJ::idxBinBack].toInt();
        auto      params  = device.GetInfo_backHole(idxBack);
        device.Offset_byIndex(params, idxBin);
        device.Offset_byProximity(params);
        device.Set_Config(Device::Mode_Position, params);
        device.Exec_sequencedTask();
    };

    /**
     * 保证最后能去洗针
     */
    auto task = [ojson, deal_reagentBin](Tube::entity_tube tube) -> TaskNode::return_type {
        using Device = DPipetteNeedle;
        QVector<TaskNode::return_type> retList;

        TaskNode task;

        /// 转为单步的操作
        QJsonObject json     = ojson;
        json[KJ::actionType] = KJ::ActionPatch;
        QJsonObject op;

        auto fromArr = ojson[KJ::from].toArray();
        assert(fromArr.count() == 2);

        op                 = fromArr[0].toObject();
        op[KJ::deviceMode] = Device::Mode_MoveSuction;
        json[KJ::op]       = op;
        deal_reagentBin(op);
        task = ActionPatch::patch_for_PipettingNeedle(json);
        if (auto ret = task(tube); ret != RetCode::Success) {
            tube->Append_strMsg(ret.to_string());
            retList << ret;
            sLog_PipettingNeedle()
                << ret << QString("移液针mode[%1]").arg(op[KJ::deviceMode].toInt())
                << get_idxBinMessage(op[KJ::idxBinGroup], op[KJ::idxBinBack]);
        } else {
            retList << TaskNode::return_type::Success;
        }

        {
            PipetteNeedle_delayWashTime_Guard guard(2000);
            op[KJ::deviceMode] = Device::Mode_WashOutside;
            /// 清洗时，考虑前面有特定值
            /// 就重置数据
            op                 = QJsonObject{};
            op[KJ::target]     = KJ::Wash;
            op[KJ::deviceMode] = Device::Mode_WashOutside;
            if (const int combine = json[KJ::actionTypeCombine].toInt(); combine == 0) {
                op[KJ::deviceMode] = Device::Mode_WashOutside;
            } else {
                op[KJ::deviceMode] = Device::Mode_Homing;
            }
            json[KJ::op] = op;
            task         = ActionPatch::patch_for_PipettingNeedle(json);
            if (auto ret = task(tube); ret != RetCode::Success) {
                tube->Append_strMsg(ret.to_string());
                retList << ret;
                sLog_PipettingNeedle()
                    << ret << QString("移液针mode[%1]").arg(op[KJ::deviceMode].toInt());
            } else {
                retList << TaskNode::return_type::Success;
            }
        }

        op                 = fromArr[1].toObject();
        op[KJ::deviceMode] = Device::Mode_MoveSuction;
        json[KJ::op]       = op;
        deal_reagentBin(op);
        task = ActionPatch::patch_for_PipettingNeedle(json);
        if (auto ret = task(tube); ret != RetCode::Success) {
            tube->Append_strMsg(ret.to_string());
            retList << ret;
            sLog_PipettingNeedle()
                << ret << QString("移液针mode[%1]").arg(op[KJ::deviceMode].toInt())
                << get_idxBinMessage(op[KJ::idxBinGroup], op[KJ::idxBinBack]);
        } else {
            retList << TaskNode::return_type::Success;
        }

        op                 = ojson[KJ::to].toObject();
        op[KJ::deviceMode] = Device::Mode_MoveDrainage;
        json[KJ::op]       = op;
        task               = ActionPatch::patch_for_PipettingNeedle(json);
        if (auto ret = task(tube); ret != RetCode::Success) {
            tube->Append_strMsg(ret.to_string());
            retList << ret;
            sLog_PipettingNeedle()
                << ret << QString("移液针mode[%1]").arg(op[KJ::deviceMode].toInt());
        } else {
            retList << TaskNode::return_type::Success;
        }

        /// 清洗时，考虑前面有特定值
        /// 就重置数据
        op                 = QJsonObject{};
        op[KJ::target]     = KJ::Wash;
        op[KJ::deviceMode] = Device::Mode_Wash;
        if (const int combine = json[KJ::actionTypeCombine].toInt(); combine == 0) {
            op[KJ::deviceMode] = Device::Mode_Wash;
        } else {
            op[KJ::deviceMode] = Device::Mode_Homing;
        }
        json[KJ::op] = op;
        task         = ActionPatch::patch_for_PipettingNeedle(json);
        if (auto ret = task(tube); ret != RetCode::Success) {
            tube->Append_strMsg(ret.to_string());
            retList << ret;
            sLog_PipettingNeedle()
                << ret << QString("移液针mode[%1]").arg(op[KJ::deviceMode].toInt());
        } else {
            retList << TaskNode::return_type::Success;
        }

        for (const auto ret : retList) {
            if (ret != TaskNode::return_type::Success) {
                return ret;
            }
        }
        return TaskNode::return_type::Success;
    };

    TaskNode obj{task, ojson};
    return obj;
}

/**
 * 注意一点，当前不是以一种from-to的模式
 * 而是一堆大步骤的联合
 * ==============================
 * （要求上料位有反应管）-
 * 1.  分杯
 * 2.  (夹爪)分杯位->稀释位
 * 3.  (夹爪)夹爪homing
 * 4.  (移液针)加液（稀释）
 * 5.  (夹爪)稀释位->混匀位
 * 6.  混匀
 * 7.  (夹爪)混匀位->稀释位
 * 8.  (夹爪)homing
 * 9.  (移液针)稀释液->反应管
 * 10. (夹爪)稀释位->抛料位
 * ===============================
 * 可以转化为 Minor-Mixing
 *
 */
TaskNode ActionMinor::Minor_for_Dilute(const QJsonObject& ojson) {
    auto task = [ojson](Tube::entity_tube tube) -> TaskNode::return_type {
        TaskNode    task;
        RetCode     ret;
        QJsonObject json = ojson;

        QJsonObject op;
        QJsonObject gjson;
        QJsonObject mjson;
        QJsonArray  process;

        //! 1. 分杯
        mjson                 = QJsonObject();
        op                    = QJsonObject();
        mjson[KJ::actionType] = KJ::ActionPatch;
        mjson[KJ::deviceName] = KJ::Mixing;
        op[KJ::deviceMode]    = DMixing::Mode_EnableDivideCup;
        mjson[KJ::op]         = op;
        task                  = ActionPatch::patch_for_Mixing(mjson);
        ret                   = task(tube);
        if (ret != RetCode::Success) {
            return ret;
        }

        //! 2. 夹爪从分杯到稀释位
        gjson                 = QJsonObject();
        op                    = QJsonObject();
        gjson[KJ::actionType] = KJ::ActionMinor;
        gjson[KJ::deviceName] = KJ::Gripper;
        op[KJ::target]        = KJ::Cup;
        gjson[KJ::from]       = op;
        op[KJ::target]        = KJ::Dilute;
        gjson[KJ::to]         = op;
        task                  = Minor_for_Gripper_from_to(gjson);
        ret                   = task(tube);
        if (ret != RetCode::Success) {
            return ret;
        }

        //! 3. 夹爪重新homing避让
        gjson                 = QJsonObject();
        op                    = QJsonObject();
        gjson[KJ::actionType] = KJ::ActionPatch;
        gjson[KJ::deviceName] = KJ::Gripper;
        op[KJ::deviceMode]    = DGripper::Mode_Homing;
        gjson[KJ::op]         = op;
        task                  = ActionPatch::patch_for_Gripper(gjson);
        ret                   = task(tube);
        if (ret != RetCode::Success) {
            return ret;
        }

        //! 4. 将所有需要混匀的体液先加入
        process = json[KJ::processPre].toArray();
        for (int i = 0; i < process.size(); i += 1) {
            auto pnjson = process[i].toObject();
            {
                QJsonObject op         = pnjson[KJ::from].toObject();
                const int   idxBin     = op[KJ::idxBinGroup].toInt();
                const int   idxBack    = op[KJ::idxBinBack].toInt();
                auto&&      reagentBin = DReagentBin::instance();
                auto        params     = reagentBin.GetInfo_backHole(idxBack);
                reagentBin.Offset_byIndex(params, idxBin);
                reagentBin.Offset_byProximity(params);
                reagentBin.Set_Config(DReagentBin::Mode_Position, params);
                reagentBin.Exec_sequencedTask();
            }
            task = Minor_for_PipetteNeedle_from_to(pnjson);
            ret  = task(tube);
            if (ret != RetCode::Success) {
                return ret;
            }
        }

        //! 5. 夹爪将稀释位转移到混匀位
        gjson                 = QJsonObject();
        op                    = QJsonObject();
        gjson[KJ::actionType] = KJ::ActionMinor;
        gjson[KJ::deviceName] = KJ::Gripper;
        op[KJ::target]        = KJ::Dilute;
        gjson[KJ::from]       = op;
        op[KJ::target]        = KJ::Mixing;
        gjson[KJ::to]         = op;
        task                  = Minor_for_Gripper_from_to(gjson);
        ret                   = task(tube);
        if (ret != RetCode::Success) {
            return ret;
        }

        //! 6. 混匀
        mjson                 = QJsonObject();
        op                    = QJsonObject();
        mjson[KJ::actionType] = KJ::ActionPatch;
        mjson[KJ::deviceName] = KJ::Mixing;
        op[KJ::deviceMode]    = DMixing::Mode_EnableMix;
        mjson[KJ::op]         = op;
        task                  = ActionPatch::patch_for_Mixing(mjson);
        ret                   = task(tube);
        if (ret != RetCode::Success) {
            return ret;
        }

        //! 7. 从混匀到稀释位
        gjson                 = QJsonObject();
        op                    = QJsonObject();
        gjson[KJ::actionType] = KJ::ActionMinor;
        gjson[KJ::deviceName] = KJ::Gripper;
        op[KJ::target]        = KJ::Mixing;
        gjson[KJ::from]       = op;
        op[KJ::target]        = KJ::Dilute;
        gjson[KJ::to]         = op;
        task                  = Minor_for_Gripper_from_to(gjson);
        ret                   = task(tube);
        if (ret != RetCode::Success) {
            return ret;
        }

        //! 8. 夹爪homing
        gjson                 = QJsonObject();
        op                    = QJsonObject();
        gjson[KJ::actionType] = KJ::ActionPatch;
        gjson[KJ::deviceName] = KJ::Gripper;
        op[KJ::deviceMode]    = DGripper::Mode_Homing;
        gjson[KJ::op]         = op;
        task                  = ActionPatch::patch_for_Gripper(gjson);
        task(tube);

        //! 9. 将稀释的转移到反应管子中
        process = json[KJ::processActual].toArray();
        for (int i = 0; i < process.size(); i += 1) {
            auto pnjson = process[i].toObject();
            task        = Minor_for_PipetteNeedle_from_to(pnjson);
            ret         = task(tube);
            if (ret != RetCode::Success) {
                return ret;
            }
        }

        //! 10. 将稀释管抛掉
        gjson                 = QJsonObject();
        op                    = QJsonObject();
        gjson[KJ::actionType] = KJ::ActionMinor;
        gjson[KJ::deviceName] = KJ::Gripper;
        op[KJ::target]        = KJ::Dilute;
        gjson[KJ::from]       = op;
        op[KJ::target]        = KJ::Throw;
        gjson[KJ::to]         = op;
        task                  = Minor_for_Gripper_from_to(gjson);
        ret                   = task(tube);
        if (ret != RetCode::Success) {
            return ret;
        }

        return TaskNode::return_type::Success;
    };

    TaskNode obj{task, ojson};
    return obj;
}

/**
 * 混匀完直接转移
 *  {
 *     "actionType": "ActionMinor",
 *     "deviceName": "Mixing",
 *     "from": {
 *         "target": "Load"
 *     },
 *     "to": {
 *         "target": "Incubate",
 *         "sleepTime": 300000
 *     }
 *  }
 */
TaskNode ActionMinor::Minor_for_Mixing(const QJsonObject& ojson) {
    auto task = [ojson](Tube::entity_tube tube) -> TaskNode::return_type {
        TaskNode    task;
        RetCode     ret;
        QJsonObject gjson = ojson;
        QJsonObject op;

        //将tube 从load位 转移到 mixing位
        gjson[KJ::deviceName] = KJ::Gripper;
        op[KJ::target]        = KJ::Mixing;
        gjson[KJ::to]         = op;
        task                  = Minor_for_Gripper_from_to(gjson);
        ret                   = task(tube);
        if (ret != RetCode::Success) {
            return ret;
        }

        //启动混匀
        QJsonObject mjson;
        mjson[KJ::actionType] = KJ::ActionPatch;
        mjson[KJ::deviceName] = KJ::Mixing;
        op                    = QJsonObject();
        op[KJ::deviceMode]    = DMixing::Mode_EnableMix;
        mjson[KJ::op]         = op;
        task                  = ActionPatch::patch_for_Mixing(mjson);
        ret                   = task(tube);
        if (ret != RetCode::Success) {
            return ret;
        }

        //将tube 从 mixing位转移到 Incubate位
        gjson                 = ojson;
        gjson[KJ::deviceName] = KJ::Gripper;
        op                    = QJsonObject();
        op[KJ::target]        = KJ::Mixing;
        gjson[KJ::from]       = op;
        task                  = Minor_for_Gripper_from_to(gjson);
        ret                   = task(tube);
        if (ret != RetCode::Success) {
            return ret;
        }

        return TaskNode::return_type::Success;
    };

    TaskNode obj{task, ojson};
    return obj;
}

/**
 * 光电检测的一整套步骤
 * 这里本身是有一个op的参数
 * + 关门
 * + 检测
 * + 开门
 */
TaskNode ActionMinor::Minor_for_Photoelectric_detect_once(const QJsonObject& ojson) {
    using Device = DPhotoelectric;
    /// 光电检测基本都是成功的
    /// 除非放管的时候close卡住了
    auto task    = [ojson](Tube::entity_tube tube) -> TaskNode::return_type {
        TaskNode task;
        /// 转为单步的操作
        QJsonObject json     = ojson;
        json[KJ::actionType] = KJ::ActionPatch;
        QJsonObject op       = json[KJ::op].toObject();

        /// 关门
        op[KJ::deviceMode] = Device::Mode_DoorClose;
        json[KJ::op]       = op;
        task               = ActionPatch::patch_for_Photoelectric(json);
        if (auto ret = task(tube); ret != RetCode::Success) {
            tube->Append_strMsg(ret.to_string());
            return ret;
        }

        /// 检测
        op[KJ::deviceMode] = Device::Mode_Detect;
        json[KJ::op]       = op;
        task               = ActionPatch::patch_for_Photoelectric(json);
        if (auto ret = task(tube); ret != RetCode::Success) {
            tube->Append_strMsg(ret.to_string());
            return ret;
        }
        //! 检测成功，记录数据
        tube->Set_expValue(Device::instance().Get_value());

        /// 开门
        op[KJ::deviceMode] = Device::Mode_DoorOpen;
        json[KJ::op]       = op;
        task               = ActionPatch::patch_for_Photoelectric(json);
        if (auto ret = task(tube); ret != RetCode::Success) {
            tube->Append_strMsg(ret.to_string());
            return ret;
        }

        return TaskNode::return_type::Success;
    };

    TaskNode obj{task, ojson};
    return obj;
}

}  // namespace COM
