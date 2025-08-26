#include "WIDGET_HomepageInstrumentStatus.h"

#include "ui_WIDGET_HomepageInstrumentStatus.h"

namespace WIDGET::HOMEPAGE {
InstrumentStatus::InstrumentStatus(QWidget *parent)
    : QWidget(parent), ui(new Ui::InstrumentStatus) {
    ui->setupUi(this);
}

InstrumentStatus::~InstrumentStatus() {
    delete ui;
}
}  // namespace WIDGET::HOMEPAGE
