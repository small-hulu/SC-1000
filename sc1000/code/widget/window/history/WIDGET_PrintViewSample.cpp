#include "WIDGET_PrintViewSample.h"

#include <QPlainTextEdit>
#include <QTextEdit>

#include "ui_widget_printviewsample.h"
WIDGET_PrintViewSample::WIDGET_PrintViewSample(QWidget *parent)
    : QWidget(parent), ui(new Ui::WIDGET_PrintViewSample) {
    ui->setupUi(this);
}

WIDGET_PrintViewSample::WIDGET_PrintViewSample(QWidget *parent, QPrintPreviewWidget *widget)
    : QWidget(parent), ui(new Ui::WIDGET_PrintViewSample), pre_widget(widget) {
    ui->setupUi(this);
    if (pre_widget) {
        pre_widget->setParent(ui->widget_preview);
    }
    setAttribute(Qt::WA_StyledBackground);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
}

WIDGET_PrintViewSample::~WIDGET_PrintViewSample() {
    delete ui;
}

void WIDGET_PrintViewSample::setTemp_json(const QJsonObject &value) {
    temp_json = value;
}

QJsonObject WIDGET_PrintViewSample::getTemp_json() const {
    return temp_json;
}

void WIDGET_PrintViewSample::on_pushButton_print_clicked() {
    emit send_print_data(temp_json);
}

void WIDGET_PrintViewSample::on_pushButton_back_clicked() {
    close();
}

void WIDGET_PrintViewSample::on_pushButton_narrow_btn_clicked() {
    if (pre_widget) {
        pre_widget->zoomOut();
    }
}

void WIDGET_PrintViewSample::on_pushButton_enlarge_btn_clicked() {
    if (pre_widget) {
        pre_widget->zoomIn();
    }
}
