#include "WQuality_QualityApplyMain.h"

#include "controller/CONTROLLER_Reagent.h"
#include "ui_WQuality_QualityApplyMain.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::QUALITY {
QualityApplyMain::QualityApplyMain(QWidget* parent)
    : QWidget(parent), ui(new Ui::QualityApplyMain) {
    ui->setupUi(this);

    /// table-left
    {
        table_left = ui->table_left;
        UTILS::init_table_mode(table_left);

        QList<QString> titles;
        titles << QString("质控品");
        titles << QString("批号");
        titles << QString("有效期");

        table_left->setColumnCount(titles.size());
        table_left->setHorizontalHeaderLabels(titles);
    }

    /// table-right
    {
        table_right = ui->table_right;
        UTILS::init_table_mode(table_right);

        QList<QString> titles;
        titles << QString("项目");
        table_right->setColumnCount(titles.size());
        table_right->setHorizontalHeaderLabels(titles);
    }

    /// 设置缓存内容
    auto quaCon = &CONTROLLER::Quality::instance();
    for (auto&& entity : quaCon->Select_all_fromDB()) {
        this->Append_leftTableLine(entity);
    }

    connect(ui->table_left, &QTableWidget::cellPressed, this,
            [this]() { this->Append_rightTableValue(); });
}

QualityApplyMain::~QualityApplyMain() {
    delete ui;
}

QualityApplyMain::UiInfo QualityApplyMain::Get_UiInfo() {
    UiInfo    info;
    const int idx_left  = table_left->currentRow();
    const int idx_right = table_right->currentRow();
    if (idx_left < 0 || idx_right < 0) {
        return info;
    }

    auto item        = m_items[idx_left];
    info.project     = table_right->item(idx_right, 0)->text();
    info.idQuality   = item->Get_idString();
    info.name        = item->table[entity_attr::name];
    info.batchNumber = item->table[entity_attr::batchNumber];
    info.timeStart   = item->table[entity_attr::timeStart];
    info.state       = item->table[entity_attr::state];
    info.valTarget   = item->table[entity_attr::valTarget];
    info.valSD       = item->table[entity_attr::valSD];
    /// 记录对应的试剂编号
    auto reagentList = CONTROLLER::Reagent::instance().Select_isLoadList();
    for (auto&& reagent : reagentList) {
        if (reagent->table[CONTROLLER::Reagent::entity_attr::project] == info.project) {
            info.idReagent = reagent->Get_idString();
            break;
        }
    }
    return info;
}

QualityApplyMain::entity_itemExp QualityApplyMain::Btn_save(QualityApplyMain::UiInfo info) {
    auto entity                              = CONTROLLER::ExpQuality::Create_oneEntityItem();
    entity->table[entity_attrExp::state]     = QString::number((int)Type::ETubeState::Apply);
    entity->table[entity_attrExp::idQuality] = info.idQuality;
    entity->table[entity_attrExp::idReagent] = info.idReagent;
    entity->table[entity_attrExp::location]  = info.location;
    entity->table[entity_attrExp::name]      = info.name;
    entity->table[entity_attrExp::batchNum]  = info.batchNumber;
    entity->table[entity_attrExp::project]   = info.project;
    entity->table[entity_attrExp::valTarget] = info.valTarget;
    entity->table[entity_attrExp::valSD]     = info.valSD;
    entity->Insert_toDB();
    return entity;
}

void QualityApplyMain::Append_leftTableLine(QualityApplyMain::entity_item entity) {
    m_items.push_back(entity);
    auto line = get_leftLineStr(entity);

    assert(line.size() == table_left->columnCount());
    const int curRowCnt = table_left->rowCount();
    table_left->insertRow(curRowCnt);
    table_left->setRowHeight(curRowCnt, UTILS::table_auto_rowHeight);
    for (int i = 0; i < line.size(); i += 1) {
        auto* item = new QTableWidgetItem(line[i]);
        item->setTextAlignment(Qt::AlignCenter);
        table_left->setItem(curRowCnt, i, item);
    }
}

void QualityApplyMain::Append_rightTableValue() {
    auto table = table_right;
    while (table->rowCount()) {
        table->removeRow(table->rowCount() - 1);
    }
    const int idx = table_left->currentRow();
    if (idx < 0) {
        return;
    }

    auto lines = get_rightLineStr(m_items[idx]);
    for (auto&& line : lines) {
        const int curRowCnt = table->rowCount();
        table->insertRow(curRowCnt);
        table->setRowHeight(curRowCnt, UTILS::table_auto_rowHeight);

        for (int j = 0; j < line.size(); j += 1) {
            auto* item = new QTableWidgetItem(line[j]);
            item->setTextAlignment(Qt::AlignCenter);
            table->setItem(curRowCnt, j, item);
        }
    }
}

QList<QString> QualityApplyMain::get_leftLineStr(QualityApplyMain::entity_item entity) {
    QList<QString> res;
    res << entity->table[entity_attr::name];
    res << entity->table[entity_attr::batchNumber];
    res << entity->Get_expirationDate().toString(QRCODE::time_format);
    return res;
}

QList<QList<QString>> QualityApplyMain::get_rightLineStr(QualityApplyMain::entity_item entity) {
    QList<QList<QString>> res;
    for (auto s : entity->table[entity_attr::project].split(ENTITY::EQuality::join_separator,
                                                            QString::SkipEmptyParts)) {
        res << QList{s};
    }
    return res;
}

void QualityApplyMain::showEvent(QShowEvent* event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}
}  // namespace WIDGET::QUALITY
