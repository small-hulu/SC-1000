#include "lte_sc.h"

#include "controller/CONTROLLER_Config.h"
#include "controller/CONTROLLER_Project.h"
#include "controller/IniSC1000.h"
#include "lte/lte.h"

void LTE_SC::Send_SC_ErrorCode(ErrorCode code) {
    auto           lte_controler = LTE_ML302::get_instance();
    QList<QString> msgList;
    msgList << "PMD:Log";
    msgList << lte_controler->getEquipment_number();
    msgList << QString("SC:%1").arg(QMetaEnum::fromType<ErrorCode>().key((int)code));
    msgList << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    qInfo() << "Send-Log" << msgList.join(",");
    lte_controler->send_text_to_tcp(msgList.join(","));
}

void LTE_SC::Send_SC_Reply(QString data,QString MD5)
{
    auto           lte_controler = LTE_ML302::get_instance();
    QList<QString> msgList;
    msgList << "PMD:Control_Reply";
    msgList << data;
    msgList << MD5;
    lte_controler->send_text_to_tcp(msgList.join(","));
}


void LTE_SC::Send_SC_Data(std::shared_ptr<ENTITY::EExpSample> expItem) {
    if (expItem == nullptr) {
        return;
    }
    using attr = ENTITY::EExpSample::attr;

    auto lte_controler = LTE_ML302::get_instance();

    QList<QString> msgList;
    msgList << "PMD:SC_Data";
    msgList << lte_controler->getEquipment_number();
    msgList << expItem->table[attr::project];

    auto reagent = expItem->Get_relatedReagent();
    if (reagent) {
        msgList << reagent->table[ENTITY::EReagent::attr::batchNum];
    } else {
        msgList << QString("");
    }

    auto sample = expItem->Get_relatedSample();
    if (sample) {
        msgList << sample->table[CONTROLLER::Sample::entity_attr::location];
    } else {
        msgList << QString("");
    }

    msgList << expItem->table[attr::timeStart];
    msgList << expItem->table[attr::timeEnd];
    msgList << expItem->table[attr::resultData];

    namespace OM                     = KeyProject::OtherMessage;
    auto          fileCU             = CONTROLLER::Create_ProjectsFolderController();
    QJsonObject   json               = fileCU.Get_projectJson(expItem->table[attr::project]);
    QJsonObject   OtherMessage       = json.value(OM::OtherMessage).toObject();
    QJsonObject   projectMessage     = OtherMessage.value(OM::ProjectMessage).toObject();
    const QString Default_Unit       = projectMessage.value(OM::Default_Unit).toString();
    const QString Reference_range    = projectMessage.value(OM::Reference_range).toString();
    const QString Result_Calculation = projectMessage.value(OM::Result_Calculation).toString();

    msgList << Default_Unit;
    msgList << "参考范围见项目说明书";
    msgList << expItem->table[attr::expData];
    if (expItem->table[attr::state].toInt() == (int)Type::ETubeState::UnKnown) {
        msgList << "异常";
    } else {
        msgList << "正常";
    }

    qInfo() << "Send-Message" << msgList.join(",");

    lte_controler->send_text_to_tcp(msgList.join(","));
}

bool LTE_SC::Init(const QString &vid, const QString &pid) {
    QString deviceNum =
        (CONTROLLER::IniSC1000::instance()[CONTROLLER::KI::machine_number].toString());
    auto lte_controler = LTE_ML302::get_instance(deviceNum);
    lte_controler->open_debug();
    if (lte_controler->open_serial_port(vid, pid)) {
        CONTROLLER::Config::Hospital hospital;

        QList<QString> msgList;
        msgList << "PMD:SC_LBS";
        msgList << lte_controler->getEquipment_number();
        msgList << lte_controler->get_coordinates();
        msgList << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        msgList << lte_controler->get_cimi();
        msgList << lte_controler->get_csq();
        msgList << hospital.hospitalName;
        msgList << hospital.departmentName;
        msgList << ("V" + qApp->applicationVersion() +
                    QDateTime::currentDateTime().toString(" yyyy.MM.dd"));

        //qInfo() << msgList;
        //qInfo() << msgList.join(",");

        if(lte_controler->open_tcp_connect("8.131.87.42", "19965"))
        {
            lte_controler->send_connect();
            lte_controler->send_heartbeat();
        }
        return lte_controler->send_text_to_tcp(msgList.join(",")).getStatus();
    }
    qWarning() << "4G模块连接失败！";
    return false;
}
