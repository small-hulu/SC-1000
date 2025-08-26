#include "WSetting_HospitalDelete.h"

#include "ui_WSetting_HospitalDelete.h"

namespace WIDGET::SETTING {
HospitalDelete::HospitalDelete(QWidget *parent) : QWidget(parent), ui(new Ui::HospitalDelete) {
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked, this, [this]() { emit signal_clicked(); });
}

HospitalDelete::~HospitalDelete() {
    delete ui;
}
}  // namespace WIDGET::SETTING
