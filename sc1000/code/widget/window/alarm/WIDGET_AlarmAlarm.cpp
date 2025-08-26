#include "WIDGET_AlarmAlarm.h"

#include "alarm/WAlarm_AlarmTable.h"
#include "ui_WIDGET_AlarmAlarm.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::ALARM {
AlarmAlarm::AlarmAlarm(QWidget *parent) : QWidget(parent), ui(new Ui::AlarmAlarm) {
    ui->setupUi(this);

    UTILS::resize_layout_stretch(ui->verticalLayout, UTILS::params_stretch_mainFrame());

    new AlarmTable(ui->widget_up);
}

AlarmAlarm::~AlarmAlarm() {
    delete ui;
}
}  // namespace WIDGET::ALARM
