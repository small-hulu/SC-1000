#include "WSetting_DEditDoctor.h"

#include <QListView>

#include "ui_WSetting_DEditDoctor.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::SETTING {
DEditDoctor::DEditDoctor(QWidget *parent) : QWidget(parent), ui(new Ui::DEditDoctor) {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);
    setAttribute(Qt::WA_DeleteOnClose);

    /// connect
    {
        connect(ui->btn_no, &QPushButton::clicked, this, [this]() { close(); });
        connect(ui->btn_yes, &QPushButton::clicked, this, [this]() {
            emit signal_yes();
            close();
        });
    }
}

DEditDoctor::~DEditDoctor() {
    delete ui;
}

DEditDoctor::Info DEditDoctor::Get_info() {
    Info info;
    info.name       = ui->edit_name->text();
    info.canTest    = ui->btn_canTest->isChecked();
    info.canAudit   = ui->btn_canAudit->isChecked();
    info.canInspect = ui->btn_canInspect->isChecked();
    return info;
}

void DEditDoctor::showEvent(QShowEvent *event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}

}  // namespace WIDGET::SETTING
