#include "WIDGET_HelpSpecification.h"

#include "ui_WIDGET_HelpSpecification.h"

namespace WIDGET::HELP {
Specification::Specification(QWidget *parent) : QWidget(parent), ui(new Ui::Specification) {
    ui->setupUi(this);
}

Specification::~Specification() {
    delete ui;
}
}  // namespace WIDGET::HELP
