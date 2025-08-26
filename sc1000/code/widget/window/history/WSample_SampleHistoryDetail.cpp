#include "WSample_SampleHistoryDetail.h"

#include "ui_WSample_SampleHistoryDetail.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::HISTORY {
SampleHistoryDetail::SampleHistoryDetail(QWidget *parent)
    : QWidget(parent), ui(new Ui::SampleHistoryDetail) {
    ui->setupUi(this);
    table = ui->tableWidget;

    /// init
    {
        ui->lab_title->setText("结果详情");

        QList<QString> titles;
        titles << QString("项目");
        titles << QString("浓度");
        titles << QString("发光值");
        titles << QString("结束时间");
        titles << QString("试剂批号");
        titles << QString("试剂瓶号");
        titles << QString("激发液批号");
        titles << QString("激发液瓶号");
        titles << QString("校准品批号");
        titles << QString("校准时间");
        UTILS::init_table_mode(table);
        UTILS::init_table_titles(table, titles);
    }
}

SampleHistoryDetail::~SampleHistoryDetail() {
    delete ui;
}

void SampleHistoryDetail::Set_entity(SampleHistoryDetail::entity_item entity) {
    m_entity = entity;
    refresh();
}

void SampleHistoryDetail::refresh() {
    if (m_entity == nullptr) {
        return;
    }

    refresh_message(m_entity);
    Append_tableLine(m_entity);
}

void SampleHistoryDetail::refresh_message(SampleHistoryDetail::entity_item entity) {
    ui->lab_num_edit->setText(entity->Get_idString());
    auto idSample = entity->table[entity_attr::idSample];
    auto sample   = CONTROLLER::Sample::instance().Select_byId_fromDB(idSample);
    if (sample) {
        ui->lab_location_edit->setText(sample->table[CONTROLLER::Sample::entity_attr::location]);
    } else {
        ui->lab_location_edit->setText("数据错误");
    }
}

void SampleHistoryDetail::Append_tableLine(SampleHistoryDetail::entity_item entity) {
    auto line = get_lineStr(entity);

    assert(line.size() == table->columnCount());
    const int curRowCnt = table->rowCount();
    table->insertRow(curRowCnt);
    table->setRowHeight(curRowCnt, UTILS::table_auto_rowHeight);
    for (int i = 0; i < line.size(); i += 1) {
        auto *item = new QTableWidgetItem(line[i]);
        item->setTextAlignment(Qt::AlignCenter);
        table->setItem(curRowCnt, i, item);
    }
}

QList<QString> SampleHistoryDetail::get_lineStr(SampleHistoryDetail::entity_item entity) {
    QList<QString> res;

    res << entity->table[entity_attr::project];
    if (entity->table[entity_attr::project] == "25-OH-VD") {
        QStringList data = entity->table[entity_attr::resultData].split(";");
        res << data[0];
    } else {
        res << entity->table[entity_attr::resultData];
    }

    res << entity->table[entity_attr::expData];
    res << entity->table[entity_attr::timeEnd];
    auto idReagent = entity->table[entity_attr::idReagent];
    auto reagent   = CONTROLLER::Reagent::instance().Select_byId_fromDB(idReagent);
    if (reagent) {
        using attr_reagent = CONTROLLER::Reagent::entity_attr;
        res << reagent->table[attr_reagent::batchNum];
        res << reagent->table[attr_reagent::bottleNum];
    } else {
        res << QString("数据错误");
        res << QString("数据错误");
    }
    res << QString("未录入");
    res << QString("未录入");
    res << QString("未录入");
    res << QString("未录入");
    return res;
}

void SampleHistoryDetail::showEvent(QShowEvent *event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}
}  // namespace WIDGET::HISTORY
