#include "WQuality_QualityApplyList.h"

#include "global/GLOBAL_TypeTube.h"
#include "ui_WQuality_QualityApplyList.h"
#include "widget/dialog/DIALOG.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::QUALITY {
QualityApplyList::QualityApplyList(QWidget* parent)
    : QWidget(parent), ui(new Ui::QualityApplyList) {
    ui->setupUi(this);
    m_table = ui->tableWidget;

    /// table
    {
        QList<QString> titles;
        titles << QString("位置");
        titles << QString("质控名称");
        titles << QString("质控品批号");
        titles << QString("申请时间");
        titles << QString("状态");
        titles << QString("项目");

        UTILS::init_table_mode(m_table);
        UTILS::init_table_titles(m_table, titles);

        /// 设置缓存内容
        for (auto&& entity : CONTROLLER::ExpQuality::instance().Get_theList_expNotDone()) {
            this->Append_tableLine(entity);
        }
    }

    /// init
    {
        m_ui_btnGroup = ButtonBottomGroup::Get();
        m_ui_btnGroup->show();
        m_ui_btnGroup->RemoveAllButtons();
        m_ui_btnGroup->Set_refreshFunc(std::bind(&QualityApplyList::refresh_bottomBtnList, this));
        refresh_bottomBtnList();
    }
}

QualityApplyList::~QualityApplyList() {
    delete ui;
    ui = nullptr;
}

void QualityApplyList::Btn_delete() {
    const int idx = m_table->currentRow();
    if (idx < 0) {
        DIALOG::Create_DWarningDialog("未选中有效项");
        return;
    }

    auto dialog = DIALOG::Create_DIsVerify("确认需要卸载？");
    connect(dialog, &DIALOG::DIsVerify::signal_yes, this, [this, idx]() {
        auto item = m_items[idx];
        m_items.removeOne(item);
        item->Delete_toDB();
        m_table->removeRow(idx);
    });
}

void QualityApplyList::Append_tableLine(QualityApplyList::entity_item entity) {
    m_items.push_back(entity);
    auto line = get_entityShowMsgList(entity);

    assert(line.size() == m_table->columnCount());
    const int curRowCnt = m_table->rowCount();
    m_table->insertRow(curRowCnt);
    m_table->setRowHeight(curRowCnt, UTILS::table_auto_rowHeight);
    for (int i = 0; i < line.size(); i += 1) {
        auto* item = new QTableWidgetItem(line[i]);
        item->setTextAlignment(Qt::AlignCenter);
        m_table->setItem(curRowCnt, i, item);
    }
}

void QualityApplyList::refresh_bottomBtnList() {
    QList<QString>                   names;
    QList<std::function<void(void)>> taskList;
    if (false) {
    } else {
        names << QString("删除");
        taskList << std::bind(&QualityApplyList::Btn_delete, this);
    }
    m_ui_btnGroup->Set_BtnList(names, taskList);
}

QList<QString> QualityApplyList::get_entityShowMsgList(QualityApplyList::entity_item entity) {
    QList<QString> res;
    res << entity->table[entity_attr::location];
    res << entity->table[entity_attr::name];
    res << entity->table[entity_attr::batchNum];
    res << entity->table[entity_attr::timeStart];
    auto e    = entity->table[entity_attr::state].toInt();
    auto info = Type::Get_TubeStateInfo(Type::ETubeState(e));
    res << info.nameChinese;
    res << entity->table[entity_attr::project];
    return res;
}

void QualityApplyList::showEvent(QShowEvent* event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}
}  // namespace WIDGET::QUALITY
