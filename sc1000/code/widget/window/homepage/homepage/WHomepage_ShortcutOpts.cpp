#include "WHomepage_ShortcutOpts.h"

#include "controller/AppEvent.h"
#include "controller/device/CDevice_DMaintain.h"
#include "ui_WHomepage_ShortcutOpts.h"
#include "widget/dialog/DIALOG.h"
#include "widget/dialog/DIALOG_DInfoBottom.h"
#include "widget/utils/UTILS_Resize.h"

namespace WIDGET::HOMEPAGE {
ShortcutOpts::ShortcutOpts(QWidget *parent) : QWidget(parent), ui(new Ui::ShortcutOpts) {
    ui->setupUi(this);

    /// connect
    {
        connect(ui->btn_jump_0, &QPushButton::clicked, this, &ShortcutOpts::btnfunc_BootMaintain);
        connect(ui->btn_jump_1, &QPushButton::clicked, this,
                &ShortcutOpts::btnfunc_SampleConsumable);
        connect(ui->btn_jump_2, &QPushButton::clicked, this, &ShortcutOpts::btnfunc_History);
        connect(ui->btn_jump_3, &QPushButton::clicked, this, &ShortcutOpts::btnfunc_CloseMaintain);
    }
}

ShortcutOpts::~ShortcutOpts() {
    delete ui;
}

void ShortcutOpts::btnfunc_BootMaintain() {
    auto dialog = DIALOG::Create_DProgressIndicator();
    dialog->Set_words("开机维护中");
    connect(dialog, &DIALOG::DProgressIndicator::signal_showed, this, [=]() {
        CONTROLLER::CDevice::DMaintain::Homing_allDevice();
        CONTROLLER::CDevice::DMaintain::Homing_pump();
        CONTROLLER::CDevice::DMaintain::Maintain_AbleASubstrate();
        CONTROLLER::CDevice::DMaintain::Maintain_AbleBSubstrate();
        dialog->close();
    });
}

void ShortcutOpts::btnfunc_SampleConsumable() {
    emit CONTROLLER::AppEvent::instance().signal_switchPage(2, 1);
}

void ShortcutOpts::btnfunc_History() {
    emit CONTROLLER::AppEvent::instance().signal_switchPage(1, 1);
}

void ShortcutOpts::btnfunc_CloseMaintain() {
    auto dialog = DIALOG::Create_DProgressIndicator();
    dialog->Set_words("关机维护中");
    connect(dialog, &DIALOG::DProgressIndicator::signal_showed, this, [=]() {
        CONTROLLER::CDevice::DMaintain::Homing_allDevice();
        CONTROLLER::CDevice::DMaintain::Maintain_TubleDisCard();
        dialog->close();
    });
}

void ShortcutOpts::showEvent(QShowEvent *event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}
}  // namespace WIDGET::HOMEPAGE
