#include "WAlarm_AlarmTable.h"

#include "ui_WAlarm_AlarmTable.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::ALARM {
AlarmTable::AlarmTable(QWidget *parent) : QWidget(parent), ui(new Ui::AlarmTable) {
    ui->setupUi(this);

    /// init
    {
        auto table = ui->tableWidget;
        UTILS::init_table_mode(table);
        QList<QString> titles;
        titles << QString("模块");
        titles << QString("级别");
        titles << QString("时间");
        titles << QString("内容");

        table->setColumnCount(titles.size());
        table->setHorizontalHeaderLabels(titles);
    }
}

AlarmTable::~AlarmTable() {
    delete ui;
}

void AlarmTable::showEvent(QShowEvent *event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}
}  // namespace WIDGET::ALARM
