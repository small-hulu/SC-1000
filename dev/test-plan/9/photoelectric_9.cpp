#include "photoelectric_9.h"

#include <QMessageBox>
#include <QtConcurrent>

#include "communication/COM_Device.hpp"
#include "ui_photoelectric_9.h"

Photoelectric_9::Photoelectric_9(QWidget* parent) : QWidget(parent), ui(new Ui::Photoelectric_9) {
    ui->setupUi(this);
    setWindowTitle("测光组件");

    {
        connect(ui->btn_start, &QPushButton::clicked, this, [this]() {
            ui->plainTextEdit->clear();
            ui->edit_runCount->setEnabled(false);
            ui->btn_start->setEnabled(false);
            m_cnt = 0;
            QTimer::singleShot(10, this, &Self::start_loop);
        });
    }
}

Photoelectric_9::~Photoelectric_9() {
    delete ui;
}

void Photoelectric_9::mark_message(const QString& msg) {
    ui->plainTextEdit->appendPlainText(msg);
}

void Photoelectric_9::operator_once() {
    QFuture<bool> future = QtConcurrent::run([this]() -> bool {
        using namespace COM;
        RetCode ret;
        auto&&  device = DPhotoelectric::instance();
        device.Set_Config(DPhotoelectric::Mode_DoorClose, {});
        ret = device.Exec_sequencedTask();
        if (ret != RetCode::Success) {
            mark_message(QString(">>>执行异常 %1").arg(ret.to_string()));
            return false;
        }

        device.Set_Config(DPhotoelectric::Mode_Detect, {});
        ret = device.Exec_sequencedTask();
        if (ret != RetCode::Success) {
            mark_message(QString(">>>执行异常 %1").arg(ret.to_string()));
            return false;
        }

        device.Set_Config(DPhotoelectric::Mode_DoorOpen, {});
        ret = device.Exec_sequencedTask();
        if (ret != RetCode::Success) {
            mark_message(QString(">>>执行异常 %1").arg(ret.to_string()));
            return false;
        }

        m_cnt += 1;
        QThread::msleep(rand() % 1000);
        return true;
    });

    QFutureWatcher<bool>* watcher = new QFutureWatcher<bool>{};
    watcher->setFuture(future);
    connect(watcher, &QFutureWatcher<bool>::finished, this, [=] {
        mark_message(QDateTime::currentDateTime().toString());
        mark_message(QString("执行次数【%1】").arg(m_cnt));

        using namespace COM;
        auto&&     device = DPhotoelectric::instance();
        const auto val    = device.Get_value();
        mark_message(QString("光值：%1").arg(val));

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

void Photoelectric_9::start_loop() {
    using namespace COM;
    bool ok = ModbusImpl::instance().isOpen();
    if (ok) {
        auto&& device = DPhotoelectric::instance();
        device.Set_Config(DPhotoelectric::Mode_HomingDoor, {});
        device.Exec_sequencedTask();
        device.Set_Config(DPhotoelectric::Mode_DoorClose, {});
        device.Exec_sequencedTask();
        device.Set_Config(DPhotoelectric::Mode_HomingPump, {});
        device.Exec_sequencedTask();

        operator_once();
    } else {
        mark_message(QString(">>> 通讯链接异常"));
    }
}
