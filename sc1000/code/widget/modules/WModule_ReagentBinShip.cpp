#ifndef WSAMPLE_SAMPLEAPPLY_H
#define WSAMPLE_SAMPLEAPPLY_H

#include "WModule_ReagentBinShip.h"

#include <QButtonGroup>

#include "controller/CONTROLLER_Consumable.h"
#include "controller/device/CDevice.hpp"
#include "global/GLOBAL_DeviceState.h"
#include "ui_WModule_ReagentBinShip.h"
#include "widget/dialog/DIALOG.h"
#include "widget/utils/UTILS_Resize.h"

namespace WIDGET {

ReagentBinShip::ReagentBinShip(QWidget* parent) : QWidget(parent), ui(new Ui::ReagentBinShip) {
    ui->setupUi(this);
    /// init
    {
        initializeTestTubes();
        updateButtonLabels(ReagentBinCache::GroupIdx_Cache - 1);
        initializeButtonGroups();

        using namespace CONTROLLER;
        ui->label_jifaye_num->setText(Consumable::instance().Get_describe_for_Substrat());
        ui->label_feiliao_num->setText(Consumable::instance().Get_describe_for_WasteTank());
    }
    /// connect
    {
        connect(ui->btn_rotate, &QPushButton::clicked, this, [this]() {
            const int idx = m_selectedGroupIdx + 1;
            updateButtonLabels(m_selectedGroupIdx);
            reset_idle_label_show();
            ReagentBinCache::GroupIdx_Cache = idx;
            rotate_reagentBin(ReagentBinCache::GroupIdx_Cache);
        });

        connect(ui->btn_rotate_left, &QPushButton::clicked, this, [this]() {
            updateButtonLabels(ReagentBinCache::GroupIdx_Cache - 1);
            reset_idle_label_show();
            /// [1, 10] -> [0, 10)
            int next                        = ReagentBinCache::GroupIdx_Cache - 1;
            next                            = ((next - 1) + Dial_Max) % Dial_Max;
            /// [0, 10) -> [1, 10]
            ReagentBinCache::GroupIdx_Cache = next + 1;
            rotate_reagentBin(ReagentBinCache::GroupIdx_Cache);
        });

        connect(ui->btn_rotate_right, &QPushButton::clicked, this, [this]() {
            updateButtonLabels(ReagentBinCache::GroupIdx_Cache - 1);
            reset_idle_label_show();
            /// [1, 10] -> [0, 10)
            int next                        = ReagentBinCache::GroupIdx_Cache - 1;
            next                            = ((next + 1) + Dial_Max) % Dial_Max;
            /// [0, 10) -> [1, 10]
            ReagentBinCache::GroupIdx_Cache = next + 1;
            rotate_reagentBin(ReagentBinCache::GroupIdx_Cache);
        });
    }
}

ReagentBinShip::~ReagentBinShip() {
    delete ui;
}

// 按钮信号槽
void ReagentBinShip::initializeButtonGroups() {
    for (int group = 0; group < 10; ++group) {
        QButtonGroup* buttonGroup = new QButtonGroup(this);
        QString       buttonName  = QString("pushButton_reagent_%1").arg(group + 1);

        QPushButton* button = findChild<QPushButton*>(buttonName);
        if (button) {
            buttonGroup->addButton(button, group + 1);
            connect(button, &QPushButton::clicked, this, &ReagentBinShip::handleButtonClicked);
        }
    }
}

void ReagentBinShip::handleButtonClicked() {
    reset_idle_label_show();
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QString                 buttonText = button->text();
        QString                 buttonName = button->objectName();
        QRegularExpression      regex("pushButton_reagent_(\\d+)");
        QRegularExpressionMatch match = regex.match(buttonName);
        if (match.hasMatch()) {
            QString xValue     = match.captured(1);
            m_selectedGroupIdx = buttonText.toInt() - 1;
            QString imagePath  = QString(":/beijing/select_%1.png").arg(xValue);
            QLabel* label      = findChild<QLabel*>(QString("label_%1").arg(xValue));

            if (label) {
                QString styleSheet = QString(
                                         "background: rgba(255, 255, 255, 0.08);"
                                         "image: url(%1);")
                                         .arg(imagePath);
                label->setStyleSheet(styleSheet);
            }
        }
    }
}

void ReagentBinShip::updateButtonLabels(int m_testTubes_ptr) {
    for (int group = 0; group < Dial_Max; ++group) {
        int          t_grop       = (m_testTubes_ptr + group + Dial_Max) % Dial_Max;
        QString      buttonName   = QString("pushButton_reagent_%1").arg(group + 1);
        QPushButton* button1      = findChild<QPushButton*>(buttonName);
        QString      testTubeData = m_testTubes.at(t_grop);
        if (button1) {
            button1->setText(testTubeData);
        }
    }
}

void ReagentBinShip::reset_idle_label_show() {
    for (int i = 1; i < Dial_Max + 1; ++i) {
        QString imagePath = QString(":/beijing/sample_%1.png").arg(i);
        QLabel* label     = findChild<QLabel*>(QString("label_%1").arg(i));
        if (label) {
            QString styleSheet = QString(
                                     "background: rgba(255, 255, 255, 0.08);"
                                     "image: url(%1);")
                                     .arg(imagePath);
            label->setStyleSheet(styleSheet);
        }
    }
}

void ReagentBinShip::rotate_reagentBin(int idx) {
    if (idx < 0 || idx > Dial_Max) {
        DIALOG::Create_DWarningDialog("指定组号错误");
        return;
    }

    auto task = [=]() {
        CONTROLLER::CDevice::DReagentBin::ToFront_forSample(idx);
        emit signal_curGroup(idx);
    };

    if (GLOBAL::EquipmentRecord::state == GLOBAL::MachineState::Waring) {
        auto dialog = DIALOG::Create_DIsVerify("仪器异常，是否继续操作？");
        connect(dialog, &DIALOG::DIsVerify::signal_yes, this, [task]() { task(); });
    } else {
        task();
    }
}

void ReagentBinShip::initializeTestTubes() {
    for (int i = 1; i <= 10; ++i) {
        m_testTubes.append(QString::number(i));
    }
}

void ReagentBinShip::showEvent(QShowEvent* event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}

}  // namespace WIDGET

#endif
