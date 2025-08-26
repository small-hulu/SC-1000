#include "gripper_5.h"

#include <QMessageBox>
#include <QtConcurrent>

#include "communication/COM_Device.hpp"
#include "communication/controller/ActionMinor.h"
#include "ui_gripper_5.h"

Gripper_5::Gripper_5(QWidget* parent) : QWidget(parent), ui(new Ui::Gripper_5) {
    ui->setupUi(this);
    setWindowTitle("抓杯手组件");

    connect(ui->btn_start, &QPushButton::clicked, this, [this]() {
        ui->textEdit->clear();
        ui->edit_runCount->setEnabled(false);
        ui->btn_start->setEnabled(false);
        m_cnt = 0;
        QTimer::singleShot(10, this, &Self::start_loop);
    });

    connect(ui->btn_throwcup, &QPushButton::clicked, this, [this]() {
        if (m_timeId == -1) {
            m_timeId = startTimer(1500);
        } else {
            killTimer(m_timeId);
            m_timeId = -1;
        }
    });
}

Gripper_5::~Gripper_5() {
    delete ui;
}

void Gripper_5::mark_message(const QString& msg) {
    ui->textEdit->append(msg);
}

void Gripper_5::operator_once() {
    QFuture<bool> future = QtConcurrent::run([this]() -> bool {
        using namespace COM;
        auto   tube = Tube::Create_new_SmartPipe();
        QPoint point(0, 0);

        auto exec_task = [&](QPoint next) -> bool {
            QJsonObject json;

            json[KJ::actionType] = KJ::ActionMinor;
            json[KJ::deviceName] = KJ::Gripper;

            QJsonObject op;
            op[KJ::target] = KJ::Incubate;
            op[KJ::i]      = point.x();
            op[KJ::j]      = point.y();
            json[KJ::from] = op;

            point        = next;
            op[KJ::i]    = point.x();
            op[KJ::j]    = point.y();
            json[KJ::to] = op;

            auto       task = ActionMinor::Minor_for_Gripper_from_to(json);
            const auto ret  = task(tube);
            if (ret != RetCode::Success) {
                mark_message(QString(">>>执行异常 %1").arg(ret.to_string()));
                return false;
            }
            return true;
        };

        for (int i = 0; i < 10; i += 1) {
            auto next = QPoint(rand() % 7, rand() % 5);
            if (exec_task(next) == false) {
                return false;
            }
        }

        m_cnt += 1;
        return exec_task({0, 0});
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

void Gripper_5::start_loop() {
    using namespace COM;
    bool ok = ModbusImpl::instance().isOpen();
    if (ok) {
        auto&& device = DGripper::instance();
        device.Set_Config(DGripper::Mode_Homing, {});
        device.Exec_sequencedTask();

        operator_once();
    } else {
        mark_message(QString(">>> 通讯链接异常"));
    }
}

void Gripper_5::timerEvent(QTimerEvent* event) {
    using namespace COM;

    auto exec_task = [&]() -> bool {
        auto&& mixCup = DMixing::instance();
        mixCup.Set_Config(DMixing::Mode_EnableDivideCup, {});
        mixCup.Exec_sequencedTask();
        /// 杯孔要非空
        if (mixCup.Get_mcuStateBits().u1000_Mixing.b02_cupholeFull == 0) {
            return false;
        }

        QJsonObject json;

        json[KJ::actionType] = KJ::ActionMinor;
        json[KJ::deviceName] = KJ::Gripper;

        QJsonObject op;
        op[KJ::target] = KJ::Cup;
        json[KJ::from] = op;

        op[KJ::target] = KJ::Throw;
        json[KJ::to]   = op;

        auto       task = ActionMinor::Minor_for_Gripper_from_to(json);
        auto       tube = Tube::Create_new_SmartPipe();
        const auto ret  = task(tube);
        if (ret != RetCode::Success) {
            mark_message(QString(">>>执行异常 %1").arg(ret.to_string()));
            return false;
        }
        return true;
    };

    exec_task();
}
