#include "WIDGET_Main_MTop.h"

#include <QDebug>
#include <QPushButton>
#include <QTimer>

#include "MTop_AppState.h"
#include "MTop_ExpState.h"
#include "communication/controller/Event.h"
#include "controller/AppEvent.h"
#include "controller/CONTROLLER_Reagent.h"
#include "controller/CONTROLLER_SampleApply.h"
#include "ui_WIDGET_Main_MTop.h"
#include "widget/dialog/DIALOG.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::MAIN {

MTop::MTop(QWidget* parent) : QWidget(parent), ui(new Ui::MTop{}) {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);

    /// 一级目录
    /// + 顶层透明化
    /// + 添加底层按钮的事件
    {
        m_qss_onIcon << QString(":/suzhou/top_on_sample.png");
        m_qss_onIcon << QString(":/suzhou/top_on_history.png");
        m_qss_onIcon << QString(":/suzhou/top_on_reagent.png");
        m_qss_onIcon << QString(":/suzhou/top_on_calibration.png");
        m_qss_onIcon << QString(":/suzhou/top_on_quality.png");
        m_qss_onIcon << QString(":/suzhou/top_on_maintrain.png");
        m_qss_onIcon << QString(":/suzhou/top_on_setting.png");
        m_qss_onIcon << QString(":/suzhou/top_on_help.png");

        m_qss_offIcon << QString(":/suzhou/top_off_sample.png");
        m_qss_offIcon << QString(":/suzhou/top_off_history.png");
        m_qss_offIcon << QString(":/suzhou/top_off_reagent.png");
        m_qss_offIcon << QString(":/suzhou/top_off_calibration.png");
        m_qss_offIcon << QString(":/suzhou/top_off_quality.png");
        m_qss_offIcon << QString(":/suzhou/top_off_maintrain.png");
        m_qss_offIcon << QString(":/suzhou/top_off_setting.png");
        m_qss_offIcon << QString(":/suzhou/top_off_help.png");

        /// 底层按钮
        m_titleBtnList << ui->btn_title_sample;
        m_titleBtnList << ui->btn_title_history;
        m_titleBtnList << ui->btn_title_reagent;
        m_titleBtnList << ui->btn_title_calibration;
        m_titleBtnList << ui->btn_title_quality;
        m_titleBtnList << ui->btn_title_maintrain;
        m_titleBtnList << ui->btn_title_setting;
        m_titleBtnList << ui->btn_title_help;

        /// 对应的图标
        m_titleIconList << ui->widget_title_sample;
        m_titleIconList << ui->widget_title_history;
        m_titleIconList << ui->widget_title_reagent;
        m_titleIconList << ui->widget_title_calibration;
        m_titleIconList << ui->widget_title_quality;
        m_titleIconList << ui->widget_title_maintrain;
        m_titleIconList << ui->widget_title_setting;
        m_titleIconList << ui->widget_title_help;

        /// 对应的文字
        m_titleLabelList << ui->label_title_sample;
        m_titleLabelList << ui->label_title_history;
        m_titleLabelList << ui->label_title_reagent;
        m_titleLabelList << ui->label_title_calibration;
        m_titleLabelList << ui->label_title_quality;
        m_titleLabelList << ui->label_title_maintrain;
        m_titleLabelList << ui->label_title_setting;
        m_titleLabelList << ui->label_title_help;

        assert(m_titleBtnList.size() == m_titleIconList.size());
        assert(m_titleBtnList.size() == m_titleLabelList.size());
        assert(m_titleBtnList.size() == m_qss_onIcon.size());
        assert(m_titleBtnList.size() == m_qss_offIcon.size());

        for (auto&& w : m_titleIconList) {
            w->setAttribute(Qt::WA_TransparentForMouseEvents);
        }
        for (auto&& w : m_titleLabelList) {
            w->setAttribute(Qt::WA_TransparentForMouseEvents);
        }

        /// 一级目录的切换
        for (int i = 0; i < m_titleBtnList.size(); i += 1) {
            auto btn = m_titleBtnList[i];
            connect(btn, &QPushButton::clicked, this, [this, i]() {
                sLog_test() << "1st LevelPage switch to page" << m_titleLabelList[i]->text();
                emit signal_switchPage(i);
            });
        }
    }

    /// app 开关&缩小
    {
        connect(ui->btn_app_close, &QPushButton::clicked, this, &AppState::App_close);
        connect(ui->btn_app_min, &QPushButton::clicked, this, &AppState::App_min);
    }

    /// init-setting
    {
        QList<QWidget*> transparentList;
        transparentList << ui->label_exp_start;
        transparentList << ui->label_exp_stop;
        transparentList << ui->widget_exp_start;
        transparentList << ui->widget_exp_stop;
        for (auto&& w : transparentList) {
            w->setAttribute(Qt::WA_TransparentForMouseEvents);
        }

        /// 整机实验的控制
        connect(ui->btn_exp_start, &QPushButton::clicked, this, [this]() {
            qInfo() << QString("btn-clicked [%1]").arg(ui->label_exp_start->text());
            sLog_test() << QString("btn-clicked [%1]").arg(ui->label_exp_start->text());
            EXP_RangeMarginCheck();
        });
        connect(ui->btn_exp_stop, &QPushButton::clicked, this, [this]() {
            qInfo() << QString("btn-clicked [%1]").arg(ui->label_exp_stop->text());
            sLog_test() << QString("btn-clicked [%1]").arg(ui->label_exp_stop->text());
            Exp_Stop();
        });

        auto&& appEvent = CONTROLLER::AppEvent::instance();
        connect(&appEvent, &CONTROLLER::AppEvent::signal_expStateHint, this,
                [this](bool flag, const QString& msg) {
                    if (flag == false) {
                        ExpStateController::instance().Reset();
                        Exp_ModifyExpBtnStyle(ExpBtnState::setStart);
                        Exp_ModifyExpBtnStyle(ExpBtnState::setStopDisable);
                    }
                });

        auto&& comEvent = COM::Event::instance();
        /// 耗材的异常
        connect(&comEvent, &COM::Event::signal_consumeWaring, this, [this](const QString& msg) {
            Exp_ModifyExpBtnStyle(ExpBtnState::setStart);
            ExpStateController::instance().Set_State(ExpStateController::ExpState::pause_byDevice);
        });

        auto&& esc = ExpStateController::instance();
        connect(&esc, &ExpStateController::signal_timeout_forceStop, this, [this]() {
            qInfo() << "超时异常 强制终止";
            sLog_test() << "超时异常 强制终止";
            Exp_Stop();
        });
    }
    /// init
    Exp_ModifyExpBtnStyle(ExpBtnState::setStopDisable);
}

MTop::~MTop() {
    delete ui;
}

void MTop::Click_indexBtn(int idx) {
    const QString qss_btn   = QString(R"(
background: %1;
border-radius : 0px;
)");
    const QString qss_icon  = QString(R"(
background: transparent;
image: url(%1);
)");
    const QString qss_title = QString(R"(
background: transparent;
width: 51px;
height: 25px;
font-family: PingFang SC;
font-weight: 500;
font-size: 26px;
color: %1;
line-height: 42px;
)");

    assert(idx < m_titleBtnList.size());
    for (int i = 0; i < m_titleBtnList.size(); i += 1) {
        m_titleBtnList[i]->setStyleSheet(qss_btn.arg("#f7f8fa"));
        m_titleIconList[i]->setStyleSheet(qss_icon.arg(m_qss_offIcon[i]));
        m_titleLabelList[i]->setStyleSheet(qss_title.arg("#000000"));
    }

    m_titleBtnList[idx]->setStyleSheet(qss_btn.arg("#37ADBA"));
    m_titleIconList[idx]->setStyleSheet(qss_icon.arg(m_qss_onIcon[idx]));
    m_titleLabelList[idx]->setStyleSheet(qss_title.arg("#ffffff"));
}

void MTop::Exp_ModifyExpBtnStyle(MTop::ExpBtnState btnState) {
    switch (btnState) {
    case ExpBtnState::setStart: {
        const QString qss = R"(
background: #19BC90;

border-top-left-radius : 45px;
border-top-right-radius : 0px;
border-bottom-left-radius : 45px;
border-bottom-right-radius : 0px;
)";
        ui->btn_exp_start->setStyleSheet(qss);
        ui->label_exp_start->setText("开始");
    } break;
    case ExpBtnState::setPause: {
        const QString qss = R"(
background: #37adba;

border-top-left-radius : 45px;
border-top-right-radius : 0px;
border-bottom-left-radius : 45px;
border-bottom-right-radius : 0px;
)";
        ui->btn_exp_start->setStyleSheet(qss);
        ui->label_exp_start->setText("暂停");
    } break;
    case ExpBtnState::setStopEnable: {
        ui->btn_exp_stop->setEnabled(true);
    } break;
    case ExpBtnState::setStopDisable: {
        ui->btn_exp_stop->setEnabled(false);
    } break;
    default: {
        qCritical() << "枚举状态错误";
    } break;
    }
}

void MTop::Exp_StartAndPause() {
    auto&& esc = ExpStateController::instance();
    switch (esc.Get_State()) {
    case ExpStateController::ExpState::idle: {
        esc.Exp_start();
        Exp_ModifyExpBtnStyle(ExpBtnState::setPause);
        Exp_ModifyExpBtnStyle(ExpBtnState::setStopEnable);
    } break;
    case ExpStateController::ExpState::running: {
        esc.Exp_pause();
        Exp_ModifyExpBtnStyle(ExpBtnState::setStart);
        Exp_ModifyExpBtnStyle(ExpBtnState::setStopEnable);
    } break;
    case ExpStateController::ExpState::pause_byUser: {
        esc.Exp_start();
        Exp_ModifyExpBtnStyle(ExpBtnState::setPause);
        Exp_ModifyExpBtnStyle(ExpBtnState::setStopEnable);
    } break;
    case ExpStateController::ExpState::pause_byDevice: {
        esc.Exp_start();
        Exp_ModifyExpBtnStyle(ExpBtnState::setPause);
        Exp_ModifyExpBtnStyle(ExpBtnState::setStopEnable);
    } break;
    case ExpStateController::ExpState::stop: {
        qCritical() << "枚举 逻辑异常";
    } break;
    default: {
        qCritical() << "枚举状态错误";
    } break;
    }
}

void MTop::Exp_Stop() {
    auto&& esc = ExpStateController::instance();
    switch (esc.Get_State()) {
    case ExpStateController::ExpState::idle: {
        qCritical() << "枚举 逻辑异常";
    } break;
    case ExpStateController::ExpState::running: {
        esc.Exp_stop();
        Exp_ModifyExpBtnStyle(ExpBtnState::setStart);
        Exp_ModifyExpBtnStyle(ExpBtnState::setStopDisable);
    } break;
    case ExpStateController::ExpState::pause_byUser: {
        esc.Exp_stop();
        Exp_ModifyExpBtnStyle(ExpBtnState::setStart);
        Exp_ModifyExpBtnStyle(ExpBtnState::setStopDisable);
    } break;
    case ExpStateController::ExpState::pause_byDevice: {
        esc.Exp_stop();
        Exp_ModifyExpBtnStyle(ExpBtnState::setStart);
        Exp_ModifyExpBtnStyle(ExpBtnState::setStopDisable);
    } break;
    case ExpStateController::ExpState::stop: {
        qCritical() << "枚举 逻辑异常";
    } break;
    default: {
        qCritical() << "枚举状态错误";
    } break;
    }
}

void MTop::EXP_RangeMarginCheck() {
    using namespace CONTROLLER;
    auto                  reaSet   = Reagent::instance().Select_isLoadList();
    auto                  applySet = SampleApply::instance().Select_all_fromDB();
    QList<QList<QString>> projectLists;
    while (applySet.size()) {
        auto apply = applySet.back()->Get_projectList();
        projectLists.push_back(apply);
        applySet.pop_back();
    }
    QHash<QString, int> countMap;
    for (const QList<QString>& list : projectLists) {
        for (const QString& str : list) {
            countMap[str]++;
        }
    }
    for (const auto& pair : countMap.keys()) {
        for (auto reagentItem : reaSet) {
            if (pair != reagentItem->table[Reagent::entity_attr::project]) {
                continue;
            }
            auto qrcode   = reagentItem->Get_qrcodeEntity();
            namespace key = QRCODE::KeyRea;
            if (countMap.value(pair) <
                (qrcode[key::base_tq].toString().toInt() -
                 reagentItem->table[Reagent::entity_attr::measuredTimes].toInt())) {
                break;
            } else {
                auto dialog = DIALOG::Create_DIsVerify(pair + "试剂不足，请更换试剂船");
//                connect(dialog, &DIALOG::DIsVerify::signal_yes, this,
//                        [this]() { Exp_StartAndPause(); });
                return;
            }
        }
    }
    Exp_StartAndPause();
}
}  // namespace WIDGET::MAIN
