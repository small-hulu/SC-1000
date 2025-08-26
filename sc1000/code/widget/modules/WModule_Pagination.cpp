#include "WModule_Pagination.h"

#include "ui_WModule_Pagination.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET {
Pagination::Pagination(QWidget *parent) : QWidget(parent), ui(new Ui::Pagination) {
    ui->setupUi(this);

    /// init-ui
    {
        ui->lab_cur->setNum(1);
        ui->lab_all->setNum(1);
    }

    {
        connect(ui->btn_left, &QPushButton::clicked, this, [this]() {
            const int num = ui->lab_cur->text().toInt();
            switch_page(num - 1);
        });

        connect(ui->btn_right, &QPushButton::clicked, this, [this]() {
            const int num = ui->lab_cur->text().toInt();
            switch_page(num + 1);
        });
    }
}

Pagination::~Pagination() {
    delete ui;
}

void Pagination::Set_pageAll(int num) {
    ui->lab_all->setNum(num);
}

int Pagination::Get_pageAll() {
    return ui->lab_all->text().toInt();
}

void Pagination::Set_pageCur(int num) {
    ui->lab_cur->setNum(num);
}

int Pagination::Get_pageCur() {
    return ui->lab_cur->text().toInt();
}

bool Pagination::switch_page(int num) {
    const int all = Get_pageAll();
    if (num > all or num <= 0) {
        return false;
    }

    ui->lab_cur->setNum(num);
    emit signal_pageSwitched(num);
    return true;
}

void Pagination::showEvent(QShowEvent *event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}
}  // namespace WIDGET
