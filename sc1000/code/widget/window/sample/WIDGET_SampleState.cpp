/** ***************************************************************************
 * Copyright © 2024 cuber-lotus
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       WIDGET_SampleState.cpp
 * @date       2024-07-05
 * @version    1.0.0
 *
 * @author     cuber-lotus
 * @Email      luyinfeng@smtbio.com
 *
 * @brief
 * 这是一个纯展示的界面
 *
 *****************************************************************************/
#include "WIDGET_SampleState.h"

#include "controller/CONTROLLER_ExpSample.h"
#include "ui_WIDGET_SampleState.h"
#include "widget/modules/WModule_TubeGroup6.h"
#include "widget/modules/WModule_TubeUiItem.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::SAMPLE {
SampleState::SampleState(QWidget *parent) : QWidget(parent), ui(new Ui::SampleState) {
    ui->setupUi(this);

    {
        ui->lab_isEmergency->setText("");
        ui->lab_number->setText("");
        ui->lab_position->setText("");
        ui->lab_project->setText("");
        ui->lab_sampleType->setText("");
        ui->lab_testType->setText("");
        ui->lab_timeStart->setText("");
        ui->lab_timeEnd->setText("");
        ui->lab_tubeType->setText("");
    }

    m_ui_btnGroup = ButtonBottomGroup::Get();
    m_ui_btnGroup->hide();
    new TubeGroup6(ui->widget_tube6);

    QList<QWidget *> tubeWidgetList;
    tubeWidgetList << ui->widget_tube_0;
    tubeWidgetList << ui->widget_tube_1;
    tubeWidgetList << ui->widget_tube_2;
    tubeWidgetList << ui->widget_tube_3;
    tubeWidgetList << ui->widget_tube_4;
    tubeWidgetList << ui->widget_tube_5;
    tubeWidgetList << ui->widget_tube_6;
    tubeWidgetList << ui->widget_tube_7;
    tubeWidgetList << ui->widget_tube_8;
    tubeWidgetList << ui->widget_tube_9;
    tubeWidgetList << ui->widget_tube_10;
    tubeWidgetList << ui->widget_tube_11;
    tubeWidgetList << ui->widget_tube_12;
    tubeWidgetList << ui->widget_tube_13;
    tubeWidgetList << ui->widget_tube_14;
    tubeWidgetList << ui->widget_tube_15;
    tubeWidgetList << ui->widget_tube_16;
    tubeWidgetList << ui->widget_tube_17;
    tubeWidgetList << ui->widget_tube_18;
    tubeWidgetList << ui->widget_tube_19;
    tubeWidgetList << ui->widget_tube_20;
    tubeWidgetList << ui->widget_tube_21;
    tubeWidgetList << ui->widget_tube_22;
    tubeWidgetList << ui->widget_tube_23;
    tubeWidgetList << ui->widget_tube_24;
    tubeWidgetList << ui->widget_tube_25;
    tubeWidgetList << ui->widget_tube_26;
    tubeWidgetList << ui->widget_tube_27;
    tubeWidgetList << ui->widget_tube_28;
    tubeWidgetList << ui->widget_tube_29;

    m_tubeList.clear();
    for (int i = 0; i < tubeWidgetList.size(); i += 1) {
        auto tubeParent = tubeWidgetList[i];
        tubeParent->setStyleSheet("");
        auto tube = new SampleStateItem(tubeParent);
        tube->Set_labelBottom(i);
        tube->Set_StateType((int)Type::ETubeState::Idle);
        tube->Set_ExpType(-1);
        m_tubeList << tube;

        connect(tube, &SampleStateItem::signal_clicked, this,
                [this, i]() { SampleState::response_tubeClicked(i); });
    }

    /// 注意，这个页面都是静态的
    /// 不会改变数据，
    {
        using attr      = ENTITY::ESample::attr;
        auto sampleList = CONTROLLER::Sample::instance().Get_isLoadList();
        for (auto &&sample : sampleList) {
            auto key  = Type::Get_TubeSpaceMark(sample->table[attr::location]);
            auto tube = m_tubeList[(int)key];

            tube->Set_StateType(sample->table[attr::state].toInt());
            tube->Set_ExpType((int)Type::ETubeExp::N_Normal);
        }
    }
}

SampleState::~SampleState() {
    delete ui;
}

void SampleState::response_tubeClicked(int idx) {
    assert(idx < m_tubeList.size());
    ui->lab_isEmergency->setText("");
    ui->lab_number->setText("");
    ui->lab_position->setText("");
    ui->lab_project->setText("");
    ui->lab_sampleType->setText("");
    ui->lab_testType->setText("");
    ui->lab_timeStart->setText("");
    ui->lab_timeEnd->setText("");
    ui->lab_tubeType->setText("");

    using attr  = ENTITY::ESample::attr;
    auto sample = CONTROLLER::Sample::instance().Get_isLoadByLocation(
        Type::Get_TubeSpaceMark((Type::ETubeSpaceMark)idx));
    if (sample == nullptr) {
        return;
    }

    ui->lab_isEmergency->setText(sample->table[attr::emergency_sign]);
    ui->lab_number->setText(sample->Get_idString());
    ui->lab_position->setText(sample->table[attr::location]);
    ui->lab_project->setText(sample->table[attr::project]);
    ui->lab_sampleType->setText(sample->table[attr::sampleTube]);
    ui->lab_testType->setText(sample->table[attr::detectType]);
    ui->lab_tubeType->setText(sample->table[attr::sampleTube]);

    /// 关联到实验信息
    /// todo 目前效果是显示最后一个
    {
        const auto format = ENTITY::EExpSample::time_format;
        QDateTime  startTime;
        QDateTime  endTime;
        auto       expIdList = sample->Get_expIdList();
        for (auto id : expIdList) {
            auto expSample = CONTROLLER::ExpSample::instance().Select_byId_fromDB(id);
            if (expSample == nullptr) {
                qWarning() << QString("实验(%1)丢失").arg(id);
                continue;
            }
            startTime =
                qMax(startTime,
                     QDateTime::fromString(
                         expSample->table[CONTROLLER::ExpSample::entity_attr::timeStart], format));
            endTime =
                qMax(endTime,
                     QDateTime::fromString(
                         expSample->table[CONTROLLER::ExpSample::entity_attr::timeEnd], format));
        }
        ui->lab_timeStart->setText(startTime.toString(format));
        if (sample->table[attr::state].toInt() ==
            (int)CONTROLLER::Sample::entity_type::TubeState::Complete) {
            ui->lab_timeEnd->setText(endTime.toString(format));
        } else {
            ui->lab_timeEnd->setText("");
        }
    }
}
}  // namespace WIDGET::SAMPLE
