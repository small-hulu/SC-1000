#include "WIDGET_SampleApplyTable.h"

#include "apply/WSample_DSampleApplyParams.h"
#include "ui_WIDGET_SampleApplyTable.h"
#include "widget/dialog/DIALOG.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::SAMPLE {

SampleApplyTable::SampleApplyTable(QWidget* parent)
    : QWidget(parent), ui(new Ui::SampleApplyTable{}) {
    ui->setupUi(this);

    /// table init
    {
        table = ui->tableWidget;
        table->clearContents();
        QList<QString> titles;
        for (int i = 0; i < (int)TableAttribute::Enum_Count; i += 1) {
            titles << QString();
        }
        titles[(int)TableAttribute::id]          = QString("编号");
        titles[(int)TableAttribute::type]        = QString("类型");
        titles[(int)TableAttribute::isEmergency] = QString("优先级");
        titles[(int)TableAttribute::project]     = QString("项目");
        titles[(int)TableAttribute::location]    = QString("位置");
        titles[(int)TableAttribute::tubeType]    = QString("样本管");
        UTILS::init_table_mode(table);
        UTILS::init_table_titles(table, titles);
    }

    /// 按钮组
    {
        m_ui_btnGroup = ButtonBottomGroup::Get();
        m_ui_btnGroup->show();
        m_ui_btnGroup->RemoveAllButtons();
        m_ui_btnGroup->Set_refreshFunc(std::bind(&SampleApplyTable::refresh_bottomBtnList, this));
        refresh_bottomBtnList();
    }

    /// 设置缓存内容
    for (auto&& entity : CONTROLLER::SampleApply::instance().Select_all_fromDB()) {
        this->Append_oneLine(entity);
    }
}

SampleApplyTable::~SampleApplyTable() {
    m_ui_btnGroup->hide();
    delete ui;
}

void SampleApplyTable::refresh_bottomBtnList() {
    QList<QString>                   names;
    QList<std::function<void(void)>> taskList;
    if (false) {
    } else {
        names << QString("删除");
        taskList << std::bind(&SampleApplyTable::Delete_oneLine, this);
    }
    m_ui_btnGroup->Set_BtnList(names, taskList);
}

void SampleApplyTable::Append_oneLine(SampleApplyTable::entity_item entity) {
    const int curRowCnt = table->rowCount();
    table->insertRow(curRowCnt);
    table->setRowHeight(curRowCnt, UTILS::table_auto_rowHeight);
    for (int i = 0; i < table->columnCount(); i += 1) {
        auto* item = new QTableWidgetItem();
        item->setTextAlignment(Qt::AlignCenter);
        table->setItem(curRowCnt, i, item);
    }

    table->item(curRowCnt, (int)TableAttribute::id)->setText(entity->Get_idString());
    table->item(curRowCnt, (int)TableAttribute::project)
        ->setText(entity->table[entity_attr::projectList]);

    auto sample = entity->Get_relatedSample();
    if (sample) {
        using sample_attr = CONTROLLER::Sample::entity_attr;
        table->item(curRowCnt, (int)TableAttribute::type)
            ->setText(sample->table[sample_attr::detectType]);
        table->item(curRowCnt, (int)TableAttribute::isEmergency)
            ->setText(sample->table[sample_attr::emergency_sign]);
        table->item(curRowCnt, (int)TableAttribute::location)
            ->setText(sample->table[sample_attr::location]);
        table->item(curRowCnt, (int)TableAttribute::tubeType)
            ->setText(sample->table[sample_attr::sampleTube]);
    }
}

void SampleApplyTable::Delete_oneLine() {
    const int idx = table->currentRow();
    if (idx < 0) {
        DIALOG::Create_DWarningDialog("未选中有效项");
        return;
    }

    auto dialog = DIALOG::Create_DIsVerify("确认需要卸载？");
    connect(dialog, &DIALOG::DIsVerify::signal_yes, this, [this, idx]() {
        QString id     = table->item(idx, (int)TableAttribute::id)->text();
        auto    entity = CONTROLLER::SampleApply::instance().Select_byId_fromDB(id);
        entity->Delete_toDB();
        qInfo() << "remove-db-entity" << entity->to_string();
        table->removeRow(idx);
    });
}

void SampleApplyTable::showEvent(QShowEvent* event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}

}  // namespace WIDGET::SAMPLE
