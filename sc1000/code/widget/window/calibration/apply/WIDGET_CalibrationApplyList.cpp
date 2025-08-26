#include "WIDGET_CalibrationApplyList.h"

#include "controller/CONTROLLER_Calibration.h"
#include "controller/CONTROLLER_Reagent.h"
#include "ui_WIDGET_CalibrationApplyList.h"
#include "widget/dialog/DIALOG.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::CALIBRATION {
CalibrationApplyList::CalibrationApplyList(QWidget *parent)
    : QWidget(parent), ui(new Ui::CalibrationApplyList) {
    ui->setupUi(this);
    m_table = ui->tableWidget;

    {
        QList<QString> titles;
        titles << QString("项目");
        titles << QString("试剂批号");
        titles << QString("校准品批号");
        titles << QString("C0");
        titles << QString("C1");
        titles << QString("C2");
        titles << QString("C3");
        titles << QString("C4");
        titles << QString("C5");

        auto table = m_table;
        UTILS::init_table_mode(table);
        UTILS::init_table_titles(table, titles);
    }

    /// init
    {
        /// 本界面就是读完后纯展示
        /// 仅可删除
        auto calSet = CONTROLLER::ExpCalibration::instance().Get_theList_expNotDone();
        for (int i = 0; i < calSet.size(); i += 1) {
            Append_tableLine(calSet[i]);
        }

        m_ui_btnGroup = ButtonBottomGroup::Get();
        m_ui_btnGroup->show();
        m_ui_btnGroup->RemoveAllButtons();
        m_ui_btnGroup->Set_refreshFunc(
            std::bind(&CalibrationApplyList::refresh_bottomBtnList, this));
        refresh_bottomBtnList();
    }
}

CalibrationApplyList::~CalibrationApplyList() {
    delete ui;
}

void CalibrationApplyList::Btn_delete() {
    const int idx = m_table->currentRow();
    if (idx >= m_items.size()) {
        qWarning() << "映射错误";
        return;
    }

    auto dialog = DIALOG::Create_DIsVerify("是否确认删除");
    connect(dialog, &DIALOG::DIsVerify::signal_yes, this, [this, idx]() {
        auto item = m_items[idx];
        m_items.removeOne(item);
        item->Delete_toDB();
        m_table->removeRow(idx);
    });
}

void CalibrationApplyList::Append_tableLine(CalibrationApplyList::entity_item entity) {
    m_items.push_back(entity);
    auto table = m_table;

    const int curRowCnt = table->rowCount();
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

void CalibrationApplyList::refresh_bottomBtnList() {
    QList<QString>                   names;
    QList<std::function<void(void)>> taskList;
    if (false) {
    } else {
        names << QString("删除");
        taskList << std::bind(&CalibrationApplyList::Btn_delete, this);
    }
    m_ui_btnGroup->Set_BtnList(names, taskList);
}

QList<QString> CalibrationApplyList::get_entityShowMsgList(
    CalibrationApplyList::entity_item entity) {
    QList<QString> line;
    line << entity->table[entity_attr::project];

    auto reagentId     = entity->table[entity_attr::idReagent];
    auto reagentEntity = CONTROLLER::Reagent::instance().Select_byId_fromDB(reagentId.toInt());
    if (reagentEntity) {
        line << reagentEntity->table[CONTROLLER::Reagent::entity_attr::batchNum];
    } else {
        line << "";
    }

    auto calId     = entity->table[entity_attr::idCalibration];
    auto calEntity = CONTROLLER::Calibration::instance().Select_byId_fromDB(calId.toInt());
    if (calEntity) {
        line << calEntity->table[CONTROLLER::Calibration::entity_attr::batchNum];
    } else {
        line << "";
    }

    /// c0~c5
    /// TODO
    auto locationList = entity->Get_c0n_LocationList();
    for (int i = 0; i <= 5; i += 1) {
        if (i < locationList.size()) {
            line << locationList[i];
        } else {
            line << "";
        }
    }
    return line;
}

void CalibrationApplyList::showEvent(QShowEvent *event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}
}  // namespace WIDGET::CALIBRATION
