#include "WModule_TableHeader.h"

#include "ui_WModule_TableHeader.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET {
TableHeader::TableHeader(QWidget *parent) : QWidget(parent), ui(new Ui::TableHeader) {
    ui->setupUi(this);
}

TableHeader::TableHeader(const QString &msg, QWidget *parent) : TableHeader(parent) {
    Set_title(msg);
}

TableHeader::~TableHeader() {
    delete ui;
}

void TableHeader::Set_title(const QString &msg) {
    ui->label->setText(msg);
}

void TableHeader::showEvent(QShowEvent *event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}
}  // namespace WIDGET
