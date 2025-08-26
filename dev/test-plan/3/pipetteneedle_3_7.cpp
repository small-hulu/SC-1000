#include "pipetteneedle_3_7.h"

#include <QDateTime>
#include <QFuture>
#include <QFutureWatcher>
#include <QMessageBox>
#include <QTimer>
#include <QtConcurrent>

#include "communication/COM.hpp"
#include "communication/controller/ActionMajor.h"
#include "communication/controller/DeviceOpAuthorizeCU.h"
#include "communication/controller/Parse.h"
#include "ui_pipetteneedle_3_7.h"
#include "util/UTILS.hpp"

PipetteNeedle_3_7::PipetteNeedle_3_7(QWidget* parent)
    : QWidget(parent), ui(new Ui::PipetteNeedle_3_7) {
    ui->setupUi(this);
    setWindowTitle("采样针组件");

    /// 试剂的设置
    {
        auto combo = ui->combo_reagent;
        UTIL::combo_init(combo);
        for (int i = 1; i <= 10; i += 1) {
            combo->addItem(QString::number(i));
        }
    }

    /// 样本的设置
    {
        auto combo = ui->combo_sample;
        UTIL::combo_init(combo);
        for (int i = UTIL::TubeSpaceMark::Mark_1; i <= UTIL::TubeSpaceMark::Mark_E10; i += 1) {
            combo->addItem(UTIL::Get_TubeSpaceMark(i));
        }
    }

    /// connect
    {
        connect(ui->btn_start, &QPushButton::clicked, this, [this]() {
            ui->textEdit->clear();
            ui->edit_runCount->setEnabled(false);
            ui->combo_sample->setEnabled(false);
            ui->combo_reagent->setEnabled(false);
            ui->btn_start->setEnabled(false);
            m_cnt = 0;
            QTimer::singleShot(10, this, &Self::start_loop);
        });
    }
}

PipetteNeedle_3_7::~PipetteNeedle_3_7() {
    delete ui;
}

void PipetteNeedle_3_7::mark_message(const QString& msg) {
    ui->textEdit->append(msg);
}

void PipetteNeedle_3_7::operator_once() {
    auto check = [](int ret) -> bool {
        switch (ret) {
        case COM::RetCode::Success:
        case COM::RetCode::Insufficient_in : return true;
        case COM::RetCode::Insufficient_out: return true;
        default                            : return false;
        }
    };

    QFuture<bool> future = QtConcurrent::run([this, check]() -> bool {
        using namespace COM;
        auto& owner = DeviceManager::enum_to_owner(DeviceManager::atomic_flag::PipettingNeedle);
        auto  lock  = MultLock::Create_entity({&owner});

        auto       tastList  = COM::Parse::parsing_process_node(m_json);
        const bool ok        = COM::Parse::simple_single_process(tastList, check);
        m_cnt               += 1;
        return ok;
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

void PipetteNeedle_3_7::start_loop() {
    const QString sampleMark = ui->combo_sample->currentText();
    UTIL::BinInfo sampleInfo = UTIL::Get_MarkToGroup(sampleMark);

    const int     reagentIdx = ui->combo_reagent->currentText().toInt();
    UTIL::BinInfo reagentInfo;
    reagentInfo.idxBinGroup = reagentIdx - 1;
    reagentInfo.idxBinBack  = 0;

    using namespace COM;

    /// 操作试剂
    auto gen = [](UTIL::BinInfo info) -> QJsonArray {
        const int idxBinGroup = info.idxBinGroup;
        const int idxBinBack  = info.idxBinBack;

        QJsonArray  jsonArr;
        QJsonObject json;
        QJsonObject op;

        /// 转动试剂仓
        json[KJ::actionType] = KJ::ActionPatch;
        json[KJ::deviceName] = KJ::ReagentBin;
        op[KJ::deviceMode]   = DReagentBin::Mode_Position;
        op[KJ::idxBinGroup]  = idxBinGroup;
        op[KJ::idxBinBack]   = idxBinBack;
        json[KJ::op]         = op;
        jsonArr.append(json);
        op = QJsonObject();

        /// 移液针去吸液
        json[KJ::actionType] = KJ::ActionPatch;
        json[KJ::deviceName] = KJ::PipettingNeedle;
        op[KJ::deviceMode]   = DPipetteNeedle::Mode_MoveSuction;
        op[KJ::target]       = KJ::ReagentBin;
        op[KJ::capacity]     = microlitre_to_motorStep_PipetteNeedle(20);
        op[KJ::idxBinBack]   = idxBinBack;
        json[KJ::op]         = op;
        jsonArr.append(json);
        op = QJsonObject();

        /// 移液针 模拟去上料位排液
        json[KJ::actionType] = KJ::ActionPatch;
        json[KJ::deviceName] = KJ::PipettingNeedle;
        op[KJ::deviceMode]   = DPipetteNeedle::Mode_MoveSuction;
        op[KJ::target]       = KJ::Load;
        op[KJ::capacity]     = 0;
        json[KJ::op]         = op;
        jsonArr.append(json);
        op = QJsonObject();

        /// 移液针将液体排出
        json[KJ::actionType] = KJ::ActionPatch;
        json[KJ::deviceName] = KJ::PipettingNeedle;
        op[KJ::deviceMode]   = DPipetteNeedle::Mode_MoveDrainage;
        op[KJ::target]       = KJ::ReagentBin;
        op[KJ::idxBinBack]   = idxBinBack;
        json[KJ::op]         = op;
        jsonArr.append(json);
        op = QJsonObject();

        /// 移液针将液体排出
        json[KJ::actionType] = KJ::ActionPatch;
        json[KJ::deviceName] = KJ::PipettingNeedle;
        op[KJ::deviceMode]   = DPipetteNeedle::Mode_Wash;
        op[KJ::target]       = KJ::Wash;
        json[KJ::op]         = op;
        jsonArr.append(json);
        op = QJsonObject();

        return jsonArr;
    };

    QJsonArray jsonArr;
    sampleInfo.idxBinBack += 4;
    for (auto info : {reagentInfo, sampleInfo}) {
        auto arr = gen(info);
        for (int i = 0; i < arr.size(); i += 1) {
            jsonArr.append(arr[i].toObject());
        }
    }

    m_json                   = QJsonObject();
    m_json[KJ::processSteps] = jsonArr;
    mark_message(QString(">>> 实验开始") + QDateTime::currentDateTime().toString());

    qDebug().noquote() << m_json;

    bool ok = COM::ModbusImpl::instance().isOpen();
    if (ok) {
        auto&& reagentBin = DReagentBin::instance();
        reagentBin.Set_Config(DReagentBin::Mode_Homing, {});
        reagentBin.Exec_sequencedTask();

        auto&& pipetteNeedle = DPipetteNeedle::instance();
        pipetteNeedle.Set_Config(DPipetteNeedle::Mode_Homing, {});
        pipetteNeedle.Exec_sequencedTask();
        pipetteNeedle.Set_Config(DPipetteNeedle::Mode_Wash, pipetteNeedle.GetInfo_to_Wash().params);
        pipetteNeedle.Exec_sequencedTask();

        operator_once();
    } else {
        mark_message(QString(">>> 通讯链接异常"));
    }
}
