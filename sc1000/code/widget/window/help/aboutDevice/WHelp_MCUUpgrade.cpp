#include "WHelp_MCUUpgrade.h"

#include <QFileDialog>
#include <QProcess>
#include <QtConcurrent>

#include "communication/COM.hpp"
#include "communication/bootloader/bootloader_operation.h"
#include "ui_WHelp_MCUUpgrade.h"
#include "widget/dialog/DIALOG.h"
#include "widget/utils/UTILS.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace WIDGET::HELP {
MCUUpgrade::MCUUpgrade(QWidget* parent) : QWidget(parent), ui(new Ui::MCUUpgrade) {
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

        auto com = ui->com_device;
        UTILS::init_combo_reset(com);
        for (auto [name, add] : deviceMsg) {
            com->addItem(name, add);
        }
    }

    /// connect
    {
        connect(ui->btn_upgrade, &QPushButton::clicked, this, &MCUUpgrade::exec_upgrade);

        connect(ui->btn_path, &QPushButton::clicked, this, [this]() {
            QString path = QFileDialog::getOpenFileName(this);
            ui->btn_path->setText(path);
        });
    }
}

MCUUpgrade::~MCUUpgrade() {
    delete ui;
}

/**
 * 弹窗显示进度百分比
 */
void MCUUpgrade::exec_upgrade() {
    using Boot = COM::BOOTLOADER::BootloaderOp;

    auto dialog = DIALOG::Create_DProgressIndicator();
    dialog->Set_percentHint("升级中");
    QFuture<int> future = QtConcurrent::run([=]() -> int {
        COM::WatchDog_Guard stopCheck;

        const int     deviceAdd = ui->com_device->currentData().toInt();
        const QString path      = ui->btn_path->text();

        Boot::UpgradeState state;
        bool               isrunning = true;

        std::thread th([&]() {
            while (isrunning) {
                QThread::msleep(500);
                const QString msg = QString::asprintf("升级中 %d %%", (int)(state.pre * 100));
                dialog->Set_percentHint(msg);
            }
        });

        Boot       boot(deviceAdd);
        const auto ret = boot.device_upgrade(path, 0x00010203, state);
        isrunning      = false;
        if (th.joinable()) {
            th.join();
        }

        return ret;
    });

    QFutureWatcher<int>* watcher = new QFutureWatcher<int>;
    watcher->setFuture(future);
    QObject::connect(watcher, &QFutureWatcher<int>::finished, [=] {
        auto ret = watcher->result();
        if (ret == Boot::ErrUpgrade::ErrUpdateSucceed) {
            DIALOG::Create_DInfoDialog("升级成功");
        } else {
            QMetaEnum     qenum = QMetaEnum::fromType<Boot::ErrUpgrade>();
            const QString msg   = QString("升级失败 %1").arg(qenum.key(ret));
            DIALOG::Create_DWarningDialog(msg);
        }

        dialog->close();
        watcher->deleteLater();
    });
}

void MCUUpgrade::on_pushButton_app_upgrade_clicked() {
    auto lte_controler = LTE_ML302::get_instance();
    using namespace CONTROLLER;
    auto&& ini = IniSC1000::instance();
    if (lte_controler->is_open_serial() == false) {
        lte_controler->open_serial_port(ini[KI::lte_vid].toString(), ini[KI::lte_pid].toString());
    }

    QString serialPortName;
    foreach (const QSerialPortInfo& info, QSerialPortInfo::availablePorts()) {
        QString vendorIdStr  = QString::number(info.vendorIdentifier());
        QString productIdStr = QString::number(info.productIdentifier());
        if (vendorIdStr == ini[KI::lte_vid].toString().toUpper() &&
            productIdStr == ini[KI::lte_pid].toString().toUpper()) {
            serialPortName = info.portName();  // 找到串口名
        }
    }
    QString update_software_path = QCoreApplication::applicationDirPath() + "/PMDRemoteUpgrade.exe";
    // 获取当前可执行文件的路径
    QString   executablePath     = QCoreApplication::applicationDirPath();
    QString   filePath           = QCoreApplication::applicationFilePath();
    QFileInfo fileInfo(filePath);
    QString   fileName = fileInfo.fileName();
    QProcess  Process;
    QString   a                = QDir::currentPath();
    QString   software_Version = "V1.0.0.1 1970-01-01";

    uint32_t pid = GetCurrentProcessId();
    lte_controler->close_serial_port();
    QStringList _list = {software_Version,      {"SC-1000"}, {executablePath},
                         {serialPortName},      {"1"},       {"SC-1000.exe"},
                         {QString::number(pid)}};
    //_path 升级软件路径   _list 调用子程序exe传参
    bool success      = Process.startDetached(update_software_path, _list);
    // 1000ms 阻塞当前程序1s等待Process执行完成
    if (success) {
        QMessageBox::information(nullptr, "信息", "升级软件打开成功");
    } else {
        QMessageBox::information(nullptr, "信息", "路径错误");
    }
}

void MCUUpgrade::on_btn_upgrade_remote_clicked() {
    auto lte_controler = LTE_ML302::get_instance();
    using namespace CONTROLLER;
    auto&& ini = IniSC1000::instance();
    if (lte_controler->is_open_serial() == false) {
        lte_controler->open_serial_port(ini[KI::lte_vid].toString(), ini[KI::lte_pid].toString());
    }

    QString serialPortName;
    foreach (const QSerialPortInfo& info, QSerialPortInfo::availablePorts()) {
        QString vendorIdStr  = QString::number(info.vendorIdentifier());
        QString productIdStr = QString::number(info.productIdentifier());
        if (vendorIdStr == ini[KI::lte_vid].toString().toUpper() &&
            productIdStr == ini[KI::lte_pid].toString().toUpper()) {
            serialPortName = info.portName();  // 找到串口名
        }
    }
    QString update_software_path = QCoreApplication::applicationDirPath() + "/PMDRemoteUpgrade.exe";
    // 获取当前可执行文件的路径
    QString   executablePath     = QCoreApplication::applicationDirPath() + "/download";
    QString   filePath           = QCoreApplication::applicationFilePath();
    QFileInfo fileInfo(filePath);
    QString   fileName = fileInfo.fileName();
    QProcess  Process;
    QString   a                = QDir::currentPath();
    QString   software_Version = "V1.0.0.1 1970-01-01";

    uint32_t pid = -1;
    lte_controler->close_serial_port();
    QStringList _list = {software_Version,      {"SC-1000"}, {executablePath},
                         {serialPortName},      {"0"},       {"appbin.zip"},
                         {QString::number(pid)}};
    //_path 升级软件路径   _list 调用子程序exe传参
    bool success      = Process.startDetached(update_software_path, _list);
    // 1000ms 阻塞当前程序1s等待Process执行完成
    if (success) {
        QMessageBox::information(nullptr, "信息", "升级软件打开成功");
    } else {
        QMessageBox::information(nullptr, "信息", "路径错误");
    }
}

void MCUUpgrade::showEvent(QShowEvent* event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}
}  // namespace WIDGET::HELP
