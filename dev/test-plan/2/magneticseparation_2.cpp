#include "magneticseparation_2.h"

#include <QMessageBox>
#include <QtConcurrent>

#include "communication/COM_Device.hpp"
#include "ui_magneticseparation_2.h"

MagneticSeparation_2::MagneticSeparation_2(QWidget* parent)
    : QWidget(parent), ui(new Ui::MagneticSeparation_2) {
    ui->setupUi(this);
    setWindowTitle("磁分离组件");

    {
        connect(ui->btn_start, &QPushButton::clicked, this, [this]() {
            ui->textEdit->clear();
            ui->btn_start->setEnabled(false);
            ui->edit_runCount->setEnabled(false);
            m_cnt = 0;
            QTimer::singleShot(10, this, &Self::start_loop);
        });
    }
}

MagneticSeparation_2::~MagneticSeparation_2() {
    delete ui;
}

void MagneticSeparation_2::mark_message(const QString& msg) {
    ui->textEdit->append(msg);
}

void MagneticSeparation_2::operator_once() {
    using namespace COM;

    QFuture<bool> future = QtConcurrent::run([this]() -> bool {
        using namespace COM;
        auto      tube = Tube::Create_new_SmartPipe();
        auto&&    cu   = DMagneticSeparation::instance().Get_CU();
        const int idx  = cu.Request_putIn();
        if (idx == -1) {
            mark_message("磁分离申请失败，请联系研发人员");
            return false;
        }

        tube->paramPkg.magneticSeparationIndex = idx;
        cu.Affirm_putIn(tube);
        while (tube->Get_state() != Tube::State_CanBeTransfer) {
            QThread::yieldCurrentThread();
        }
        cu.Affirm_fetch(tube);

        m_cnt += 1;
        return true;
    });

    QFutureWatcher<bool>* watcher = new QFutureWatcher<bool>{};
    watcher->setFuture(future);
    connect(watcher, &QFutureWatcher<bool>::finished, this, [=] {
        mark_message(QDateTime::currentDateTime().toString());
        mark_message(QString("执行次数【%1】").arg(m_cnt));

        auto ok = watcher->result();
        if (ok == false) {
            mark_message("实验异常 终止");
            auto message =
                new QMessageBox(QMessageBox::Warning, "异常报警", windowTitle() + "异常停止");
            message->setAttribute(Qt::WA_DeleteOnClose);
            message->show();
            return;
        }
        const int loopCnt = ui->edit_runCount->text().toInt();
        if (m_cnt < loopCnt) {
            QTimer::singleShot(100, this, &Self::operator_once);
        }
        watcher->deleteLater();
    });
}

void MagneticSeparation_2::start_loop() {
    using namespace COM;
    bool ok = ModbusImpl::instance().isOpen();
    if (ok) {
        auto&& device = DMagneticSeparation::instance();
        device.Set_Config(DMagneticSeparation::Mode_HomingTray, {});
        device.Exec_sequencedTask();
        device.Set_Config(DMagneticSeparation::Mode_HomingZ, {});
        device.Exec_sequencedTask();
        device.Set_Config(DMagneticSeparation::Mode_HomingWashPump, {});
        device.Exec_sequencedTask();
        device.Set_Config(DMagneticSeparation::Mode_HomingASubstratePump, {});
        device.Exec_sequencedTask();

        operator_once();
    } else {
        mark_message(QString(">>> 通讯链接异常"));
    }
}
