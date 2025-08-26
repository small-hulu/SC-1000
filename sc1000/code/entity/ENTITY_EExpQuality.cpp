#include "ENTITY_EExpQuality.h"

#include "controller/CONTROLLER_ExpCalibration.h"

namespace ENTITY {
EExpQuality::EExpQuality() {
    table[attr::timeStart] = QDateTime::currentDateTime().toString(time_format);
}

CONTROLLER::Reagent::entity_item EExpQuality::Get_relatedReagent() {
    QString calId = table[attr::idReagent];
    return CONTROLLER::Reagent::instance().Select_byId_fromDB(calId);
}

CONTROLLER::Quality::entity_item EExpQuality::Get_relatedQuality() {
    QString calId = table[attr::idQuality];
    return CONTROLLER::Quality::instance().Select_byId_fromDB(calId);
}

void EExpQuality::Set_expData(int data) {
    table[attr::lightVal]  = QString("%1").arg(data);
    table[attr::timeEnd]   = QDateTime::currentDateTime().toString(time_format);
    table[attr::state]     = QString::number((int)ETubeState::Complete);
    table[attr::valResult] = QString::number(Get_Concentration());
}

double EExpQuality::Get_Concentration() {
    auto idReagent = table[attr::idReagent];
    auto reagentVal =
        DB::SqlExe::Select_oneLine_byId(::DB::TableEntity::reagent{}, idReagent.toInt());
    if (reagentVal.size() == 0) {
        qWarning() << QString("[id=%1]ExpSam search Reagent Failed [idReagent=%2]")
                          .arg(Get_idString())
                          .arg(idReagent);
        return -1;
    }
    // 去掉id
    reagentVal.pop_front();
    using reagent_attr = ::DB::TableEntity::reagent::table_attribute;
    if (reagentVal.size() != reagent_attr::Enum_Count) {
        qWarning() << QString("[id=%1]ExpSam search Reagent Error [idReagent=%2]")
                          .arg(Get_idString())
                          .arg(idReagent);
        return -1;
    }

    /// 原始数据
    const int odata = table[attr::lightVal].toInt();

    /// 试剂中存储的 4plc
    FourPLC plc;
    plc.A = reagentVal[reagent_attr::A].toDouble();
    plc.B = reagentVal[reagent_attr::B].toDouble();
    plc.C = reagentVal[reagent_attr::C].toDouble();
    plc.D = reagentVal[reagent_attr::D].toDouble();

    auto rea = CONTROLLER::Reagent::instance().Select_byId_fromDB(idReagent);
    if (rea) {
        auto expCal = CONTROLLER::ExpCalibration::instance().Select_byId_fromDB(
            rea->table[EReagent::attr::idExpCal]);
        if (expCal) {
            auto cal = expCal->Get_relatedCalibration();
            if (cal) {
                auto           c0nLight = expCal->Get_c0n_avgLightValList();
                auto           c0nVal   = cal->Get_c0n_ConcentrationList();
                qDebug() << "c0nLight:";
                for (int i = 0; i < c0nLight.size(); ++i) {
                    qDebug() << QString("  [%1] = %2").arg(i).arg(c0nLight[i]);
                }

                qDebug() << "c0nVal:";
                for (int i = 0; i < c0nVal.size(); ++i) {
                    qDebug() << QString("  [%1] = %2").arg(i).arg(c0nVal[i]);
                }
                FourPLCWithCal plcCal;
                plcCal.fourplc = plc;
                for (int i = 0; i < qMin(c0nVal.size(), c0nLight.size()); i += 1) {
                    plcCal.pointList.push_back({c0nLight[i].toDouble(), c0nVal[i]});
                }
                //得到拟合后的新四参数
                auto newPlc = Adjust_AD_FromPointList(plcCal.pointList, plcCal.fourplc);
                plcCal.fitting_fourplc = newPlc;
                qDebug()<<"拟合后新四参数:"<<QString::fromStdString(newPlc.to_string())<<endl;
                auto ret = alg4plc_rlu_to_concentration(plcCal.fitting_fourplc,odata);
                //auto ret = alg4plc_rlu_to_concentration_withCal(plcCal, odata);
                if (ret.is_valid) {
                    return qMax(0.0, ret.value);
                } else {
                    qDebug() << "定标后计算失败";
                    return 0;
                }
            }
        }
    }

    /// 没有校准过的，直接用原始的4plc来处理
    auto ret = alg4plc_rlu_to_concentration(plc, odata);
    if (ret.is_valid) {
        return qMax(0.0, ret.value);
    } else {
        qDebug() << "未定标后计算失败";
        return 0;
    }
}

bool EExpQuality::Is_qualified() {
    const double target        = table[attr::valTarget].toDouble();
    const double sd            = table[attr::valSD].toDouble();
    const double concentration = table[attr::valResult].toDouble();
    return concentration >= (target - sd) && concentration <= (target + sd);
}
}  // namespace ENTITY
