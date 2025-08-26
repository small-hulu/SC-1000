#include "SingleStepPhotoelectric.h"

#include <QDebug>
#include <QListView>
#include <QMetaEnum>

#include "communication/bootloader/bootloader_operation.h"
#include "communication/device/photoelectric/DPhotoelectric.h"
#include "ui_SingleStepPhotoelectric.h"

namespace COM::SINGLEWSTEP {
namespace {
using Device = COM::DPhotoelectric;
}

SingleStepPhotoelectric::SingleStepPhotoelectric(QWidget *parent)
    : QWidget(parent), ui(new Ui::SingleStepPhotoelectric) {
    ui->setupUi(this);
    if (parent) {
        this->setGeometry(parent->rect());
    }
    setWindowTitle("光点组件");
    ui->comboBox->setView(new QListView());

    ui->comboBox->clear();
    QMetaEnum qenum = QMetaEnum::fromType<Device::Mode>();
    for (int i = 0; i < Device::Mode_Count; i += 1) {
        ui->comboBox->addItem(qenum.key(i), i);
    }

    connect(ui->btn, &QPushButton::clicked, this, &SingleStepPhotoelectric::operator_once);
    connect(ui->btn_show, &QPushButton::clicked, this, []() {
        qDebug().noquote() << Device::instance().to_string();
        auto val = Device::instance().Get_value();
        qDebug() << "val = " << val;
    });
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

SingleStepPhotoelectric::~SingleStepPhotoelectric() {
    delete ui;
}

void SingleStepPhotoelectric::operator_once() {
    auto          &device = Device::instance();
    auto           type   = static_cast<Device::Mode>(ui->comboBox->currentData().toInt());
    Device::Params params{};
    params.isValid  = true;
    params.capacity = ui->edit_capacity->text().toInt();
    params.position = ui->edit_2->text().toInt();
    device.Set_Config(type, params);
    device.Exec_parallelTask();
}

void SingleStepPhotoelectric::showEvent(QShowEvent *event) {
    Q_UNUSED(event);
    QWidget *parent = static_cast<QWidget *>(this->parent());
    if (parent) {
        this->setGeometry(parent->rect());
    }
}
}  // namespace COM::SINGLEWSTEP
