#include "SingleStepThermostat.h"

#include <QListView>
#include <QMetaEnum>

#include "communication/bootloader/bootloader_operation.h"
#include "communication/device/thermostat/DThermostat.h"
#include "ui_SingleStepThermostat.h"

namespace COM::SINGLEWSTEP {
namespace {
using Device = COM::DThermostat;
}

SingleStepThermostat::SingleStepThermostat(QWidget *parent)
    : QWidget(parent), ui(new Ui::SingleStepThermostat) {
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

    connect(ui->btn, &QPushButton::clicked, this, &SingleStepThermostat::operator_once);
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

SingleStepThermostat::~SingleStepThermostat() {
    delete ui;
}

void SingleStepThermostat::operator_once() {
    auto          &device = Device::instance();
    auto           type   = static_cast<Device::Mode>(ui->comboBox->currentData().toInt());
    int            able   = ui->edit_able->text().toInt();
    float          temp   = ui->edit_temp->text().toFloat();
    Device::Params params{};
    params.isValid           = true;
    params.incubateAble      = able;
    params.reagentBinAble    = able;
    params.reagentBinLidAble = able;
    params.photoelectricAble = able;
    params.fan0Able          = able;
    params.fan1Able          = able;
    params.fan2Able          = able;
    params.fan3Able          = able;

    params.incubateTemp      = temp;
    params.photoelectricTemp = temp;
    params.reagentBinTemp    = temp;
    params.reagentBinLidTemp = temp;

    device.Set_Config(type, params);
    device.Exec_parallelTask();
}

void SingleStepThermostat::showEvent(QShowEvent *event) {
    Q_UNUSED(event);
    QWidget *parent = static_cast<QWidget *>(this->parent());
    if (parent) {
        this->setGeometry(parent->rect());
    }
}
}  // namespace COM::SINGLEWSTEP
