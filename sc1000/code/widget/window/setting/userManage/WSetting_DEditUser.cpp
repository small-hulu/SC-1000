#include "WSetting_DEditUser.h"

#include <QListView>

#include "ui_WSetting_DEditUser.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::SETTING {
DEditUser::DEditUser(QWidget *parent) : QWidget(parent), ui(new Ui::DEditUser) {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);
    setAttribute(Qt::WA_DeleteOnClose);

    /// init-ui
    { UTILS::init_combo_reset(ui->combo_userType); }

    /// connect
    {
        connect(ui->btn_no, &QPushButton::clicked, this, [this]() { close(); });
        connect(ui->btn_yes, &QPushButton::clicked, this, [this]() {
            emit signal_yes();
            close();
        });
    }
}

DEditUser::~DEditUser() {
    delete ui;
}

void DEditUser::Set_user(const ENTITY::User &user) {
    ui->line_userId->setText(user.table[entity_attr::username]);
    ui->line_pdNew->setText(user.table[entity_attr::password]);
    auto combo = ui->combo_userType;
    combo->clear();
    for (int i = 0; i < (int)user.Get_level(); i += 1) {
        combo->addItem(user.get_levelType(i), i);
    }
}

void DEditUser::Get_user(ENTITY::User &user) {
    user.table[entity_attr::username] = ui->line_userId->text();
    user.table[entity_attr::password] = ui->line_pdNew->text();
    auto combo                        = ui->combo_userType;
    int  curCombo                     = combo->currentData().toInt();
    user.table[entity_attr::level]    = QString::number(curCombo);
}

void DEditUser::showEvent(QShowEvent *event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}

}  // namespace WIDGET::SETTING
