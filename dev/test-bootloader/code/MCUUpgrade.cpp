#include "MCUUpgrade.h"

#include <QFileDialog>
#include <QFutureWatcher>
#include <QListView>
#include <QtConcurrent>

#include "communication/COM.hpp"
#include "communication/bootloader/bootloader_operation.h"
#include "ui_MCUUpgrade.h"

MCUUpgrade::MCUUpgrade(QWidget* parent) : QWidget(parent), ui(new Ui::MCUUpgrade{}) {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);

    /// device-list
    {
        auto get_listItem = [](auto&& device) -> QPair<QString, int> {
            QString name = device.objectName();
            int     add  = device.get_dataUnitConfig().deviceAddress;
            return {name, add};
        };
        QList<QPair<QString, int>> deviceMsg;
        deviceMsg << get_listItem(COM::DGripper::instance());
        deviceMsg << get_listItem(COM::DMixing::instance());
        deviceMsg << get_listItem(COM::DReagentBin::instance());
        deviceMsg << get_listItem(COM::DPhotoelectric::instance());
        deviceMsg << get_listItem(COM::DPipetteNeedle::instance());
        deviceMsg << get_listItem(COM::DMagneticSeparation::instance());
        deviceMsg << get_listItem(COM::DThermostat::instance());

        auto combo = ui->com_device;
        combo->clear();
        combo->setView(new QListView{});
        for (auto [name, add] : deviceMsg) {
            combo->addItem(name, add);
        }

        combo = ui->com_port;
        combo->clear();
        combo->setView(new QListView{});
        refresh_port();
    }

    /// connect
    {
        connect(ui->btn_upgrade, &QPushButton::clicked, this, &MCUUpgrade::exec_upgrade);

        connect(ui->btn_path, &QPushButton::clicked, this, [this]() {
            QString path = QFileDialog::getOpenFileName(this);
            ui->btn_path->setText(path);
        });

        connect(ui->btn_link, &QPushButton::clicked, this, &MCUUpgrade::open_modbus);
    }
}

MCUUpgrade::~MCUUpgrade() {
    delete ui;
}

void MCUUpgrade::refresh_port() {
    ui->com_port->clear();
    for (auto& info : QSerialPortInfo::availablePorts()) {
        ui->com_port->addItem(info.portName());
    }
}

void MCUUpgrade::open_modbus() {
    const QString port = ui->com_port->currentText();
    QString       msg;

    using namespace COM;
    auto modbusPtr = &COM::ModbusImpl::instance();
    if (modbusPtr->open(port) == false) {
        msg = port + ") modbus connect = false";
    } else {
        msg = port + ") modbus connect = true";
    }

    qInfo().noquote().nospace() << msg;
    ui->textEdit->append(msg);
}

/**
 * 弹窗显示进度百分比
 */
void MCUUpgrade::exec_upgrade() {
    using Boot = COM::BOOTLOADER::BootloaderOp;

    auto task = [=]() -> int {
        const int     deviceAdd = ui->com_device->currentData().toInt();
        const QString path      = ui->btn_path->text();

        QString msg = QString(">>>deviceAdd[%1] path[%2]").arg(deviceAdd).arg(path);
        qDebug() << msg;
        ui->textEdit->append(msg);

        Boot::UpgradeState state;
        bool               isrunning = true;

        std::thread th([&]() {
            while (isrunning) {
                QThread::msleep(500);
                const QString msg = QString::asprintf("升级中 %.2lf %%", (state.pre * 100));
                qDebug() << msg;
                ui->textEdit->append(msg);
            }
        });

        Boot       boot(deviceAdd);
        const auto ret = boot.device_upgrade(path, 0x00010203, state);
        isrunning      = false;
        if (th.joinable()) {
            th.join();
        }

        return ret;
    };

    QFuture<int>         future  = QtConcurrent::run(task);
    QFutureWatcher<int>* watcher = new QFutureWatcher<int>;
    watcher->setFuture(future);
    QObject::connect(watcher, &QFutureWatcher<int>::finished, [=] {
        auto ret = watcher->result();
        if (ret == Boot::ErrUpgrade::ErrUpdateSucceed) {
            const QString msg = ("$$$升级成功");
            qDebug() << (msg);
            ui->textEdit->append(msg);
        } else {
            QMetaEnum     qenum = QMetaEnum::fromType<Boot::ErrUpgrade>();
            const QString msg   = QString("$$$升级失败 %1").arg(qenum.key(ret));
            qDebug() << (msg);
            ui->textEdit->append(msg);
        }
        watcher->deleteLater();
    });
}
