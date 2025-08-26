/** ***************************************************************************
 * Copyright © 2024 cuber-lotus
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       CONTROLLER_Project.cpp
 * @date       2024-06-04
 * @version    1.0.0
 *
 * @author     陆寅峰
 *
 * @brief
 * 该文件依赖于通讯包结构的业务逻辑
 *
 *****************************************************************************/
#include "CONTROLLER_Project.h"

#include <QDebug>
#include <QJsonDocument>

#include "communication/config/CKeyJson.hpp"
#include "communication/utility/UnitConv.h"
#include "controller/IniSC1000.h"
#include "global/GLOBAL_KeyProject.h"

namespace CONTROLLER {
namespace KJ                 = COM::KJ;
namespace BM                 = KeyProject::BaseMsessage;
namespace RS                 = KeyProject::ReactionScheme;
namespace FD                 = KeyProject::FixedDilution;

///a:吖叮酯 d:磁珠
QMap<QString, int> cavityMap = {{"A瓶a腔", 0}, {"A瓶b腔", 1}, {"A瓶c腔", 2}, {"A瓶d腔", 3}};
QMap<QString, int> em        = {{KeyProject::Emergency_t, 1}, {KeyProject::convention, 0}};

Project::BinInfo Project::Create_reagentBinInfo(const QString&                   holeStr,
                                                CONTROLLER::Reagent::entity_item reagent) {
    int idx = holeStr.toInt();
    assert(idx >= 1 && idx <= 10);
    idx += -1;

    BinInfo info;
    info.idxBinGroup    = idx;
    QString fillingtext = reagent->table[Reagent::entity_attr::filling];  // eg. 8,0,0,8  单位:ml
    QStringList fillingList = fillingtext.split(",", QString::SkipEmptyParts);
    for (int i = 0; i < fillingList.size() && i < info.liquidTotalList.size(); ++i) {
        bool ok;
        int  value = fillingList[i].toInt(&ok);
        if (ok) {
            info.liquidTotalList[i] = value * 1000;  // ml --> ul
        }
    }
    return info;
}

/**
 * * 用户范围 [1, 20] & [E1, E10]
 * * 后端范围 [0, 9] & {0, 1, 2}
 **********
 *   E1   *
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
 */
Project::BinInfo Project::Create_sampleBinInfo(const QString& holeStr) {
    BinInfo info;

    int idx = holeStr.toInt();
    assert(idx >= 1 && idx <= 30);
    info.idxBinGroup = (idx - 1) / 3;
    info.idxBinBack  = (idx - 1) % 3;
    return info;
}

/**
 * 1. 判断试剂和样本对应的项目是否一致
 * 2. 判断项目列表是否有这个项目的文件
 * 3. 根据试剂和样本的属性修改配置json
 * 4. 返回为false, json未定义
 * ==============================
 * 试剂：项目 => 1:1
 * 样本：项目 => 1:n
 */
QJsonObject Project::Merge_FlowAndLocation(ExtraInfo info, Project::BinInfo reagentBinInfo,
                                           Project::BinInfo sampleBinInfo) {
    auto    fileCU = Create_ProjectsFolderController();
    QString path   = fileCU.Get_projectPath(info.project);
    QFile   file(path);

    if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
        qWarning() << QString("File[%1] exist[%2] open[%3]")
                          .arg(path)
                          .arg(file.exists())
                          .arg(file.isOpen());
        return {};
    }
    // 获取文件的 JSON 对象
    QJsonObject json = QJsonDocument::fromJson(file.readAll()).object();
    if (json.isEmpty()) {
        qWarning() << "JSON 文件解析失败";
        return {};
    }

    // 根据项目配置生成项目的 re_json 流程文件
    QJsonObject re_json, emergency, BaseMsessage, projectMessage;
    re_json[KJ::message]      = info.project;
    re_json[KJ::emergency]    = em[info.isEmergency];
    re_json[KJ::processSteps] = QJsonArray();

    // 根据反应模式生成流程
    BaseMsessage            = json.value(BM::BaseMsessage).toObject();
    projectMessage          = BaseMsessage.value(BM::ProjectMessage).toObject();
    QString Responsive_Mode = projectMessage.value(BM::ResponsiveMode).toString();

    switch (info.expType) {
    case ExpType::FromSample: {
        // Inherit project file settings
        info.isNeedDiluent =
            (projectMessage.value(FD::Fixed_dilution_switch).toString() == "打开") ? true : false;
    } break;
    case ExpType::FromCalibration: {
        // Inherit (project file settings & quality control product information)
        info.isNeedDiluent =
            (projectMessage.value(FD::Fixed_dilution_switch).toString() == "打开") ? true : false;
        info.isNeedDiluent = info.isNeedDiluent && info.isCaliInheritDiluent;
    } break;
    case ExpType::FromQuality: {
        // Inherit project file settings
        info.isNeedDiluent =
            (projectMessage.value(FD::Fixed_dilution_switch).toString() == "打开") ? true : false;

    } break;
    }

    if (Responsive_Mode == BM::OneStepMethod) {
        One_step_reaction(info, json, re_json, reagentBinInfo, sampleBinInfo);
    } else if (Responsive_Mode == BM::DelayedOneStepMethod) {
        Delay_one_step_reaction(info, json, re_json, reagentBinInfo, sampleBinInfo);
    }

//    qDebug() << "Parsed RE_JSON:" << endl
//             << reagentBinInfo.to_string() << endl
//             << sampleBinInfo.to_string() << endl
//             << re_json;
    qDebug().noquote().nospace() << "Parsed RE_JSON:" << endl
                                 << reagentBinInfo.to_string() << endl
                                 << sampleBinInfo.to_string() << endl
                                 << QJsonDocument(re_json).toJson(QJsonDocument::Indented);

    return re_json;
}

QJsonObject Project::create_actionStep(const QString& deviceName, const QJsonObject& from,
                                       const QJsonObject& to) {
    QJsonObject step;
    step[KJ::actionType] = KJ::ActionMinor;
    step[KJ::deviceName] = deviceName;
    step[KJ::from]       = from;
    step[KJ::to]         = to;
    return step;
}

void Project::add_step(QJsonObject& json, const QString& deviceName, const QJsonObject& from,
                       const QJsonObject& to) {
    QJsonArray  processStepsArray = json[KJ::processSteps].toArray();
    QJsonObject newStep           = create_actionStep(deviceName, from, to);
    processStepsArray.append(newStep);
    json[KJ::processSteps] = processStepsArray;
}
void Project::Move_PipetteBeforeDoubleSuction(QJsonObject& re_json) {
    if (!re_json.contains("processSteps") || !re_json["processSteps"].isArray()) {
        qWarning() << "re_json 中不包含有效的 processSteps 数组";
        return;
    }

    QJsonArray steps = re_json["processSteps"].toArray();

    int moveIndex = -1;
    QJsonObject pipetteToMove;

    // 查找符合条件的 PipettingNeedle（idxBinBack >= 4）
    for (int i = 0; i < steps.size(); ++i) {
        QJsonObject step = steps[i].toObject();
        if (step["deviceName"].toString() == "PipettingNeedle" &&
            step["from"].isObject()) {
            QJsonObject from = step["from"].toObject();
            int idxBinBack = from.value("idxBinBack").toInt(-1);
            if (idxBinBack >= 4) {
                moveIndex = i;
                pipetteToMove = step;
                break;
            }
        }
    }

    if (moveIndex == -1) {
        qWarning() << "未找到满足条件的 PipettingNeedle 元素";
        return;
    }

    // 删除该元素
    steps.removeAt(moveIndex);

    // 找到 DoubleSuction 的位置
    int insertIndex = -1;
    for (int i = 0; i < steps.size(); ++i) {
        QJsonObject step = steps[i].toObject();
        if (step["deviceName"].toString() == "DoubleSuction") {
            insertIndex = i;
            break;
        }
    }

    if (insertIndex == -1) {
        qWarning() << "未找到 DoubleSuction 元素";
        return;
    }

    // 插入到 DoubleSuction 前
    steps.insert(insertIndex, pipetteToMove);

    // 更新 re_json
    re_json["processSteps"] = steps;

    qDebug() << "成功将 PipettingNeedle (idxBinBack >= 4) 移动到 DoubleSuction 前";
}

void Project::Move_Sample_PipetteBeforeOther(QJsonObject& re_json) {
    if (!re_json.contains("processSteps") || !re_json["processSteps"].isArray()) {
            qWarning() << "re_json 中不包含有效的 processSteps 数组";
            return;
        }

        QJsonArray steps = re_json["processSteps"].toArray();
        QList<QJsonObject> toMove;
        QList<int> toRemoveIndices;

        for (int i = 0; i < steps.size(); ++i) {
            const QJsonObject& step = steps[i].toObject();
            if (step["deviceName"].toString() != "PipettingNeedle")
                continue;

            if (!step["from"].isObject())
                continue;

            QJsonObject from = step["from"].toObject();
            int idxBinBack = from.value("idxBinBack").toInt(-1);
            if (idxBinBack >= 4) {
                toMove.append(step);
                toRemoveIndices.prepend(i);
            }
        }

        if (toMove.isEmpty()) {
            return;
        }

        for (int idx : toRemoveIndices)
            steps.removeAt(idx);

        int insertIndex = steps.size();
        for (int i = 0; i < steps.size(); ++i) {
            const QJsonObject& step = steps[i].toObject();
            if (step["deviceName"].toString() == "PipettingNeedle") {
                insertIndex = i;
                break;
            }
        }

        for (int i = 0; i < toMove.size(); ++i) {
            steps.insert(insertIndex + i, toMove[i]);
        }
        re_json["processSteps"] = steps;
        qDebug() << QString("已将 %1 个 idxBinBack>=4 的 PipettingNeedle 前移").arg(toMove.size());
}

void Project::add_diluteStep(QJsonObject& json, const QString& deviceName,
                             const QJsonArray& processPre, const QJsonArray& processActual) {
    QJsonArray  processStepsArray = json.value(KJ::processSteps).toArray();
    QJsonObject diluteStep;
    diluteStep[KJ::actionType]    = KJ::ActionMinor;
    diluteStep[KJ::deviceName]    = deviceName;
    diluteStep[KJ::processActual] = processActual;
    diluteStep[KJ::processPre]    = processPre;

    processStepsArray.append(diluteStep);
    json[KJ::processSteps] = processStepsArray;
}

void Project::handle_single_suctionStep(QJsonObject& re_json, const QJsonObject& step,
                                 BinInfo reagentBinInfo, int regent_level_tracking_config)
{
        using Component = std::tuple<QString /* cavityKey */, QString /* volumeKey */>;

        const std::vector<Component> keys = {
            {RS::R1cavity, RS::R1CLA},
            {RS::R2cavity, RS::R2CLA},
            {RS::R3cavity, RS::R3CLA},
        };

        for (const auto& [cavityKey, volumeKey] : keys) {
            const QString cavityStr = step.value(cavityKey).toString();
            if (cavityStr.isEmpty()|| cavityStr == "/") continue;

            const int volumeMicrolitre = step.value(volumeKey).toString().toInt();
            const int volumeSteps = COM::microlitre_to_motorStep_PipetteNeedle(volumeMicrolitre);

            QJsonObject from, to;
            from[KJ::target]      = KJ::ReagentBin;
            from[KJ::idxBinBack]  = cavityMap.value(cavityStr);  // 直接使用全局变量
            from[KJ::idxBinGroup] = reagentBinInfo.idxBinGroup;
            from[KJ::suctionMode] = regent_level_tracking_config;
            from[KJ::capacity]    = volumeSteps;

            to[KJ::target] = KJ::Load;

            add_step(re_json, KJ::PipettingNeedle, from, to);
        }
}
void Project::handle_suctionStep(QJsonObject& re_json, const QJsonObject& step,
                                 BinInfo reagentBinInfo, int regent_level_tracking_config) {
    QJsonObject from, to;

    int r1ComponentVolume = step.value(RS::R1CLA).toString().toInt();
    int r2ComponentVolume = step.value(RS::R2CLA).toString().toInt();
    int r3ComponentVolume = step.value(RS::R3CLA).toString().toInt();
    r1ComponentVolume     = COM::microlitre_to_motorStep_PipetteNeedle(r1ComponentVolume);
    r2ComponentVolume     = COM::microlitre_to_motorStep_PipetteNeedle(r2ComponentVolume);
    r3ComponentVolume     = COM::microlitre_to_motorStep_PipetteNeedle(r3ComponentVolume);
    QString r1_cav        = step.value(RS::R1cavity).toString();
    QString r2_cav        = step.value(RS::R2cavity).toString();
    QString r3_cav        = step.value(RS::R3cavity).toString();

    from[KJ::target]      = KJ::ReagentBin;
    from[KJ::idxBinBack]  = cavityMap.value(r1_cav, -1);
    from[KJ::idxBinGroup] = reagentBinInfo.idxBinGroup;
    from[KJ::suctionMode] = regent_level_tracking_config;
    from[KJ::capacity]    = r1ComponentVolume;
    to[KJ::target]        = KJ::Load;
    if (cavityMap.value(r2_cav, -1) == -1) {
        add_step(re_json, KJ::PipettingNeedle, from, to);
        return;
    }
    QJsonArray jsonArr;
    jsonArr.append(from);

    from[KJ::target]      = KJ::ReagentBin;
    from[KJ::idxBinBack]  = cavityMap.value(r2_cav, -1);
    from[KJ::idxBinGroup] = reagentBinInfo.idxBinGroup;
    from[KJ::suctionMode] = regent_level_tracking_config;
    from[KJ::capacity]    = r2ComponentVolume;
    to[KJ::target]        = KJ::Load;
    jsonArr.append(from);

    QJsonObject op;
    op[KJ::actionType]     = KJ::ActionMinor;
    op[KJ::deviceName]     = KJ::DoubleSuction;
    op[KJ::from]           = jsonArr;
    op[KJ::to]             = to;
    auto processStepsArray = re_json[KJ::processSteps].toArray();
    processStepsArray.append(op);
    re_json[KJ::processSteps] = processStepsArray;

    if (cavityMap.value(r3_cav, -1) == -1) {
        return;
    }

    from[KJ::target]      = KJ::ReagentBin;
    from[KJ::idxBinBack]  = cavityMap.value(r3_cav, -1);
    from[KJ::idxBinGroup] = reagentBinInfo.idxBinGroup;
    from[KJ::suctionMode] = regent_level_tracking_config;
    from[KJ::capacity]    = r3ComponentVolume;
    to[KJ::target]        = KJ::Load;
    add_step(re_json, KJ::PipettingNeedle, from, to);
}

void Project::One_step_reaction(ExtraInfo info, QJsonObject json, QJsonObject& re_json,
                                BinInfo reagentBinInfo, BinInfo sampleBinInfo) {
    QJsonArray  processStepsArray;
    QJsonObject from;
    QJsonObject to;
    QJsonObject reactionScheme, projectMessage, step1, FixedDilution;
    // 创建默认步骤
    QJsonObject defaultFrom;
    defaultFrom[KJ::target] = KJ::Cup;
    QJsonObject defaultTo;
    defaultTo[KJ::target] = KJ::Load;
    add_step(re_json, KJ::Gripper, defaultFrom, defaultTo);

    // get level tracking config
    auto& ini = CONTROLLER::IniSC1000::instance();
    int   regent_level_tracking_config =
        ini[CONTROLLER::KI::setting_level_tracking].toBool()
              ? (int)COM::KJ::Enum_suctionMode::trackingAndMeasurementReagent
              : (int)COM::KJ::Enum_suctionMode::liquidLevelDetector;
    // check if sample need level tracking
    int sample_level_tracking_config =
        (info.sampleTubeType == Type::SampleTubeType::MicroTube)
            ? (int)COM::KJ::Enum_suctionMode::trackingAndMeasurementSample
            : (int)COM::KJ::Enum_suctionMode::liquidLevelDetector;

    // calculation of dilution parameters
    QJsonObject baseMessage = json.value(BM::BaseMsessage).toObject();
    projectMessage          = baseMessage.value(KeyProject::ProjectMessage).toObject();
    double sampleVolume     = projectMessage.value(BM::SampleAspiration).toString().toDouble();

    FixedDilution  = json.value(FD::FixedDilution).toObject();
    projectMessage = FixedDilution.value(FD::ProjectMessage).toObject();

    QString Fixed_dilution_chamber  = projectMessage.value(FD::Fixed_dilution_chamber).toString();
    double  Fixed_dilution_multiple = projectMessage.value(FD::Fixed_dilution_multiple).toDouble();
    double  Fixed_dilution_capacity =
        projectMessage.value(FD::Fixed_dilution_capacity).toString().toDouble();

    bool needDilution = info.isNeedDiluent;

    int sample_dilute_amount = 0;
    if (needDilution) {
        // int sample_dilute_amount = (Fixed_dilution_multiple - 1.00) * sampleVolume;
        // dilute calculate

        sample_dilute_amount = dilte_calc(Fixed_dilution_multiple, sampleVolume);
        if (sample_dilute_amount <= 0) {
            // error and report
            sLog_test() << "稀释倍数设置异常，倍数过大或样本加样量过小";
        }
        // default
        sample_dilute_amount = 0;
    }

    if (needDilution) {
        QJsonArray  processPreArray;
        QJsonArray  processActualArray;
        QJsonObject preStep1;
        preStep1[KJ::actionType] = KJ::ActionMinor;
        preStep1[KJ::deviceName] = KJ::PipettingNeedle;

        QJsonObject from1;
        from1[KJ::target]      = KJ::ReagentBin;
        from1[KJ::idxBinBack]  = cavityMap.value(Fixed_dilution_chamber, -1);
        from1[KJ::idxBinGroup] = reagentBinInfo.idxBinGroup;
        from1[KJ::capacity]    = COM::microlitre_to_motorStep_PipetteNeedle(sample_dilute_amount);
        from1[KJ::suctionMode] = regent_level_tracking_config;
        preStep1[KJ::from]     = from1;

        QJsonObject to1;
        to1[KJ::target]  = KJ::Dilute;
        preStep1[KJ::to] = to1;
        processPreArray.append(preStep1);

        QJsonObject preStep2;  // todo ......
        preStep2[KJ::actionType] = KJ::ActionMinor;
        preStep2[KJ::deviceName] = KJ::PipettingNeedle;

        constexpr int SplitNum = 4;

        QJsonObject from2;
        from2[KJ::target]      = KJ::ReagentBin;
        from2[KJ::idxBinBack]  = sampleBinInfo.idxBinBack + SplitNum;
        from2[KJ::idxBinGroup] = sampleBinInfo.idxBinGroup;
        from2[KJ::capacity]    = COM::microlitre_to_motorStep_PipetteNeedle(sampleVolume);
        from2[KJ::suctionMode] = sample_level_tracking_config;
        preStep2[KJ::from]     = from2;

        QJsonObject to2;
        to2[KJ::target]  = KJ::Dilute;
        preStep2[KJ::to] = to2;
        processPreArray.append(preStep2);

        QJsonObject actualStep;
        actualStep[KJ::actionType] = KJ::ActionMinor;
        actualStep[KJ::deviceName] = KJ::PipettingNeedle;

        QJsonObject fromActual;
        fromActual[KJ::capacity] =
            COM::microlitre_to_motorStep_PipetteNeedle(Fixed_dilution_capacity);
        fromActual[KJ::target]      = KJ::Dilute;
        // may be need fix level suction on dilute pos?
        fromActual[KJ::suctionMode] = 0;
        actualStep[KJ::from]        = fromActual;

        QJsonObject toActual;
        toActual[KJ::target] = KJ::Load;
        actualStep[KJ::to]   = toActual;
        processActualArray.append(actualStep);
        add_diluteStep(re_json, KJ::Dilute, processPreArray, processActualArray);
    }

    // 创建新的 actionStep
    reactionScheme = json.value(RS::ReactionScheme).toObject();
    projectMessage = reactionScheme.value(RS::ProjectMessage).toObject();
    step1          = projectMessage.value(RS::Step1).toObject();
    //handle_suctionStep(re_json, step1, reagentBinInfo, regent_level_tracking_config);
    //2025 7 11根据试剂要求改为单吸
    handle_single_suctionStep(re_json, step1, reagentBinInfo, regent_level_tracking_config);

    if (!needDilution) {
        constexpr int SplitNum = 4;

        from[KJ::target]      = KJ::ReagentBin;
        from[KJ::idxBinBack]  = sampleBinInfo.idxBinBack + SplitNum;
        from[KJ::idxBinGroup] = sampleBinInfo.idxBinGroup;
        from[KJ::suctionMode] = 0;
        from[KJ::capacity]    = COM::microlitre_to_motorStep_PipetteNeedle(sampleVolume);
        from[KJ::suctionMode] = sample_level_tracking_config;
        to[KJ::target]        = KJ::Load;
        add_step(re_json, KJ::PipettingNeedle, from, to);
    }

    reactionScheme        = json.value(RS::ReactionScheme).toObject();
    projectMessage        = reactionScheme.value(RS::ProjectMessage).toObject();
    step1                 = projectMessage.value(RS::Step1).toObject();
    double incubationTime = step1.value(RS::TimeStep1).toString().toDouble();
    from                  = QJsonObject();
    from[KJ::target]      = KJ::Load;
    to[KJ::target]        = KJ::Incubate;
    to[KJ::sleepTime]     = incubationTime * 60 * 1000;
    add_step(re_json, KJ::Mixing, from, to);

    to               = QJsonObject();
    from[KJ::target] = KJ::Incubate;
    to[KJ::target]   = KJ::MagneticSeparation;
    add_step(re_json, KJ::Gripper, from, to);

    reactionScheme          = json.value(RS::ReactionScheme).toObject();
    QJsonObject HelpMessage = reactionScheme.value(RS::HelpMessage).toObject();
    incubationTime          = HelpMessage.value(RS::FinalIncubationTime).toString().toDouble();
    QString sample_order =HelpMessage.value(RS::sample_order).toString();
    from[KJ::target]        = KJ::MagneticSeparation;
    to[KJ::target]          = KJ::Incubate;
    to[KJ::sleepTime]       = incubationTime * 60 * 1000;
    add_step(re_json, KJ::Mixing, from, to);

    to               = QJsonObject();
    from[KJ::target] = KJ::Incubate;
    to[KJ::target]   = KJ::Photoelectric;
    add_step(re_json, KJ::Gripper, from, to);

    from[KJ::target] = KJ::Photoelectric;
    to[KJ::target]   = KJ::Throw;
    add_step(re_json, KJ::Gripper, from, to);

    qDebug()<<"一步法样本添加时机"<<HelpMessage.value(RS::sample_order).toString();
    if(sample_order == "F")
    {
        qDebug()<<"一步法样本添加时机"<<HelpMessage.value(RS::sample_order).toString();
        Move_Sample_PipetteBeforeOther(re_json);
    }

}

void Project::Delay_one_step_reaction(ExtraInfo info, QJsonObject json, QJsonObject& re_json,
                                      BinInfo reagentBinInfo, BinInfo sampleBinInfo) {
    QJsonArray  processStepsArray;
    QJsonObject from;
    QJsonObject to;
    QJsonObject reactionScheme, projectMessage, step1, step2, FixedDilution;
    // 创建默认步骤
    QJsonObject defaultFrom;
    defaultFrom[KJ::target] = KJ::Cup;
    QJsonObject defaultTo;
    defaultTo[KJ::target] = KJ::Load;
    add_step(re_json, KJ::Gripper, defaultFrom, defaultTo);

    QJsonObject baseMessage = json.value(BM::BaseMsessage).toObject();
    projectMessage          = baseMessage.value(KeyProject::ProjectMessage).toObject();
    double sampleVolume     = projectMessage.value(BM::SampleAspiration).toString().toDouble();

    // get level tracking config
    auto& ini                          = CONTROLLER::IniSC1000::instance();
    int   regent_level_tracking_config = ini[CONTROLLER::KI::setting_level_tracking].toBool();
    // check if sample need level tracking
    int sample_level_tracking_config =
        (info.sampleTubeType == Type::SampleTubeType::MicroTube)
            ? (int)COM::KJ::Enum_suctionMode::trackingAndMeasurementSample
            : (int)COM::KJ::Enum_suctionMode::liquidLevelDetector;
    FixedDilution  = json.value(FD::FixedDilution).toObject();
    projectMessage = FixedDilution.value(RS::ProjectMessage).toObject();

    QString Fixed_dilution_chamber  = projectMessage.value(FD::Fixed_dilution_chamber).toString();
    double  Fixed_dilution_multiple = projectMessage.value(FD::Fixed_dilution_multiple).toDouble();
    double  Fixed_dilution_capacity =
        projectMessage.value(FD::Fixed_dilution_capacity).toString().toDouble();
    bool needDilution         = info.isNeedDiluent;
    int  sample_dilute_amount = 0;
    if (needDilution) {
        // int sample_dilute_amount = (Fixed_dilution_multiple - 1.00) * sampleVolume;
        // dilute calculate

        sample_dilute_amount = dilte_calc(Fixed_dilution_multiple, sampleVolume);
        if (sample_dilute_amount <= 0) {
            // error and report
            sLog_test() << "稀释倍数设置异常，倍数过大或样本加样量过小";
        }
        // default
        sample_dilute_amount = 0;
    }

    if (needDilution) {
        QJsonArray  processPreArray;
        QJsonArray  processActualArray;
        QJsonObject preStep1;
        preStep1[KJ::actionType] = KJ::ActionMinor;
        preStep1[KJ::deviceName] = KJ::PipettingNeedle;

        QJsonObject from1;
        from1[KJ::target]      = KJ::ReagentBin;
        from1[KJ::idxBinBack]  = cavityMap.value(Fixed_dilution_chamber, -1);
        from1[KJ::idxBinGroup] = reagentBinInfo.idxBinGroup;
        from1[KJ::capacity]    = COM::microlitre_to_motorStep_PipetteNeedle(sample_dilute_amount);
        from1[KJ::suctionMode] = regent_level_tracking_config;
        preStep1[KJ::from]     = from1;

        QJsonObject to1;
        to1[KJ::target]  = KJ::Dilute;
        preStep1[KJ::to] = to1;
        processPreArray.append(preStep1);

        QJsonObject preStep2;  // todo ......
        preStep2[KJ::actionType] = KJ::ActionMinor;
        preStep2[KJ::deviceName] = KJ::PipettingNeedle;

        constexpr int SplitNum = 4;

        QJsonObject from2;
        from2[KJ::target]      = KJ::ReagentBin;
        from2[KJ::idxBinBack]  = sampleBinInfo.idxBinBack + SplitNum;
        from2[KJ::idxBinGroup] = sampleBinInfo.idxBinGroup;
        from2[KJ::capacity]    = COM::microlitre_to_motorStep_PipetteNeedle(sampleVolume);
        from2[KJ::suctionMode] = sample_level_tracking_config;
        preStep2[KJ::from]     = from2;

        QJsonObject to2;
        to2[KJ::target]  = KJ::Dilute;
        preStep2[KJ::to] = to2;
        processPreArray.append(preStep2);

        QJsonObject actualStep;
        actualStep[KJ::actionType] = KJ::ActionMinor;
        actualStep[KJ::deviceName] = KJ::PipettingNeedle;
        actualStep[KJ::capacity] =
            COM::microlitre_to_motorStep_PipetteNeedle(Fixed_dilution_capacity);
        QJsonObject fromActual;
        fromActual[KJ::target]      = KJ::Dilute;
        // may be need fix level suction on dilute pos?
        fromActual[KJ::suctionMode] = 0;
        actualStep[KJ::from]        = fromActual;

        QJsonObject toActual;
        toActual[KJ::target] = KJ::Load;
        actualStep[KJ::to]   = toActual;
        processActualArray.append(actualStep);
        add_diluteStep(re_json, KJ::Dilute, processPreArray, processActualArray);
    }

    // 创建新的 actionStep
    reactionScheme = json.value(RS::ReactionScheme).toObject();
    projectMessage = reactionScheme.value(RS::ProjectMessage).toObject();
    step1          = projectMessage.value(RS::Step1).toObject();
    //handle_suctionStep(re_json, step1, reagentBinInfo, regent_level_tracking_config);
    handle_single_suctionStep(re_json, step1, reagentBinInfo, regent_level_tracking_config);
    if (!needDilution) {
        constexpr int SplitNum = 4;

        from[KJ::target]      = KJ::ReagentBin;
        from[KJ::idxBinBack]  = sampleBinInfo.idxBinBack + SplitNum;
        from[KJ::idxBinGroup] = sampleBinInfo.idxBinGroup;
        from[KJ::suctionMode] = sample_level_tracking_config;
        from[KJ::capacity]    = COM::microlitre_to_motorStep_PipetteNeedle(sampleVolume);
        to[KJ::target]        = KJ::Load;
        add_step(re_json, KJ::PipettingNeedle, from, to);
    }

    reactionScheme        = json.value(RS::ReactionScheme).toObject();
    projectMessage        = reactionScheme.value(RS::ProjectMessage).toObject();
    step1                 = projectMessage.value(RS::Step1).toObject();
    double incubationTime = step1.value(RS::TimeStep1).toString().toDouble();
    from                  = QJsonObject();
    from[KJ::target]      = KJ::Load;
    to[KJ::target]        = KJ::Incubate;
    to[KJ::sleepTime]     = incubationTime * 60 * 1000;  // 改为延时一步法的延时时间
    add_step(re_json, KJ::Mixing, from, to);

    to               = QJsonObject();
    from[KJ::target] = KJ::Incubate;
    to[KJ::target]   = KJ::Load;
    add_step(re_json, KJ::Gripper, from, to);  // 夹回到Load位置

    // 加最后一个液体
    step2 = projectMessage.value(RS::Step2).toObject();
    handle_suctionStep(re_json, step2, reagentBinInfo, regent_level_tracking_config);

    // 再次混匀
    incubationTime    = step2.value(RS::TimeStep2).toString().toDouble();
    from              = QJsonObject();
    from[KJ::target]  = KJ::Load;
    to[KJ::target]    = KJ::Incubate;
    to[KJ::sleepTime] = incubationTime * 60 * 1000;  // 孵育时间
    add_step(re_json, KJ::Mixing, from, to);

    to               = QJsonObject();
    from[KJ::target] = KJ::Incubate;
    to[KJ::target]   = KJ::MagneticSeparation;
    add_step(re_json, KJ::Gripper, from, to);

    reactionScheme          = json.value(RS::ReactionScheme).toObject();
    QJsonObject HelpMessage = reactionScheme.value(RS::HelpMessage).toObject();
    QString sample_order =HelpMessage.value(RS::sample_order).toString();
    from[KJ::target]        = KJ::MagneticSeparation;
    to[KJ::target]          = KJ::Incubate;
    to[KJ::sleepTime]       = incubationTime * 60 * 1000;
    add_step(re_json, KJ::Mixing, from, to);

    to               = QJsonObject();
    from[KJ::target] = KJ::Incubate;
    to[KJ::target]   = KJ::Photoelectric;
    add_step(re_json, KJ::Gripper, from, to);

    from[KJ::target] = KJ::Photoelectric;
    to[KJ::target]   = KJ::Throw;
    add_step(re_json, KJ::Gripper, from, to);

    incubationTime          = HelpMessage.value(RS::FinalIncubationTime).toString().toDouble();
    qDebug()<<"样本添加时机"<<HelpMessage.value(RS::sample_order).toString();
    if(sample_order == "F")
        Move_Sample_PipetteBeforeOther(re_json);
}

// return : ret<0 ,error convert ; Otherwise, it indicates the amount of diluent added.
int Project::dilte_calc(double dilution_multiple, int sample_volume) {
    int ret                     = 0;
    // Maximum capacity of reaction tube
    const int tube_max_capacity = 700;
    // Minimum filling capacity of the sample needle
    const int needle_minmum_cap = 2;
    // define sample fill capacity
    int dilution_sample_cap     = sample_volume;

    // check dilution_multiple
    if (dilution_multiple < 1) {
        // error par
        ret = -1;
        return ret;
    }

    // check if capacity out of tube maximum capacity
    if (dilution_multiple * dilution_sample_cap > tube_max_capacity) {
        // Calculate new sample fill capacity
        dilution_sample_cap = tube_max_capacity / dilution_multiple;
    }
    // check if dilution_sample_cap < needle minimum capacity
    if (dilution_sample_cap < needle_minmum_cap) {
        // error
        ret = -1;
    } else {
        ret = dilution_sample_cap * (dilution_multiple - 1);
    }
    return ret;
}
}  // namespace CONTROLLER
