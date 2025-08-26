#include "WQuality_QualityResultMain.h"

#include "ui_WQuality_QualityResultMain.h"
#include "widget/dialog/DIALOG.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::QUALITY {
QualityResultMain::QualityResultMain(QWidget* parent)
    : QWidget(parent), ui(new Ui::QualityResultMain) {
    ui->setupUi(this);
    table = ui->table_left;

    /// table-left
    {
        UTILS::init_table_mode(table);

        QList<QString> titles;
        titles << QString("质控名称");
        titles << QString("质控品批号");
        titles << QString("状态");
        titles << QString("申请时间");
        titles << QString("结束时间");
        titles << QString("位置");
        titles << QString("项目");
        titles << QString("结果");
        titles << QString("发光值");

        table->setColumnCount(titles.size());
        table->setHorizontalHeaderLabels(titles);
    }
    /// 设置缓存内容
    for (auto&& entity : CONTROLLER::ExpQuality::instance().Get_theList_expDone()) {
        this->Append_tableLine(entity);
    }
}

QualityResultMain::~QualityResultMain() {
    delete ui;
}

void QualityResultMain::Entity_uninstall() {
    const int idx = table->currentRow();
    if (idx < 0) {
        DIALOG::Create_DWarningDialog("未选中有效项");
        return;
    }

    auto dialog = DIALOG::Create_DIsVerify("确认需要卸载？");
    connect(dialog, &DIALOG::DIsVerify::signal_yes, this, [this, idx]() {
        auto item = m_items[idx];
        m_items.removeOne(item);
        item->Delete_toDB();
        table->removeRow(idx);
    });
}

void QualityResultMain::Append_tableLine(QualityResultMain::entity_item entity) {
    m_items.push_back(entity);
    auto line = get_lineStr(entity);

    assert(line.size() == table->columnCount());

    const int insertRow = 0; // 插入到最上面
    table->insertRow(insertRow);
    table->setRowHeight(insertRow, UTILS::table_auto_rowHeight);
    for (int i = 0; i < line.size(); ++i) {
        auto* item = new QTableWidgetItem(line[i]);
        item->setTextAlignment(Qt::AlignCenter);
        table->setItem(insertRow, i, item);
    }
}

QList<QString> QualityResultMain::get_lineStr(QualityResultMain::entity_item entity) {
    QList<QString> res;
    res << entity->table[entity_attr::name];
    res << entity->table[entity_attr::batchNum];
    res << Type::Get_TubeStateInfo((Type::ETubeState)entity->table[entity_attr::state].toInt())
               .nameChinese;
    res << entity->table[entity_attr::timeStart];
    res << entity->table[entity_attr::timeEnd];
    res << entity->table[entity_attr::location];
    res << entity->table[entity_attr::project];
    res << entity->table[entity_attr::valResult];
    res << entity->table[entity_attr::lightVal];
    return res;
}

void QualityResultMain::showEvent(QShowEvent* event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}
}  // namespace WIDGET::QUALITY
