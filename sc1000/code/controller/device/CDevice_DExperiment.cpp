#include "CDevice_DExperiment.h"

#include <QDebug>
#include <QList>

#include "../CONTROLLER_Calibration.h"
#include "../CONTROLLER_ExpCalibration.h"
#include "../CONTROLLER_ExpQuality.h"
#include "../CONTROLLER_Project.h"
#include "../CONTROLLER_Reagent.h"
#include "../CONTROLLER_Sample.h"
#include "../CONTROLLER_SampleApply.h"
#include "../CONTROLLER_UpToLower.h"
#include "communication/controller/DispatchController.h"
#include "communication/controller/Parse.h"
#include "controller/CONTROLLER_Config.h"
#include "lis/Lis_SC1000.h"
#include "lte/lte_lbs.h"
#include "utility/UTILITY_Hash.h"
#include "widget/window/history/WIDGET_HistorySample.h"

namespace CONTROLLER::CDevice {

void DExperiment::Strat() {
    qDebug() << __PRETTY_FUNCTION__;

    using namespace COM;
    auto& taskCU = DispatchController::instance();
    qWarning() << ">>>[begin]" << taskCU.to_string();

    analyse_expSample();
    analyse_expCalibration();
    analyse_expQuality();

    taskCU.Start_dispatch();
    qWarning() << ">>>[end]" << taskCU.to_string();
}

void DExperiment::Pause() {
    qDebug() << __PRETTY_FUNCTION__;
    using namespace COM;
    auto& taskCU = DispatchController::instance();
    taskCU.Pause_dispatch();
}

void DExperiment::Stop() {
    qDebug() << __PRETTY_FUNCTION__;
    using namespace COM;
    auto& taskCU = DispatchController::instance();
    taskCU.Stop_dispatch();
}

void DExperiment::analyse_expSample() {
    auto& up_control = UpToLower::instance();
    using namespace COM;
    auto& taskCU = DispatchController::instance();

    auto reaSet   = Reagent::instance().Select_isLoadList();
    auto applySet = SampleApply::instance().Select_all_fromDB();
    while (applySet.size()) {
        auto apply = applySet.back();
        applySet.pop_back();
        auto sampleItem = apply->Get_relatedSample();
        apply->Delete_toDB();
        if (sampleItem == nullptr) {
            continue;
        }

        for (auto reagentItem : reaSet) {
            Project::ExtraInfo sampleInfo;
            sampleInfo.project        = reagentItem->table[Reagent::entity_attr::project];
            sampleInfo.isEmergency    = sampleItem->table[Sample::entity_attr::emergency_sign];
            sampleInfo.sampleTubeType = [sampleItem]() -> Type::SampleTubeType {
                auto data    = sampleItem->table[Sample::entity_attr::sampleTube];
                auto valList = Type::TypeList_of_tube_type();
                int  ret =
                    std::find(std::begin(valList), std::end(valList), data) - std::begin(valList);
                return (Type::SampleTubeType)ret;
            }();

            sampleInfo.expType = Project::ExpType::FromSample;
            if (apply->Get_projectList().contains(sampleInfo.project) == false) {
                continue;
            }
            Project::BinInfo reaInfo = Project::Create_reagentBinInfo(
                reagentItem->table[Reagent::entity_attr::location], reagentItem);
            Project::BinInfo samInfo =
                Project::Create_sampleBinInfo(sampleItem->table[Sample::entity_attr::location]);
            QJsonObject json = Project::Merge_FlowAndLocation(sampleInfo, reaInfo, samInfo);
            if (json.empty()) {
                continue;
            }

            auto expItem  = ExpSample::Create_oneEntityItem();
            expItem->json = json;
            expItem->Set_id_aboutReagentAndSample(reagentItem->id, sampleItem->id);
            expItem->table[ExpSample::entity_attr::project] = sampleInfo.project;
            expItem->table[ExpSample::entity_attr::type] =
                QString::number((int)ExpSample::entity_type::ExpType::N_Normal);

            auto hash = UTILITY::get_hash(expItem->Get_hash(), qHash(json));
            auto task = taskCU.Append_taskPackage(hash, expItem->json);
            if (task == nullptr) {
                qDebug() << ">>>task generate error";
                continue;
            }
            expItem->Insert_toDB();
            sampleItem->table[Sample::entity_attr::state] =
                QString::number((int)Sample::entity_type::TubeState::Testing);
            sampleItem->Append_expId(expItem->Get_idString());
            sampleItem->Update_toDB();
            reagentItem->Add_measureCnt();
            reagentItem->Update_toDB();
            up_control.Add_id(reagentItem->Get_idString());

            QObject::connect(task.get(), &TaskPackage::signal_valueReceived,
                             [expItem](const int val) {
                                 expItem->Set_expData(val);
                                 expItem->Update_toDB();
                                 UpToLower::instance().Remove_id(
                                     expItem->table[ExpSample::entity_attr::idReagent]);
                                 qDebug() << QString("实验数据存储完毕：%1").arg(val);

                                 LTE_LBS::instance().Send_Data(expItem);
                                 LIS::Lis_SC1000::Send_ExpData(expItem, false);
                                 CONTROLLER::Config::Network network;
                                 if (network.lisConfig.autoUpload == 1) {
                                     LIS::Lis_SC1000::Send_ExpData_By_Net(expItem);
                                 }

                             });
            QObject::connect(task.get(), &TaskPackage::signal_waring,
                             [expItem](const QString& msg) {
                                 expItem->table[ExpSample::entity_attr::state] =
                                     QString::number((int)Type::ETubeState::UnKnown);
                                 expItem->Update_toDB();
                             });
            QObject::connect(
                task.get(), &TaskPackage::signal_pipetteNeedleSuction,
                [reagentItem](const int idxBinGroup, const int idxBinBack, const int val) {
                    reagentItem->set_liquidConsume(idxBinBack, val);
                    reagentItem->Update_toDB();
                    qDebug()
                        << QString("试剂消耗更新：Hole: %1, Value: %2").arg(idxBinBack).arg(val);
                });
        }
    }
}

/**
 * 解析校准品的实验
 */
void DExperiment::analyse_expCalibration() {
    using namespace COM;
    auto& taskCU     = DispatchController::instance();
    auto& up_control = UpToLower::instance();
    auto  expCalSet  = ExpCalibration::instance().Get_theList_expNotDone();

    for (auto expCal : expCalSet) {
        expCal->table[ExpCalibration::entity_attr::state] =
            QString::number((int)Type::ETubeState::Testing);
        expCal->Builder();
        expCal->Update_toDB();

        auto reagentItem = expCal->Get_relatedReagent();
        auto calItem     = expCal->Get_relatedCalibration();
        if (reagentItem == nullptr || calItem == nullptr) {
            continue;
        }

        reagentItem->table[ENTITY::EReagent::attr::idExpCal] = expCal->Get_idString();
        reagentItem->Add_measureCnt();
        reagentItem->Update_toDB();

        Project::BinInfo reaInfo = Project::Create_reagentBinInfo(
            reagentItem->table[Reagent::entity_attr::location], reagentItem);
        Project::ExtraInfo extraInfo;
        extraInfo.expType = Project::ExpType::FromCalibration;
        extraInfo.project = expCal->table[ExpCalibration::entity_attr::project];

        auto locationList = expCal->Get_c0n_LocationList();
        for (int location = 0; location < locationList.size(); location += 1) {
            Project::BinInfo calInfo       = Project::Create_sampleBinInfo(locationList[location]);
            // get experiment type
            // get calibration experiment is need inherit diluent
            auto      qrcodeCal            = calItem->Get_qrcodeEntity();
            const int repetitionsCount     = qrcodeCal[QRCODE::KeyCal::repetitions].toInt();
            extraInfo.isCaliInheritDiluent = !!(qrcodeCal[QRCODE::KeyCal::dilution].toInt());
            QJsonObject json = Project::Merge_FlowAndLocation(extraInfo, reaInfo, calInfo);
            if (json.empty()) {
                continue;
            }

            for (int cnt = 0; cnt < repetitionsCount; cnt += 1) {
                auto hash = UTILITY::get_hash(expCal->Get_hash(), qHash(json) + cnt);
                auto task = taskCU.Append_taskPackage(hash, json);
                if (task == nullptr) {
                    continue;
                }

                up_control.Add_id(reagentItem->Get_idString());
                QObject::connect(task.get(), &TaskPackage::signal_valueReceived,
                                 [expCal, location, cnt](const int val) {
                                     expCal->Set_expData(val, location, cnt);
                                     expCal->Update_toDB();
                                     UpToLower::instance().Remove_id(
                                         expCal->table[ExpCalibration::entity_attr::idReagent]);
                                 });
                QObject::connect(task.get(), &TaskPackage::signal_waring,
                                 [expCal](const QString& msg) {
                                     expCal->table[ExpCalibration::entity_attr::state] =
                                         QString::number((int)Type::ETubeState::UnKnown);
                                     expCal->Update_toDB();
                                 });
                QObject::connect(
                    task.get(), &TaskPackage::signal_pipetteNeedleSuction,
                    [reagentItem](const int idxBinGroup, const int idxBinBack, const int val) {
                        reagentItem->set_liquidConsume(idxBinBack, val);
                        reagentItem->Update_toDB();
                        qDebug() << QString("试剂消耗更新：Hole: %1, Value: %2")
                                        .arg(idxBinBack)
                                        .arg(val);
                    });
            }  // test-repetitions
        }  // c0c1c2c3-location
    }
}

/**
 * 解析质控的实验
 */
void DExperiment::analyse_expQuality() {
    using namespace COM;
    auto& taskCU     = DispatchController::instance();
    auto& up_control = UpToLower::instance();
    auto  expQuaSet  = ExpQuality::instance().Get_theList_expNotDone();

    for (auto expQua : expQuaSet) {
        auto reagentItem = expQua->Get_relatedReagent();
        if (reagentItem == nullptr) {
            continue;
        }
        Project::BinInfo reaInfo = Project::Create_reagentBinInfo(
            reagentItem->table[Reagent::entity_attr::location], reagentItem);

        QString          quaLocation = expQua->table[ExpQuality::entity_attr::location];
        Project::BinInfo quaInfo     = Project::Create_sampleBinInfo(quaLocation);

        Project::ExtraInfo sampleInfo;
        sampleInfo.project = expQua->table[ExpQuality::entity_attr::project];
        sampleInfo.expType = Project::ExpType::FromQuality;
        QJsonObject json   = Project::Merge_FlowAndLocation(sampleInfo, reaInfo, quaInfo);
        if (json.empty()) {
            continue;
        }
        auto task = taskCU.Append_taskPackage(expQua->Get_hash(), json);
        if (task == nullptr) {
            continue;
        }
        expQua->table[ExpQuality::entity_attr::state] =
            QString::number((int)Type::ETubeState::Testing);
        expQua->Update_toDB();

        reagentItem->Add_measureCnt();
        reagentItem->Update_toDB();
        up_control.Add_id(reagentItem->Get_idString());
        QObject::connect(task.get(), &TaskPackage::signal_valueReceived, [expQua](const int val) {
            expQua->Set_expData(val);
            expQua->Update_toDB();
            UpToLower::instance().Remove_id(expQua->table[ExpQuality::entity_attr::idReagent]);
            qDebug() << QString("校准数据存储完毕：%1").arg(val);
        });
        QObject::connect(task.get(), &TaskPackage::signal_waring, [expQua](const QString& msg) {
            expQua->table[ExpQuality::entity_attr::state] =
                QString::number((int)Type::ETubeState::UnKnown);
            expQua->Update_toDB();
        });
        QObject::connect(
            task.get(), &TaskPackage::signal_pipetteNeedleSuction,
            [reagentItem](const int idxBinGroup, const int idxBinBack, const int val) {
                reagentItem->set_liquidConsume(idxBinBack, val);
                reagentItem->Update_toDB();
                qDebug() << QString("试剂消耗更新：Hole: %1, Value: %2").arg(idxBinBack).arg(val);
            });
    }
}

}  // namespace CONTROLLER::CDevice
