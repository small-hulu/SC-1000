#include "WCalibration_CalibrationManageMain.h"

#include "WCalibration_DCalibrationManageDetail.h"
#include "controller/CONTROLLER_Reagent.h"
#include "ui_WCalibration_CalibrationManageMain.h"
#include "widget/Background.h"
#include "widget/dialog/DIALOG.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::CALIBRATION {
CalibrationManageMain::CalibrationManageMain(QWidget *parent)
    : QWidget(parent), ui(new Ui::CalibrationManageMain) {
    ui->setupUi(this);
    table_left  = ui->table_left;
    table_right = ui->table_right;

    /// init-table
    {
        QList<QString> titles;
        titles << QString("序号");
        titles << QString("项目");
        titles << QString("校准品批号");
        titles << QString("C0");
        titles << QString("C1");
        titles << QString("C2");
        titles << QString("C3");
        titles << QString("C4");
        titles << QString("C5");
        auto table = table_left;

        UTILS::init_table_mode(table);
        UTILS::init_table_titles(table, titles);
        UTILS::init_table_colWidth(table, titles);
    }

    {
        QList<QString> titles;
        titles << QString("试剂批号");
        titles << QString("主曲线");
        auto table = ui->table_right;

        UTILS::init_table_mode(table);
        UTILS::init_table_titles(table, titles);
    }

    /// data-init
    {
        auto entityList = CONTROLLER::Calibration::instance().Select_all_fromDB();
        for (auto entity : entityList) {
            Append_tableLine(entity);
        }
    }

    /// connect
    {
        connect(table_left, &QTableWidget::cellPressed, this, [this]() { Append_tableValue(); });
    }
}

CalibrationManageMain::~CalibrationManageMain() {
    delete ui;
}

void CalibrationManageMain::Btn_delete() {
    const int index = table_left->currentRow();
    if (index < 0 || index >= m_items.size()) {
        return;
    }

    auto dialog = DIALOG::Create_DIsVerify("请确认删除");
    connect(dialog, &DIALOG::DIsVerify::signal_yes, this, [this]() {
        const int index  = table_left->currentRow();
        auto      entity = m_items[index];
        entity->Delete_toDB();
        m_items.removeOne(entity);
        table_left->removeRow(index);
    });
}

void CalibrationManageMain::Btn_edit() {
    const int index = table_left->currentRow();
    if (index < 0 || index >= m_items.size()) {
        return;
    }
    auto entity = m_items[index];

    auto dialog = new DCalibrationManageDetail(Background::Get());
    connect(dialog, &DCalibrationManageDetail::signal_yes, this, [this, entity, index]() {
        auto table = table_left;
        auto line  = get_entityShowMsgList(entity);
        assert(line.size() == table->columnCount());
        for (int i = 0; i < line.size(); i += 1) {
            table->item(index, i)->setText(line[i]);
        }

        entity->Update_toDB();
    });
    dialog->Set_entity(entity);
    dialog->show();
}

void CalibrationManageMain::Btn_addNew() {
    auto entity = CONTROLLER::Calibration::instance().Create_oneEntityItem();
    auto dialog = new DCalibrationManageDetail(Background::Get());
    connect(dialog, &DCalibrationManageDetail::signal_yes, this, [this, entity]() {
        entity->Insert_toDB();
        Append_tableLine(entity);
    });
    dialog->Set_entity(entity);
    dialog->show();
}

void CalibrationManageMain::Append_tableLine(CalibrationManageMain::entity_item entity) {
    m_items.push_back(entity);
    auto table = table_left;

    const int curRowCnt = table_left->rowCount();
    table->insertRow(curRowCnt);
    table->setRowHeight(curRowCnt, UTILS::table_auto_rowHeight);

    QList<QString> line = get_entityShowMsgList(entity);
    assert(line.size() == table->columnCount());
    for (int i = 0; i < line.size(); i += 1) {
        auto *item = new QTableWidgetItem(line[i]);
        item->setTextAlignment(Qt::AlignCenter);
        table->setItem(curRowCnt, i, item);
    }
}

/**
 * 根据项目名称进行查找
 */
void CalibrationManageMain::Append_tableValue() {
    const int idx = table_left->currentRow();
    if (idx < 0) {
        return;
    }

    auto item    = m_items[idx];
    auto project = item->table[entity_attr::project];
    auto reaList = CONTROLLER::Reagent::instance().FilterList_by_project(project);

    QList<QList<QString>> mat;
    for (auto reagent : reaList) {
        QList<QString> line;
        line << reagent->table[CONTROLLER::Reagent::entity_attr::batchNum];
        line << reagent->Get_MainCurve();
        mat << std::move(line);
    }

    auto table = table_right;
    while (table->rowCount()) {
        table->removeRow(table->rowCount() - 1);
    }
    for (int i = 0; i < mat.size(); i += 1) {
        table->insertRow(i);
        table->setRowHeight(i, UTILS::table_auto_rowHeight);
        assert(table->columnCount() == mat[i].size());
        for (int j = 0; j < mat[i].size(); j += 1) {
            auto *item = new QTableWidgetItem(mat[i][j]);
            item->setTextAlignment(Qt::AlignCenter);
            table->setItem(i, j, item);
        }
    }
}

QList<QString> CalibrationManageMain::get_entityShowMsgList(
    CalibrationManageMain::entity_item entity) {
    QList<QString> line;
    line << QString::number(entity->id);
    line << entity->table[entity_attr::project];
    line << entity->table[entity_attr::batchNum];
    auto valList = entity->Get_c0n_ConcentrationList();
    for (int i = 0; i < 6; i += 1) {
        if (i < valList.size()) {
            line << QString::number(valList[i]);
        } else {
            line << QString("---");
        }
    }
    return line;
}

void CalibrationManageMain::showEvent(QShowEvent *event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}
}  // namespace WIDGET::CALIBRATION
