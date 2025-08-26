#include "WReagent_ReagentTable.h"

#include "WReagent_DReagentDetails.h"
#include "WReagent_DReagentParams.h"
#include "ui_WReagent_ReagentTable.h"
#include "widget/dialog/DIALOG.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::REAGENT {
ReagentTable::ReagentTable(QWidget* parent) : QWidget(parent), ui(new Ui::ReagentTable{}) {
    ui->setupUi(this);
    table = ui->tableWidget;

    /// init-table
    {
        QList<QString> titles;
        titles << QString("位置");
        titles << QString("项目");
        titles << QString("剩余测试数");
        titles << QString("有效期");
        titles << QString("批号");
        titles << QString("瓶号");
        titles << QString("状态标记");
        titles << QString("开瓶标记");
        titles << QString("校准状态");
        UTILS::init_table_mode(table);
        UTILS::init_table_titles(table, titles);
    }

    startTimer(1500);
    timerEvent(nullptr);
}

ReagentTable::~ReagentTable() {
    if (ui) {
        delete ui;
        ui = nullptr;
    }
}

void ReagentTable::Entity_install(int curIdx) {
    auto entity                        = CONTROLLER::Reagent::Create_oneEntityItem();
    entity->table[entity_attr::isLoad] = "1";
    // set location to reagent bin current select index
    if (curIdx >= 0) {
        entity->table[entity_attr::location] = QString::number(curIdx);
    }
    auto dialog = new DReagentParams{entity, Background::Get()};

    connect(dialog, &DReagentParams::signal_yes, this, [this, dialog]() {
        auto entity = dialog->Get_entity();
        CONTROLLER::Reagent::instance().Load_entity_toCU(entity);
    });
    dialog->show();
}

void ReagentTable::Entity_uninstall() {
    const int idx = table->currentRow();
    if (idx < 0) {
        DIALOG::Create_DWarningDialog("未选中有效项");
        return;
    }

    auto dialog = DIALOG::Create_DIsVerify("确认需要卸载？");
    connect(dialog, &DIALOG::DIsVerify::signal_yes, this, [this, idx]() {
        auto item = m_items[idx];
        m_items.removeOne(item);
        item->table[entity_attr::isLoad] = "0";
        item->Update_toDB();
        table->removeRow(idx);
    });
}

void ReagentTable::Entity_modify() {
    const int idx = table->currentRow();
    if (idx < 0) {
        DIALOG::Create_DWarningDialog("未选中有效项");
        return;
    }

    auto entity = m_items[idx];
    auto dialog = new DReagentParams{entity, Background::Get()};
    connect(dialog, &DReagentParams::signal_yes, this,
            [dialog]() { dialog->Get_entity()->Update_toDB(); });
    dialog->show();
}

void ReagentTable::Entity_details() {
    const int idx = table->currentRow();
    if (idx < 0) {
        DIALOG::Create_DWarningDialog("未选中有效项");
        return;
    }
    auto entity = m_items[idx];
    auto dialog = new DReagentDetails{entity, Background::Get()};
    dialog->show();
}

void ReagentTable::Append_tableLine(ReagentTable::entity_item entity) {
    m_items.push_back(entity);
    auto line = get_lineValues(entity);

    assert(line.size() == table->columnCount());
    const int curRowCnt = table->rowCount();
    table->insertRow(curRowCnt);
    table->setRowHeight(curRowCnt, UTILS::table_auto_rowHeight);
    for (int i = 0; i < line.size(); i += 1) {
        auto* item = new QTableWidgetItem(line[i]);
        item->setTextAlignment(Qt::AlignCenter);
        table->setItem(curRowCnt, i, item);

        if (i == 2) {
            auto op = entity->Deter_surplus(line[i]);
            if (op.has_value()) {
                item->setForeground(op.value());
            }
        }
    }
}

QList<QString> ReagentTable::get_lineValues(ReagentTable::entity_item item) {
    QList<entity_attr> attrList;
    attrList << entity_attr::location;
    attrList << entity_attr::project;
    attrList << entity_attr::ableMeasureBottleCnt;
    attrList << entity_attr::periodOfValidity;
    attrList << entity_attr::batchNum;
    attrList << entity_attr::bottleNum;
    attrList << entity_attr::state;
    attrList << entity_attr::bottleOpenTime;
    attrList << entity_attr::calibrationStatus;
    auto line = item->Get_attrListValues(attrList);

    auto qrcode   = item->Get_qrcodeEntity();
    namespace key = QRCODE::KeyRea;
    line[2]       = QString::number(qrcode[key::base_tq].toString().toInt() -
                                    item->table[entity_attr::measuredTimes].toInt());
    return line;
}

void ReagentTable::showEvent(QShowEvent* event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}

void ReagentTable::timerEvent(QTimerEvent* event) {
    int row = table->currentRow();
    table->clearContents();
    while (table->rowCount()) {
        table->removeRow(table->rowCount() - 1);
    }
    m_items.clear();
    for (auto&& entity : CONTROLLER::Reagent::instance().Select_isLoadList()) {
        Append_tableLine(entity);
    }
    table->setCurrentCell(row, 0);
}

}  // namespace WIDGET::REAGENT
