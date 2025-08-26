/** ***************************************************************************
 * Copyright © 2024 cuber-lotus
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       CONTROLLER_Project.h
 * @date       2024-05-27
 * @version    1.0.0
 *
 * @author     陆寅峰
 *
 * @brief
 * 项目管理控制文件
 *
 *****************************************************************************/
#ifndef PROJECT_H_1716773554
#define PROJECT_H_1716773554

#include <QFile>
#include <QJsonObject>

#include "CONTROLLER_Calibration.h"
#include "CONTROLLER_ExpSample.h"
#include "CONTROLLER_Reagent.h"
#include "CONTROLLER_Sample.h"
#include "entity/EConfig_Project.h"
#include "global/GLOBAL_KeyProject.h"
#include "global/GLOBAL_Path.hpp"
#include "global/GLOBAL_Type.h"
#include "utility/UTILITY_FileSuffix.h"

namespace CONTROLLER {
inline ::UTILITY::FileSuffix Create_ProjectsFolderController() {
    return ::UTILITY::FileSuffix(to_absoluteFilePath(Path_ProjectsFolder), "json");
}

class Project {
public:
    /**
     * 实验来源枚举
     */
    enum class ExpType : int {
        FromSample,       /// 普通实验
        FromCalibration,  /// 定标实验
        FromQuality,      /// 质控实验
    };

    /**
     * 和后台交互的试剂仓相关的信息
     */
    struct BinInfo {
        /// [后台] 试剂仓的组号
        int idxBinGroup              = 0;
        /// [后台] 孔位
        int idxBinBack               = 0;
        /// 试剂仓内的信息
        QVector<int> liquidTotalList = QVector<int>(4);
        QVector<int> liquidConsume   = QVector<int>(4);

        QString to_string() const {
            QString msg;
            msg += QString("idxBinGroup[%1]").arg(idxBinGroup);
            msg += QString("idxBinBack[%1]").arg(idxBinBack);
            return msg;
        }
    };

    /**
     * 额外辅助信息
     */
    struct ExtraInfo {
        QString project;                                     /// 项目信息
        QString isEmergency;                                 /// 是否急诊
        ExpType expType              = ExpType::FromSample;  /// 实验来源
        bool    isCaliInheritDiluent = false;                /// 校准品是否继承稀释
        bool    isNeedDiluent        = false;                /// 是否稀释
        Type::SampleTubeType sampleTubeType = Type::SampleTubeType::GeneralTube;  /// 样本管类型
    };

    /**
     * 根据用户输入的`试剂位`
     * 得到后端控制映射的试剂仓信息
     * =======================
     * 注意：
     * * 用户范围 [1, 10]
     * * 后端范围 [0, 9]
     */
    static BinInfo Create_reagentBinInfo(const QString&                   holeStr,
                                         CONTROLLER::Reagent::entity_item reagent);

    /**
     * 根据用户输入的`样本位`
     * 得到后端控制映射的试剂仓信息
     * =======================
     * 注意：
     * * 用户范围 [1, 20] & [E1, E10]
     * * 后端范围 [0, 9] & {0, 1, 2}
     * ********
     *    E1  *
     *  2     *
     *      1 *
     * ********
     *    |
     *    v
     * ********
     *    0   *
     *  1     *
     *      2 *
     * ********
     * 三个样本位是一组
     */
    static BinInfo Create_sampleBinInfo(const QString& holeStr);

    /**
     * 将试剂和样本基于项目这个共同的属性来结合
     * 核心结果保存在 json 中
     * 返回为false, json未定义
     */
    static QJsonObject Merge_FlowAndLocation(ExtraInfo info, BinInfo reagentBinInfo,
                                             BinInfo sampleBinInfo);

private:
    static QJsonObject create_actionStep(const QString& deviceName, const QJsonObject& from,
                                         const QJsonObject& to);
    static void add_step(QJsonObject& json, const QString& deviceName, const QJsonObject& from,
                         const QJsonObject& to);
    static void Move_PipetteBeforeDoubleSuction(QJsonObject& re_json);
    static void Move_Sample_PipetteBeforeOther(QJsonObject& re_json);
    static void add_diluteStep(QJsonObject& json, const QString& deviceName,
                               const QJsonArray& processPre, const QJsonArray& processActual);
    static void handle_suctionStep(QJsonObject& re_json, const QJsonObject& step,
                                   BinInfo reagentBinInfo, int regent_level_tracking_config);
    static void handle_single_suctionStep(QJsonObject& re_json, const QJsonObject& step,
                                   BinInfo reagentBinInfo, int regent_level_tracking_config);
    static void One_step_reaction(ExtraInfo info, QJsonObject json, QJsonObject& re_json,
                                  Project::BinInfo reagentBinInfo, Project::BinInfo sampleBinInfo);
    static void Delay_one_step_reaction(ExtraInfo info, QJsonObject json, QJsonObject& re_json,
                                        Project::BinInfo reagentBinInfo,
                                        Project::BinInfo sampleBinInfo);
    static int  dilte_calc(double dilution_multiple, int sample_volume);
};
}  // namespace CONTROLLER

#endif  // PROJECT_H_1716773554
