#include "WModule_TablePageTurnSwitch.h"

#include "WModule_SwitchList.h"
#include "ui_WModule_TablePageTurnSwitch.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET {
TablePageTurnSwitch::TablePageTurnSwitch(QWidget *parent)
    : QWidget(parent), ui(new Ui::TablePageTurnSwitch) {
    ui->setupUi(this);
    ui->edit_msg->setEnabled(false);

    /// connect
    {
        connect(ui->btn_left, &QPushButton::clicked, this, [this]() {
            const int curPage = ui->edit_msg->text().toInt();
            check_page(curPage - 1);
        });
        connect(ui->btn_right, &QPushButton::clicked, this, [this]() {
            const int curPage = ui->edit_msg->text().toInt();
            check_page(curPage + 1);
        });
    }
}

TablePageTurnSwitch::~TablePageTurnSwitch() {
    delete ui;
}

void TablePageTurnSwitch::Build_By_ItemCount(int all, int each) {
    this->all  = all;
    this->each = each;
    ui->lab_all->setText(QString::number(all));
    ui->edit_msg->setText("1");
}

void TablePageTurnSwitch::check_page(int page) {
    if (page <= 0 || (page - 1) * each > all) {
        return;
    }

    ui->edit_msg->setText(QString::number(page));
    const int start = (page - 1) * each + 1;
    const int end   = qMin(page * each, all);
    emit      signal_curRange(start, end);
}

void TablePageTurnSwitch::showEvent(QShowEvent *event) {
    UTILS::resize_widget_fromParent(this);
    const int width = this->width();
    if (width <= 550) {
        ui->label->hide();
        ui->lab_all->hide();
        ui->label_3->hide();
        ui->horizontalSpacer_4->changeSize(0, 0, QSizePolicy::Ignored, QSizePolicy::Ignored);
    }
    return QWidget::showEvent(event);
}
}  // namespace WIDGET
