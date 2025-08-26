#include "WSetting_DEditCombine.h"

#include <QListView>

#include "controller/CONTROLLER_Config.h"
#include "ui_WSetting_DEditCombine.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::SETTING {
DEditCombine::DEditCombine(QWidget *parent) : QWidget(parent), ui(new Ui::DEditCombine) {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);
    setAttribute(Qt::WA_DeleteOnClose);

    /// init-ui
    { ui->edit_1->setEnabled(false); }

    /// connect
    {
        connect(ui->btn_no, &QPushButton::clicked, this, [this]() { close(); });
        connect(ui->btn_yes, &QPushButton::clicked, this, [this]() {
            affirm();
            emit siganl_yes();
            close();
        });
    }
}

DEditCombine::~DEditCombine() {
    delete ui;
}

void DEditCombine::Set_listMessage(const QList<QString> &msg) {
    auto edit = ui->edit_1;
    for (auto &&s : msg) {
        edit->append(s);
    }
}

void DEditCombine::affirm() {
    CONTROLLER::Config::ProjectCombine::Combine combine;
    combine.name        = ui->edit_0->text();
    combine.projectList = ui->edit_1->toPlainText().split("\n");
    CONTROLLER::Config::ProjectCombine pcom;
    pcom.Append_Combine(combine);
}

void DEditCombine::showEvent(QShowEvent *event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}

}  // namespace WIDGET::SETTING
