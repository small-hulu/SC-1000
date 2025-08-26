#include "WQuality_DQualityManageInstallByScan.h"

#include "qrcode/QRCODE.hpp"
#include "ui_WQuality_DQualityManageInstallByScan.h"
#include "widget/dialog/DIALOG.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::QUALITY {
DQualityManageInstallByScan::DQualityManageInstallByScan(QWidget *parent)
    : QWidget(parent), ui(new Ui::DQualityManageInstallByScan) {
    ui->setupUi(this);

    /// dialog
    {
        this->setAttribute(Qt::WA_StyledBackground);
        this->setAttribute(Qt::WA_DeleteOnClose);
    }

    /// init
    {
        QList<QString> titles;
        titles << QString("质控品名称");
        titles << QString("项目");
        titles << QString("靶值");
        titles << QString("SD值");

        auto table = ui->tableWidget;
        UTILS::init_table_mode(table);
        UTILS::init_table_titles(table, titles);

        Reset();
    }

    /// connect
    {
        connect(ui->btn_no, &QPushButton::clicked, this, [this]() { this->close(); });

        connect(ui->btn_yes, &QPushButton::clicked, this, [this]() {
            emit signal_yes();
            this->close();
        });

        /// 接受条码的信号
        /// todo
        auto &&qrcode = QRCODE::SerialQrcode::instance();
        connect(&qrcode, &QRCODE::SerialQrcode::signal_qrcode_originalData, this,
                [this](QByteArray bytes, int type) {
                    if (type != (int)QRCODE::QrcodeType::quality) {
                        DIALOG::Create_DWarningDialog("条码匹配错误");
                        return;
                    }
                    m_barcode = bytes;
                    Reset();
                    refresh_ui();
                });
    }
}

DQualityManageInstallByScan::~DQualityManageInstallByScan() {
    delete ui;
}

void DQualityManageInstallByScan::Reset() {
    ui->lab_batchNum->setText("");
    ui->lab_bottleNum->setText("");
    ui->lab_product->setText("");
    ui->lab_validity->setText("");

    auto table = ui->tableWidget;
    while (table->rowCount()) {
        table->removeRow(table->rowCount() - 1);
    }
}

QList<CONTROLLER::Quality::entity_item> DQualityManageInstallByScan::Get_curEntityItem() {
    namespace qrattr = QRCODE::KeyQua;
    QRCODE::EQuality qr(m_barcode);

    auto gen_entity = [this, qr](int idx, const QString &target, const QString &sd) {
        auto entity                        = CONTROLLER::Quality::Create_oneEntityItem();
        entity->table[entity_attr::qrcode] = m_barcode;
        entity->table[entity_attr::name] =
            QString("%1(Q%2)").arg(qr[qrattr::base_qcname].toString()).arg(idx);
        entity->table[entity_attr::batchNumber]      = qr[qrattr::base_batchnum].toString();
        entity->table[entity_attr::bottleNumber]     = qr[qrattr::base_bottleno].toString();
        entity->table[entity_attr::project]          = qr[qrattr::qci_proname].toString();
        entity->table[entity_attr::productDate]      = qr[qrattr::base_producdate].toString();
        entity->table[entity_attr::periodOfValidity] = qr[qrattr::base_validity].toString();

        entity->table[entity_attr::valTarget] = target;
        entity->table[entity_attr::valSD]     = sd;
        return entity;
    };
    auto targetList = qr[qrattr::qci_targetlist].toStringList();
    auto sdList     = qr[qrattr::qci_sdlist].toStringList();
    if (targetList.size() != sdList.size()) {
        sLog_qrcode() << "质控品 靶值.len != sd.len";
        return {};
    }

    QList<entity_item> ans;
    for (int i = 0; i < targetList.size(); i += 1) {
        ans << gen_entity(i + 1, targetList[i], sdList[i]);
    }

    return ans;
}

/**
 * 根据条码刷新ui
 */
void DQualityManageInstallByScan::refresh_ui() {
    auto entityList = Get_curEntityItem();
    if (entityList.size() == 0) {
        return;
    }

    /// 标题
    {
        auto entity = entityList.front();
        ui->lab_batchNum->setText(entity->table[entity_attr::batchNumber]);
        ui->lab_bottleNum->setText(entity->table[entity_attr::bottleNumber]);
        ui->lab_product->setText(entity->table[entity_attr::productDate]);
        ui->lab_validity->setText(entity->Get_expirationDate().toString(QRCODE::time_format));
    }

    /// 表格
    QList<QList<QString>> matrix;
    for (auto entity : entityList) {
        QList<QString> lines;
        lines << entity->table[entity_attr::name];
        lines << entity->table[entity_attr::project];
        lines << entity->table[entity_attr::valTarget];
        lines << entity->table[entity_attr::valSD];
        matrix << std::move(lines);
    }
    UTILS::init_table_fix_content(ui->tableWidget, matrix);
}

void DQualityManageInstallByScan::showEvent(QShowEvent *event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}
}  // namespace WIDGET::QUALITY
