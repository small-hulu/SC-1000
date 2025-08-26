#include "WIDGET_Main_MBottom.h"

#include <QDateTime>
#include <QDebug>

#include "communication/COM_Device.hpp"
#include "controller/AppEvent.h"
#include "ui_WIDGET_Main_MBottom.h"

namespace WIDGET {
namespace MAIN {

MBottom::MBottom(QWidget* parent) : QWidget(parent), ui(new Ui::MBottom{}) {
    ui->setupUi(this);

    /// connect
    {
        auto&& appEvent = CONTROLLER::AppEvent::instance();
        connect(&appEvent, &CONTROLLER::AppEvent::signal_expStateHint, this,
                &MBottom::modify_expState);
    }

    /// 使用QObject自带的计时功能
    /// 并在计时器未timeout前调一次，保证ui的显示
    this->startTimer(1000);
    this->timerEvent(nullptr);
    modify_expState(false, "");
}

MBottom::~MBottom() {
    delete ui;
}

void MBottom::modify_expState(bool flag, const QString& msg) {
    ui->lab_exp_state->setText(msg);
    if (flag) {
        ui->lab_exp_state->show();
        ui->widget_exp_state->show();
    } else {
        ui->lab_exp_state->hide();
        ui->widget_exp_state->hide();
    }
}

/**
 * 采用QObject自带的计时操作
 */
void MBottom::timerEvent(QTimerEvent* event) {
    /// [时间]
    ui->lab_time->setText(QDateTime::currentDateTime().toString("hh:mm:ss"));
    ui->lab_date->setText(QDateTime::currentDateTime().toString("yyyy/MM/dd"));
    /// [时间]

    /// [通信]
    auto&& modbus = COM::ModbusImpl::instance();
    if (modbus.isOpen() == false) {
        const char* qss = R"(
image: url(:/suzhou/bottom_circle_red.png);
border: 0px;
)";
        ui->widget_com_state->setStyleSheet(qss);
        return;
    }
    const char* qss = R"(
image: url(:/suzhou/bottom_circle_green.png);
border: 0px;
)";
    ui->widget_com_state->setStyleSheet(qss);
    /// [通信]

    /// [温度监控]
    auto&& device = COM::DThermostat::instance();
    auto   param  = device.Get_MCUTemp();
    if (param.isValid) {
        QString incubateTemp   = QString::number(param.incubateTemp, 'f', 1);
        QString reagentBinTemp = QString::number(param.reagentBinTemp, 'f', 1);
        ui->lab_reaction_temp->setText(incubateTemp + QString("℃"));
        ui->lab_reagentBin_temp->setText(reagentBinTemp + QString("℃"));
    } else {
        ui->lab_reaction_temp->setText("---");
        ui->lab_reagentBin_temp->setText("---");
    }
    /// [温度监控]

    QWidget::timerEvent(event);
}

}  // namespace MAIN
}  // namespace WIDGET
