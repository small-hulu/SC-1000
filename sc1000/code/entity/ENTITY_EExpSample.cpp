#include "ENTITY_EExpSample.h"

#include "algorithm/algorithm.hpp"
#include "controller/CONTROLLER_ExpCalibration.h"
#include "controller/CONTROLLER_Project.h"
#include "controller/CONTROLLER_Sample.h"
#include "global/GLOBAL_KeyProject.h"

namespace OM = KeyProject::OtherMessage;
namespace ENTITY {
/**
 * 分两种创建
 * 1. 新实验的创建
 * 2. 从数据库中读出数据
 */
EExpSample::EExpSample() {
    /// 2会把1覆盖
    table[attr::timeStart] = QDateTime::currentDateTime().toString(time_format);
}

/**
 * 实验合法需要满足三个条件
 * 1. 本身这条信息是合法的
 * 2. 对应试剂编号合法
 * 3. 对应样本编号合法
 */
bool EExpSample::Is_valid() {
    if (id == 0) {
        return false;
    }
    if (table[attr::idSample].toInt() == 0) {
        return false;
    }
    if (table[attr::idReagent].toInt() == 0) {
        return false;
    }
    return true;
}

bool EExpSample::Is_inRangeTime(QDateTime startTime, QDateTime endTime) {
    if (!(startTime <= endTime)) {
        return false;
    }
    QDateTime start = QDateTime::fromString(table[attr::timeStart], time_format);
    QDateTime end   = QDateTime::fromString(table[attr::timeEnd], time_format);
    return (startTime <= start) && (end <= endTime);
}

void EExpSample::Set_id_aboutReagentAndSample(int idReagent, int idSample) {
    table[attr::idReagent] = QString("%1").arg(idReagent);
    table[attr::idSample]  = QString("%1").arg(idSample);
}

void EExpSample::Set_expData(int data) {
    qDebug() << "实验结果存储, 实验id:" << Get_idString() << " data:" << data;
    table[attr::timeEnd] = QDateTime::currentDateTime().toString(time_format);
    table[attr::expData] = QString("%1").arg(data);
    auto sampleId        = table[attr::idSample];
    auto sample          = CONTROLLER::Sample::instance().Select_byId_fromDB(sampleId);
    if (sample) {
        sample->Save_expValue_ById(Get_idString(), data);
        sample->Update_toDB();
    }

    double val = Get_Concentration();
    qDebug()<<val<<endl;


    if (val < 0) {
        table[attr::resultData] = QString("计算异常");
    } else {
        //处理结果补偿
        namespace OM                      = KeyProject::OtherMessage;
        auto          fileCU              = CONTROLLER::Create_ProjectsFolderController();
        QJsonObject   json                = fileCU.Get_projectJson(table[attr::project]);
        QJsonObject   OtherMessage        = json.value(OM::OtherMessage).toObject();
        QJsonObject   projectMessage      = OtherMessage.value(OM::ProjectMessage).toObject();
        const QString Result_Compensate   = projectMessage.value(OM::Result_Compensate).toString();
        if(!Result_Compensate.isEmpty())
        {
            ///val*=Result_Compensate
            double res = Result_Compensate.toDouble();
            val*=res;
        }
        qDebug()<<val<<endl;
        if (table[attr::project] == "25-OH-VD") {
            QString ret             = result_Format(val);
            table[attr::resultData] = ret;
        }
        else if (table[attr::project] == "hs-cTnI" && val <= 0) {
            qDebug() << "hs-cTnI处理" << endl;
            table[attr::resultData] = "<0.002";
        }
        else {
            table[attr::resultData] = QString("%1").arg(val);
        }

    }
}

QString EExpSample::result_Format(double val) {
    using namespace CONTROLLER;
    auto    fileCU = Create_ProjectsFolderController();
    QString path   = fileCU.Get_projectPath("25-OH-VD");
    QFile   file(path);

    if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
        qWarning() << QString("File[%1] exist[%2] open[%3]")
                          .arg(path)
                          .arg(file.exists())
                          .arg(file.isOpen());
        return {};
    }

    QJsonObject json = QJsonDocument::fromJson(file.readAll()).object();
    if (json.isEmpty()) {
        qWarning() << "JSON 文件解析失败";
        return {};
    }
    QJsonObject   OtherMessage       = json.value(OM::OtherMessage).toObject();
    QJsonObject   projectMessage     = OtherMessage.value(OM::ProjectMessage).toObject();
    const QString Result_Calculation = projectMessage.value(OM::Result_Calculation).toString();

    double  res10 = val * 0.10;
    double  res90 = val * 0.90;
    QString TripletResult =
        QString("%1;%2;%3").arg(val).arg(res10, 0, 'f', 2).arg(res90, 0, 'f', 2);
    return TripletResult;
}

int EExpSample::Get_expData() {
    return table[attr::expData].toInt();
}

double EExpSample::Get_Concentration() {
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
    const int odata = table[attr::expData].toInt();

    /// 试剂中存储的 4plc
    FourPLC plc;
    plc.A = reagentVal[reagent_attr::A].toDouble();
    plc.B = reagentVal[reagent_attr::B].toDouble();
    plc.C = reagentVal[reagent_attr::C].toDouble();
    plc.D = reagentVal[reagent_attr::D].toDouble();

    auto rea = CONTROLLER::Reagent::instance().Select_byId_fromDB(idReagent);
    if (rea) {
        //目前还是在用最新的校准实验，后面把激活功能添加后，在从库中取激活的曲线去出值
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

QPair<int, QString> EExpSample::Judge_scope() {
    QString             val = table[attr::resultData];
    QPair<int, QString> ans;
    ans.first  = 0;
    ans.second = val;

    auto reagent = Get_relatedReagent();
    if (reagent == nullptr) {
        return ans;
    }

    auto qrcode   = reagent->Get_qrcodeEntity();
    namespace key = QRCODE::KeyRea;
    auto low      = qrcode[key::project_curvestartcon].toString();
    auto high     = qrcode[key::project_curveendcon].toString();
    if (val.toDouble() < low.toDouble()) {
        ans.second = "<" + low;
    } else if (val.toDouble() > high.toDouble()) {
        ans.second = ">" + high;
    }
    return ans;
}
}  // namespace ENTITY
