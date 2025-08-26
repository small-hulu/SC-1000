#include "Lis_SC1000.h"

#include "controller/CONTROLLER_Config.h"
#include "controller/CONTROLLER_Project.h"

namespace LIS {

void Lis_SC1000::send_agent(const QString &data, bool isForce) {
    CONTROLLER::Config::Network network;
    if (!isForce && network.lisConfig.autoUpload == 0) {
        return;
    }
    qDebug()<<"lis"<<data<<endl;
    Lis::instance().Write_bySequenced(data.toUtf8());
}

void Lis_SC1000::Set_defaultInfo(SerialBase::ComInfo &comInfo) {
    comInfo.BaudRate    = 115200;
    comInfo.DataBits    = QSerialPort::Data8;
    comInfo.Checkbit    = QSerialPort::NoParity;
    comInfo.Stopbit     = QSerialPort::OneStop;
    comInfo.FlowControl = QSerialPort::NoFlowControl;

    CONTROLLER::Config::Network network;
    comInfo.port_name = network.serialConfig.serialPort;
}

void Lis_SC1000::Send_ExpData(CONTROLLER::ExpSample::entity_item entity, bool isForce) {
    using attr = CONTROLLER::ExpSample::entity_item::element_type::attr;

    QList<QString> msgList;
    msgList << "pumaide";

    QDateTime time = QDateTime::fromString(entity->table[attr::timeEnd], entity->time_format);
    msgList << time.date().toString("yyyy.MM.dd");
    msgList << time.time().toString();

    auto sample = entity->Get_relatedSample();
    if (sample) {
        msgList << sample->table[ENTITY::ESample::attr::number];
        msgList << sample->table[ENTITY::ESample::attr::patient_name];
    } else {
        msgList << "";
        msgList << "";
    }
    msgList << entity->table[attr::project];
    msgList << entity->table[attr::resultData];

    namespace OM                      = KeyProject::OtherMessage;
    auto          fileCU              = CONTROLLER::Create_ProjectsFolderController();
    QJsonObject   json                = fileCU.Get_projectJson(entity->table[attr::project]);
    QJsonObject   OtherMessage        = json.value(OM::OtherMessage).toObject();
    QJsonObject   projectMessage      = OtherMessage.value(OM::ProjectMessage).toObject();
    const QString Default_Unit        = projectMessage.value(OM::Default_Unit).toString();
    const QString Reference_range     = projectMessage.value(OM::Reference_range).toString();
    const QString Result_Calculation  = projectMessage.value(OM::Result_Calculation).toString();
    msgList.back()                   += Default_Unit;

    send_agent(msgList.join(",") + "\n", isForce);
}
void Lis_SC1000::Send_ExpData_By_Net(CONTROLLER::ExpSample::entity_item entity) {
    using attr = CONTROLLER::ExpSample::entity_item::element_type::attr;

    QList<QString> msgList;
    msgList << "pumaide";

    QDateTime time = QDateTime::fromString(entity->table[attr::timeEnd], entity->time_format);
    msgList << time.date().toString("yyyy.MM.dd");
    msgList << time.time().toString();

    auto sample = entity->Get_relatedSample();
    if (sample) {
        msgList << sample->table[ENTITY::ESample::attr::number];
        msgList << sample->table[ENTITY::ESample::attr::patient_name];
    } else {
        msgList << "";
        msgList << "";
    }
    msgList << entity->table[attr::project];
    msgList << entity->table[attr::resultData];

    namespace OM                      = KeyProject::OtherMessage;
    auto          fileCU              = CONTROLLER::Create_ProjectsFolderController();
    QJsonObject   json                = fileCU.Get_projectJson(entity->table[attr::project]);
    QJsonObject   OtherMessage        = json.value(OM::OtherMessage).toObject();
    QJsonObject   projectMessage      = OtherMessage.value(OM::ProjectMessage).toObject();
    const QString Default_Unit        = projectMessage.value(OM::Default_Unit).toString();
    const QString Reference_range     = projectMessage.value(OM::Reference_range).toString();
    const QString Result_Calculation  = projectMessage.value(OM::Result_Calculation).toString();
    msgList.back()
            += Default_Unit;

    auto& conn = Lis_net_con::instance();
    if (conn.isConnected()) {
        QString msg = msgList.join(",") + "\n";
        QTextCodec* codec = QTextCodec::codecForName("GBK");
        conn.sendData(codec->fromUnicode(msg));
    }
}

void Lis_SC1000::Send_singleMessage(const QString &data, bool isForce) {
    QList<QString> msgList;
    msgList << "putinum";
    msgList << data;

    send_agent(msgList.join(",") + "\n", isForce);
}

}  // namespace LIS
