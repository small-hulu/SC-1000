#include "WSetting_PaperPrintFormat.h"

#include <QGridLayout>
#include <QPainter>

#include "ui_WSetting_PaperPrintFormat.h"
namespace WIDGET::SETTING {

PaperPrintFormat::PaperPrintFormat(QWidget *parent)
    : QWidget(parent), ui(new Ui::PaperPrintFormat{}) {
    ui->setupUi(this);
}

PaperPrintFormat::~PaperPrintFormat() {
    delete ui;
}

void PaperPrintFormat::showEvent(QShowEvent *event) {
    auto parent = this->parentWidget();
    if (parent) {
        setGeometry(parent->rect());
    }
    QWidget::showEvent(event);
}

}  // namespace WIDGET::SETTING
