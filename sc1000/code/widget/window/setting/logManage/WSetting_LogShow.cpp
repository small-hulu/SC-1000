#include "WSetting_LogShow.h"

#include "controller/CONTROLLER_Log.h"
#include "ui_WSetting_LogShow.h"
#include "widget/dialog/DIALOG.h"
#include "widget/modules/WModule_TablePageTurnSwitch.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::SETTING {

LogShow::LogShow(QWidget* parent) : QWidget(parent), ui(new Ui::LogShow{}) {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);

    {
        QList<QString> titles;
        titles << QString("类型");
        titles << QString("时间");
        titles << QString("内容");
        table = ui->tableWidget;
        UTILS::init_table_clear(table);
        UTILS::init_table_turnForm_init(table);
        UTILS::init_table_titles(table, titles);
        UTILS::init_table_rowCount(table, Row_Count);
        UTILS::init_table_colWidth(table, {1, 1, 4});
    }

    /// pageSwitch
    {
        pageSwitch = new TablePageTurnSwitch(ui->widget_switch);
        connect(pageSwitch, &TablePageTurnSwitch::signal_curRange, this, &LogShow::show_range);
        const int all = CONTROLLER::Log::Get_allCount();
        pageSwitch->Build_By_ItemCount(all, Row_Count);
        pageSwitch->signal_curRange(1, qMin(all, Row_Count));
    }

    /// combo
    {
        UTILS::init_combo_reset(ui->combo_type);
        auto combo = ui->combo_type;
        combo->addItem("全部", -1);
        auto qenum = ENTITY::LogEnum::Get_qEnum();
        for (int i = 0; i < qenum.keyCount(); i += 1) {
            auto evalue = qenum.value(i);
            combo->addItem(ENTITY::Log::Get_logTypeString(evalue), evalue);
        }

        connect(combo, (void(QComboBox::*)(int))(&QComboBox::currentIndexChanged), this,
                &LogShow::switch_type);
    }

    /// clear
    {
        /// 根据当前选择的删除
        connect(ui->btn_clear, &QPushButton::clicked, this, &LogShow::clear_log);
    }
}

LogShow::~LogShow() {
    delete ui;
}

void LogShow::show_range(int start, int end) {
    if (start <= 0 || end <= 0 || end < start) {
        UTILS::init_table_clear_content(table);
        return;
    }

    const int type = ui->combo_type->currentData().toInt();
    auto      res  = CONTROLLER::Log::Get_rangeEntity_byType(start, end, type);
    assert(res.size() <= table->rowCount());
    for (int i = 0; i < table->rowCount(); i += 1) {
        if (i < res.size()) {
            using attr = ENTITY::Log::attr;

            table->item(i, 0)->setText(res[i].Get_TypeString());
            table->item(i, 1)->setText(res[i].table[attr::time]);
            table->item(i, 2)->setText(res[i].table[attr::message]);
        } else {
            for (int j = 0; j < table->columnCount(); j += 1) {
                auto item = table->item(i, j);
                item->setText("");
            }
        }
    }
}

void LogShow::switch_type() {
    const int type = ui->combo_type->currentData().toInt();
    const int all  = CONTROLLER::Log::Get_allCount_byType(type);
    pageSwitch->Build_By_ItemCount(all, Row_Count);
    pageSwitch->signal_curRange(1, qMin(all, Row_Count));
}

void LogShow::clear_log() {
    auto dialog = DIALOG::Create_DIsVerify("确认删除？");
    connect(dialog, &DIALOG::DIsVerify::signal_yes, this, [this]() {
        for (int i = 0; i < table->rowCount(); i += 1) {
            for (int j = 0; j < table->columnCount(); j += 1) {
                auto item = table->item(i, j);
                item->setText("");
            }
        }

        const int type = ui->combo_type->currentData().toInt();
        CONTROLLER::Log::Clear_byType(type);
        pageSwitch->Build_By_ItemCount(0, Row_Count);
        pageSwitch->signal_curRange(1, qMin(0, Row_Count));
    });
}

void LogShow::showEvent(QShowEvent* event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}

}  // namespace WIDGET::SETTING
