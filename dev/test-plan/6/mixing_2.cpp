#include "mixing_2.h"

#include <QMessageBox>
#include <QtConcurrent>

#include "communication/COM_Device.hpp"
#include "ui_mixing_2.h"

Mixing_2::Mixing_2(QWidget* parent) : QWidget(parent), ui(new Ui::Mixing_2) {
    ui->setupUi(this);
    setWindowTitle("分杯混匀组件");

    connect(ui->btn_start, &QPushButton::clicked, this, [this]() {
        ui->textEdit->clear();
        ui->edit_runCount->setEnabled(false);
        ui->btn_start->setEnabled(false);
        m_cnt = 0;
        QTimer::singleShot(10, this, &Self::start_loop);
    });
}

Mixing_2::~Mixing_2() {
    delete ui;
}

void Mixing_2::mark_message(const QString& msg) {
    ui->textEdit->append(msg);
}

void Mixing_2::operator_once() {
    QFuture<bool> future = QtConcurrent::run([this]() -> bool {
        using namespace COM;

        auto&& device = DMixing::instance();
        if (rand() & 1) {
            device.Set_Config(DMixing::Mode_EnableMix, {});
        } else {
            device.Set_Config(DMixing::Mode_EnableDivideCup, {});
        }
        const auto ret = device.Exec_sequencedTask();
        if (ret != RetCode::Success) {
            mark_message(QString(">>>执行异常 %1").arg(ret.to_string()));
            return false;
        }

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

void Mixing_2::start_loop() {
    using namespace COM;
    bool ok = ModbusImpl::instance().isOpen();
    if (ok) {
        auto&& device = DMixing::instance();
        device.Set_Config(DMixing::Mode_HomingMix, {});
        device.Exec_sequencedTask();
        device.Set_Config(DMixing::Mode_HomingDivideCup, {});
        device.Exec_sequencedTask();

        operator_once();
    } else {
        mark_message(QString(">>> 通讯链接异常"));
    }
}
