#include "SingleStepMagneticSeparation.h"

#include <QDebug>
#include <QListView>
#include <QMetaEnum>

#include "communication/bootloader/bootloader_operation.h"
#include "communication/device/magneticSeparation/DMagneticSeparation.h"
#include "ui_SingleStepMagneticSeparation.h"

namespace COM::SINGLEWSTEP {
namespace {
using Device = COM::DMagneticSeparation;
}

SingleStepMagneticSeparation::SingleStepMagneticSeparation(QWidget *parent)
    : QWidget(parent), ui(new Ui::SingleStepMagneticSeparation) {
    ui->setupUi(this);

    if (parent) {
        this->setGeometry(parent->rect());
    }
    setWindowTitle("磁分离");
    ui->comboBox->setView(new QListView());

    ui->comboBox->clear();
    QMetaEnum qenum = QMetaEnum::fromType<Device::Mode>();
    for (int i = 0; i < Device::Mode_Count; i += 1) {
        ui->comboBox->addItem(qenum.key(i), i);
    }

    connect(ui->btn, &QPushButton::clicked, this, &SingleStepMagneticSeparation::operator_once);
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

SingleStepMagneticSeparation::~SingleStepMagneticSeparation() {
    delete ui;
}

void SingleStepMagneticSeparation::operator_once() {
    auto          &device = Device::instance();
    auto           type   = static_cast<Device::Mode>(ui->comboBox->currentData().toInt());
    Device::Params params;
    params.isValid            = true;
    params.position           = ui->edit_0->text().toInt();
    params.washCapacity       = ui->edit_1->text().toInt();
    params.aSubstrateCapacity = ui->edit_2->text().toInt();
    params.zAxisHeight        = ui->edit_3->text().toInt();

    device.Set_Config(type, params);
    device.Exec_parallelTask();
}

void SingleStepMagneticSeparation::showEvent(QShowEvent *event) {
    Q_UNUSED(event);
    QWidget *parent = static_cast<QWidget *>(this->parent());
    if (parent) {
        this->setGeometry(parent->rect());
    }
}
}  // namespace COM::SINGLEWSTEP
