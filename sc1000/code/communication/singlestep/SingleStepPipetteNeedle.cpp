#include "SingleStepPipetteNeedle.h"

#include <QDebug>
#include <QListView>
#include <QMetaEnum>

#include "communication/bootloader/bootloader_operation.h"
#include "communication/device/pipettingNeedle/DPipetteNeedle.h"
#include "ui_SingleStepPipetteNeedle.h"

namespace COM::SINGLEWSTEP {
namespace {
using Device = COM::DPipetteNeedle;
}

SingleStepPipetteNeedle::SingleStepPipetteNeedle(QWidget *parent)
    : QWidget(parent), ui(new Ui::SingleStepPipettingNeedle) {
    ui->setupUi(this);
    if (parent) {
        this->setGeometry(parent->rect());
    }
    ui->comboBox->setView(new QListView());

    ui->comboBox->clear();
    QMetaEnum qenum = QMetaEnum::fromType<Device::Mode>();
    for (int i = 0; i < Device::Mode_Count; i += 1) {
        ui->comboBox->addItem(qenum.key(i), i);
    }

    connect(ui->btn, &QPushButton::clicked, this, &SingleStepPipetteNeedle::operator_once);
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

SingleStepPipetteNeedle::~SingleStepPipetteNeedle() {
    delete ui;
}

void SingleStepPipetteNeedle::operator_once() {
    auto          &device = Device::instance();
    auto           type   = static_cast<Device::Mode>(ui->comboBox->currentData().toInt());
    Device::Params params;
    params.isValid           = true;
    params.yPosition         = ui->edit_0->text().toInt();
    params.zPosition         = ui->edit_1->text().toInt();
    params.suctionCapacity   = ui->edit_2->text().toInt();
    params.washYPosition     = params.yPosition;
    params.washZPosition     = params.zPosition;
    params.suctionZPosition  = params.zPosition;
    params.drainageZPosition = params.zPosition;
    params.suctionThreshold  = ui->edit_3->text().toInt();

    device.Set_Config(type, params);
    device.Exec_parallelTask();
}

void SingleStepPipetteNeedle::showEvent(QShowEvent *event) {
    Q_UNUSED(event);
    QWidget *parent = static_cast<QWidget *>(this->parent());
    if (parent) {
        this->setGeometry(parent->rect());
    }
}
}  // namespace COM::SINGLEWSTEP
