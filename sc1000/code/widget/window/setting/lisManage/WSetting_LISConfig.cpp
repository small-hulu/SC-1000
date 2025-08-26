#include "WSetting_LISConfig.h"

#include <QSerialPort>
#include <QSerialPortInfo>

#include "controller/CONTROLLER_Config.h"
#include "ui_WSetting_LISConfig.h"
#include "widget/dialog/DIALOG.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::SETTING {

LISConfig::LISConfig(QWidget *parent) : QWidget(parent), ui(new Ui::LISConfig{}) {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);

    {
        m_ui_btnGroup = ButtonBottomGroup::Get();  // 按钮组
        m_ui_btnGroup->show();
        m_ui_btnGroup->Set_refreshFunc({});
        QList<QString> titles;
        titles << QString("保存配置");
        QList<std::function<void()>> taskList;
        taskList << std::bind(&LISConfig::save, this);

        m_ui_btnGroup->Set_BtnList(titles, taskList);
    }

    /// init-ui
    {
        auto combo = ui->combo_serialPort;
        UTILS::init_combo_reset(combo);
        combo->addItem("");
        for (auto &info : QSerialPortInfo::availablePorts()) {
            combo->addItem(info.portName());
        }

        mpLISDirection[0] = QString("单项");
        mpLISDirection[1] = QString("双项");
        combo             = ui->combo_lisDirection;
        UTILS::init_combo_reset(combo);
        for (auto it = mpLISDirection.cbegin(); it != mpLISDirection.cend(); it += 1) {
            combo->addItem(it.value(), it.key());
        }

        mpLISUpload[0] = QString("手动");
        mpLISUpload[1] = QString("自动");
        combo          = ui->combo_lisUpload;
        UTILS::init_combo_reset(combo);
        for (auto it = mpLISUpload.cbegin(); it != mpLISUpload.cend(); it += 1) {
            combo->addItem(it.value(), it.key());
        }
    }

    /// read-settings
    {
        CONTROLLER::Config::Network network;
        ui->combo_serialPort->setCurrentText(network.serialConfig.serialPort);

        ui->edit_serverIP->setText(network.networkConfig.serverIP);
        ui->edit_serverPort->setText(network.networkConfig.serverPort);

        ui->combo_lisUpload->setCurrentText(mpLISUpload[network.lisConfig.autoUpload]);
        ui->combo_lisDirection->setCurrentText(mpLISDirection[network.lisConfig.direction]);
    }
}

LISConfig::~LISConfig() {
    m_ui_btnGroup->hide();
    delete ui;
}
void LISConfig::lis_net_connect()
{
    CONTROLLER::Config::Network network;
    qint16 port = static_cast<qint16>(network.networkConfig.serverPort.toInt());
    auto& conn = Lis_net_con::instance();
    if (conn.connectToServer(network.networkConfig.serverIP, port)) {
        conn.sendData("Hello TCP Server");
        WIDGET::DIALOG::Create_DInfoDialog("Lis连接成功");
    }
    else
        WIDGET::DIALOG::Create_DInfoDialog("Lis连接失败");
}
void LISConfig::save() {
    CONTROLLER::Config::Network network;
    network.serialConfig.serialPort = ui->combo_serialPort->currentText();

    network.networkConfig.serverIP   = ui->edit_serverIP->text();
    network.networkConfig.serverPort = ui->edit_serverPort->text();

    network.lisConfig.autoUpload = ui->combo_lisUpload->currentData().toInt();
    network.lisConfig.direction  = ui->combo_lisDirection->currentData().toInt();

    network.Save();
    DIALOG::Create_DInfoDialog("保存完成");
}

void LISConfig::showEvent(QShowEvent *event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}

}  // namespace WIDGET::SETTING

void WIDGET::SETTING::LISConfig::on_pushButton_clicked()
{
    save();
    CONTROLLER::Config::Network network;
    qint16 port = static_cast<qint16>(network.networkConfig.serverPort.toInt());
    auto& conn = Lis_net_con::instance();
    if (conn.connectToServer(network.networkConfig.serverIP, port)) {
        conn.sendData("Hello TCP Server");
        WIDGET::DIALOG::Create_DInfoDialog("Lis连接成功");
    }
    else
        WIDGET::DIALOG::Create_DInfoDialog("Lis连接失败");
}
