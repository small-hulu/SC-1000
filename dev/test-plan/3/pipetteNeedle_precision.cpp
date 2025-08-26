#include "pipetteNeedle_precision.h"

#include "communication/COM_Device.hpp"
#include "log/LOG.h"
#include "ui_pipetteNeedle_precision.h"

using namespace COM;

PipetteNeedlePrecision::PipetteNeedlePrecision(QWidget *parent)
    : QWidget(parent), ui(new Ui::PipetteNeedlePrecision) {
    ui->setupUi(this);
    setWindowTitle("加样精度检测");

    connect(ui->btn_homing, &QPushButton::clicked, this, &PipetteNeedlePrecision::op_homing);
    connect(ui->btn_op_once, &QPushButton::clicked, this, [this]() {
        ui->btn_op_once->setEnabled(false);
        PipetteNeedlePrecision::op_once();
        ui->btn_op_once->setEnabled(true);
    });
}

PipetteNeedlePrecision::~PipetteNeedlePrecision() {
    delete ui;
}

void PipetteNeedlePrecision::op_homing() {
    auto &&reagentBin = DReagentBin::instance();
    auto   rparams    = DReagentBin::Params{};
    reagentBin.Set_Config(DReagentBin::Mode_Homing, rparams);
    reagentBin.Exec_sequencedTask();

    auto &&pipette = DPipetteNeedle::instance();
    auto   pparams = DPipetteNeedle::Params();
    pipette.Set_Config(DPipetteNeedle::Mode_Homing, pparams);
    pipette.Exec_sequencedTask();
    pparams = pipette.GetInfo_to_Wash().params;
    pipette.Set_Config(DPipetteNeedle::Mode_Wash, pparams);
    pipette.Exec_sequencedTask();
    pipette.Set_Config(DPipetteNeedle::Mode_Homing, pparams);
    pipette.Exec_sequencedTask();

    m_liquidResidual  = ui->edit_init_liquid->text().toInt();
    const int fromIdx = ui->edit_from->text().toInt();
    COM::SharedMemory::instance().Set_ReagentBinHole_liquidResidual(0, fromIdx, m_liquidResidual);
}

void PipetteNeedlePrecision::op_once() {
    QString   msg;
    const int threshold = ui->edit_threshold->text().toInt();
    const int capacity  = ui->edit_Suction->text().toInt();
    const int fromIdx   = ui->edit_from->text().toInt();
    const int toIdx     = ui->edit_to->text().toInt();

    RetCode ret;
    auto  &&reagentBin = DReagentBin::instance();
    auto    rparams    = DReagentBin::Params{};
    auto  &&pipette    = DPipetteNeedle::instance();
    auto    pparams    = DPipetteNeedle::Params();

    ui->textEdit->append(QDateTime::currentDateTime().toString());

    /// 吸液
    {
        rparams = reagentBin.GetInfo_backHole(fromIdx);
        reagentBin.Offset_byProximity(rparams);
        reagentBin.Set_Config(DReagentBin::Mode_Position, rparams);
        ret = reagentBin.Exec_sequencedTask();
        if (ret != RetCode::Success) {
            ui->textEdit->append(QString("试剂仓 [%1]").arg(ret.to_string()));
        }
        pparams                  = pipette.GetInfo_to_ReagentBin(fromIdx).params;
        pparams.suctionThreshold = threshold;
        pparams.suctionCapacity  = capacity;
        pipette.Set_Config(DPipetteNeedle::Mode_MoveSuction, pparams);
        ret = pipette.Exec_sequencedTask();
        if (ret != RetCode::Success) {
            ui->textEdit->append(QString("移液针 [%1]").arg(ret.to_string()));
        }
        qDebug().noquote().nospace() << pipette.to_string();

        /// 验证随量跟踪模式
        pipette.Suction_reagentTrack(pparams, 0, fromIdx);
        msg = QString("理想深度 %1").arg(pparams.zPosition);
        ui->textEdit->append(msg);
        sLog_PipettingNeedle() << msg;
        msg = QString("实际深度 %1").arg(pipette.Get_mcuParams().descentCache);
        ui->textEdit->append(msg);
        sLog_PipettingNeedle() << msg;

        /// 执行吸取完后的累计
        const int liquidCapacity  = motorStep_to_microlitre_PipetteNeedle(capacity);
        m_liquidResidual         -= liquidCapacity;
        COM::SharedMemory::instance().Set_ReagentBinHole_liquidResidual(0, fromIdx,
                                                                        m_liquidResidual);
    }

    /// 排液
    {
        rparams = reagentBin.GetInfo_backHole(toIdx);
        reagentBin.Offset_byProximity(rparams);
        reagentBin.Set_Config(DReagentBin::Mode_Position, rparams);
        ret = reagentBin.Exec_sequencedTask();
        if (ret != RetCode::Success) {
            ui->textEdit->append(QString("试剂仓 [%1]").arg(ret.to_string()));
        }
        pparams = pipette.GetInfo_to_ReagentBin(toIdx).params;
        pipette.Set_Config(DPipetteNeedle::Mode_MoveDrainage, pparams);
        ret = pipette.Exec_sequencedTask();
        if (ret != RetCode::Success) {
            ui->textEdit->append(QString("移液针 [%1]").arg(ret.to_string()));
        }
        qDebug().noquote().nospace() << pipette.to_string();
    }
}
