#include "SingleStepGripper.h"

#include <QDebug>
#include <QListView>
#include <QMetaEnum>

#include "communication/bootloader/bootloader_operation.h"
#include "communication/device/gripper/DGripper.h"
#include "ui_SingleStepGripper.h"

namespace COM::SINGLEWSTEP {
namespace {
using Device = COM::DGripper;
}

SingleStepGripper::SingleStepGripper(QWidget *parent)
    : QWidget(parent), ui(new Ui::SingleStepGripper) {
    ui->setupUi(this);
    if (parent) {
        this->setGeometry(parent->rect());
    }
    setWindowTitle("夹爪");
    ui->comboBox->setView(new QListView());

    ui->comboBox->clear();
    QMetaEnum qenum = QMetaEnum::fromType<Device::Mode>();
    for (int i = 0; i < Device::Mode_Count; i += 1) {
        ui->comboBox->addItem(qenum.key(i), i);
    }

    connect(ui->btn, &QPushButton::clicked, this, &SingleStepGripper::operator_once);
    connect(ui->btn_show, &QPushButton::clicked, this,
            []() { qDebug().noquote() << Device::instance().to_string(); });
    connect(ui->btn_reset, &QPushButton::clicked, this, []() {
        auto                    &device = Device::instance();
        BOOTLOADER::BootloaderOp boot(device.get_dataUnitConfig().deviceAddress);
        const auto               ret = boot.system_reset();
        if (ret == 0) {
            qDebug() << "reset ok";
        } else {
            qWarning() << "reset error ret = " << ret;
        }
    });
}

SingleStepGripper::~SingleStepGripper() {
    delete ui;
}

void SingleStepGripper::operator_once() {
    auto          &device = Device::instance();
    auto           type   = static_cast<Device::Mode>(ui->comboBox->currentData().toInt());
    Device::Params params;
    params.isValid     = true;
    params.xPosition   = ui->edit_0->text().toInt();
    params.yPosition   = ui->edit_1->text().toInt();
    params.zPosition   = ui->edit_4->text().toInt();
    params.deepGrab    = ui->edit_2->text().toInt();
    params.deepRelease = ui->edit_3->text().toInt();
    params.openRange   = ui->edit_5->text().toInt();

    device.Set_Config(type, params);
    device.Exec_parallelTask();
}

void SingleStepGripper::showEvent(QShowEvent *event) {
    Q_UNUSED(event);
    QWidget *parent = static_cast<QWidget *>(this->parent());
    if (parent) {
        this->setGeometry(parent->rect());
    }
}
}  // namespace COM::SINGLEWSTEP
