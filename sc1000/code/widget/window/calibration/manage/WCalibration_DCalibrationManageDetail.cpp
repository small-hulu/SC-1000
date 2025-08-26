#include "WCalibration_DCalibrationManageDetail.h"

#include "qrcode/QRCODE.hpp"
#include "ui_WCalibration_DCalibrationManageDetail.h"
#include "widget/dialog/DIALOG.h"

namespace WIDGET::CALIBRATION {
DCalibrationManageDetail::DCalibrationManageDetail(QWidget *parent)
    : QWidget(parent), ui(new Ui::DCalibrationManageDetail) {
    ui->setupUi(this);

    if (parent) {
        this->setGeometry(parent->rect());
    }
    this->setAttribute(Qt::WA_StyledBackground);
    this->setAttribute(Qt::WA_DeleteOnClose);

    /// 保证是一个非空的
    {
        if (m_entity == nullptr) {
            m_entity = CONTROLLER::Calibration::Create_oneEntityItem();
        }
    }

    /// connect 取消 确认
    {
        connect(ui->btn_no, &QPushButton::clicked, this, [this]() { this->close(); });

        connect(ui->btn_yes, &QPushButton::clicked, this, [this]() {
            record_ui_to_entity();
            emit signal_yes();
            this->close();
        });

        auto &&qrcode = QRCODE::SerialQrcode::instance();
        connect(&qrcode, &QRCODE::SerialQrcode::signal_qrcode_originalData, this,
                [this](const QByteArray &bytes, int type) {
                    if (type != (int)QRCODE::QrcodeType::calibration) {
                        DIALOG::Create_DWarningDialog("条码匹配错误");
                        return;
                    }

                    m_entity->table[entity_attr::qrcode] = bytes;
                    save_qrcodeToUi();
                });
    }
}

DCalibrationManageDetail::~DCalibrationManageDetail() {
    delete ui;
}

void DCalibrationManageDetail::Set_entity(DCalibrationManageDetail::entity_item entity) {
    m_entity = entity;
    record_entity_to_ui();
}

void DCalibrationManageDetail::record_ui_to_entity() {
    m_entity->table[entity_attr::project]  = ui->edit_params_1->text();
    m_entity->table[entity_attr::batchNum] = ui->edit_params_2->text();
    m_entity->table[entity_attr::typeTube] = ui->edit_params_3->text();

    m_entity->Set_c0n_ConcentrationList(merge_c0n_fromui());
    m_entity->table[entity_attr::mark] = ui->textEdit->toPlainText();
}

void DCalibrationManageDetail::record_entity_to_ui() {
    ui->edit_params_1->setText(m_entity->table[entity_attr::project]);
    ui->edit_params_2->setText(m_entity->table[entity_attr::batchNum]);
    ui->edit_params_3->setText(m_entity->table[entity_attr::typeTube]);

    QList<QLineEdit *> editList;
    editList << ui->edit_c0;
    editList << ui->edit_c1;
    editList << ui->edit_c2;
    editList << ui->edit_c3;
    editList << ui->edit_c4;
    editList << ui->edit_c5;
    auto valList = m_entity->Get_c0n_ConcentrationList();
    for (int i = 0; i < qMin(valList.size(), editList.size()); i += 1) {
        editList[i]->setText(QString::number(valList[i]));
    }
}

QList<double> DCalibrationManageDetail::merge_c0n_fromui() {
    QList<QLineEdit *> editList;
    editList << ui->edit_c0;
    editList << ui->edit_c1;
    editList << ui->edit_c2;
    editList << ui->edit_c3;
    editList << ui->edit_c4;
    editList << ui->edit_c5;
    QList<double> res;
    for (auto edit : editList) {
        QString s = edit->text();
        if (s.isEmpty() == false) {
            res << s.toDouble();
        } else {
            break;
        }
    }
    return res;
}

void DCalibrationManageDetail::save_qrcodeToUi() {
    ui->textEdit->setText(m_entity->table[entity_attr::qrcode]);

    QRCODE::ECalibration qr(m_entity->table[entity_attr::qrcode].toUtf8());
    namespace attr_qr = QRCODE::KeyCal;
    ui->edit_params_1->setText(qr[attr_qr::base_proname].toString());
    ui->edit_params_2->setText(qr[attr_qr::base_batchnum].toString());
    auto               c0nList = qr[attr_qr::calconf_calconvlist].toStringList();
    QList<QLineEdit *> editList;
    editList << ui->edit_c0;
    editList << ui->edit_c1;
    editList << ui->edit_c2;
    editList << ui->edit_c3;
    editList << ui->edit_c4;
    editList << ui->edit_c5;
    for (int i = 0; i < editList.size(); i += 1) {
        if (i < c0nList.size()) {
            editList[i]->setText(c0nList[i]);
        } else {
            editList[i]->setText("");
        }
    }
}
}  // namespace WIDGET::CALIBRATION
