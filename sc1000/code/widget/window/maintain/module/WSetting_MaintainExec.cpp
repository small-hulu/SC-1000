#include "WSetting_MaintainExec.h"

#include "ui_WSetting_MaintainExec.h"

namespace WIDGET::MAINTAIN {
MaintainExec::MaintainExec(QWidget *parent) : QWidget(parent), ui(new Ui::MaintainExec) {
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked, this, [this]() { emit signal_clicked(); });
}

MaintainExec::~MaintainExec() {
    delete ui;
}
}  // namespace WIDGET::MAINTAIN
