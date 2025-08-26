#include "WIDGET_ReagentConsume.h"

#include "controller/CONTROLLER_Consumable.h"
#include "ui_WIDGET_ReagentConsume.h"
#include "widget/dialog/DIALOG.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::REAGENT {
ReagentConsume::ReagentConsume(QWidget* parent) : QWidget(parent), ui(new Ui::ReagentConsume) {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);

    {
        QList<QWidget*> widgetList;
        widgetList << ui->widget_1;
        widgetList << ui->widget_2;
        widgetList << ui->widget_3;
        widgetList << ui->widget_4;
        widgetList << ui->widget_5;
        widgetList << ui->widget_6;
        widgetList << ui->widget_7;

        assert(widgetList.size() == (int)Consume_Type::EType_Count);
        for (int i = 0; i < widgetList.size(); i += 1) {
            auto item = new ReagentConsumeItem(widgetList[i]);
            m_ui_consumeItemList << item;

            connect(item, &ReagentConsumeItem::signal_clicked, this, [this, i]() {
                m_curType = (Consume_Type)i;
                refresh_bottomBtnList();
            });
        }
    }

    {
        m_ui_btnGroup = ButtonBottomGroup::Get();
        m_ui_btnGroup->show();

        m_ui_btnGroup->Set_refreshFunc(std::bind(&ReagentConsume::refresh_bottomBtnList, this));
        refresh_bottomBtnList();
    }

    Reset();

    startTimer(3000);
}

ReagentConsume::~ReagentConsume() {
    m_ui_btnGroup->hide();
    delete ui;
}

void ReagentConsume::Reset() {
    struct Node {
        QString        title;
        QString        hint;
        QList<QString> msgList;
        double         ratio;
    };
    QList<Node> conList;

    auto&& washLiquidEnhance = CONTROLLER::Consumable::instance().cons_washLiquidEnhance;
    conList << Node{washLiquidEnhance.Get_consumeName(), washLiquidEnhance.Get_hintMessage(),
                    washLiquidEnhance.Get_descriptorMessage(), washLiquidEnhance.Get_ratio()};

    auto&& washLiquid = CONTROLLER::Consumable::instance().cons_washLiquid;
    conList << Node{washLiquid.Get_consumeName(), washLiquid.Get_hintMessage(),
                    washLiquid.Get_descriptorMessage(), washLiquid.Get_ratio()};

    auto&& cup = CONTROLLER::Consumable::instance().cons_reactionCup;
    conList << Node{cup.Get_consumeName(), cup.Get_hintMessage(), cup.Get_descriptorMessage(),
                    cup.Get_ratio()};

    auto&& wasteTank = CONTROLLER::Consumable::instance().cons_wasteTank;
    conList << Node{wasteTank.Get_consumeName(), wasteTank.Get_hintMessage(),
                    wasteTank.Get_descriptorMessage(), wasteTank.Get_ratio()};

    auto&& wasteLiquidTank = CONTROLLER::Consumable::instance().cons_wasteLiquidTank;
    conList << Node{wasteLiquidTank.Get_consumeName(), wasteLiquidTank.Get_hintMessage(),
                    wasteLiquidTank.Get_descriptorMessage(), wasteLiquidTank.Get_ratio()};

    auto&& A = CONTROLLER::Consumable::instance().cons_ASubstrat;
    conList << Node{A.Get_consumeName(), A.Get_hintMessage(), A.Get_descriptorMessage(),
                    A.Get_ratio()};

    auto&& B = CONTROLLER::Consumable::instance().cons_BSubstrat;
    conList << Node{B.Get_consumeName(), B.Get_hintMessage(), B.Get_descriptorMessage(),
                    B.Get_ratio()};

    assert(conList.size() == m_ui_consumeItemList.size());
    for (int i = 0; i < m_ui_consumeItemList.size(); i += 1) {
        auto item = m_ui_consumeItemList[i];
        item->Set_title(conList[i].title);
        item->Set_hint(conList[i].hint);
        item->Set_descriptorMessage(conList[i].msgList);
        item->Set_ratio(conList[i].ratio);
    }
}

void ReagentConsume::refresh_bottomBtnList() {
    QList<QString>                   names;
    QList<std::function<void(void)>> taskList;
    std::function<void(void)>        task;

    names << QString("卸载");
    task = []() { DIALOG::Create_DWarningDialog("请选择有效项"); };

    switch (m_curType) {
    case Consume_Type::EType_washLiquidEnhance: {
    } break;
    case Consume_Type::EType_washLiquid: {
    } break;
    case Consume_Type::EType_cup: {
    } break;
    case Consume_Type::EType_wasteTank: {
        task = [this]() {
            auto dialog = DIALOG::Create_DIsVerify("请确认清空");
            connect(dialog, &DIALOG::DIsVerify::signal_yes, this, []() {
                auto&& wasteTank = CONTROLLER::Consumable::instance().cons_wasteTank;
                wasteTank.Reset();
                wasteTank.Update_toDB();
            });
        };
    } break;
    case Consume_Type::EType_wasteLiquidTank: {
    } break;
    case Consume_Type::EType_A: {
        task = [this]() {
            auto dialog = DIALOG::Create_DIsVerify("请确认卸载");
            connect(dialog, &DIALOG::DIsVerify::signal_yes, this, []() {
                auto&& A = CONTROLLER::Consumable::instance().cons_ASubstrat;
                A.Reset();
                A.Update_toDB();
            });
        };
    } break;
    case Consume_Type::EType_B: {
        task = [this]() {
            auto dialog = DIALOG::Create_DIsVerify("请确认卸载");
            connect(dialog, &DIALOG::DIsVerify::signal_yes, this, []() {
                auto&& B = CONTROLLER::Consumable::instance().cons_BSubstrat;
                B.Reset();
                B.Update_toDB();
            });
        };
    } break;
    case Consume_Type::EType_Count: {
    } break;
    }

    taskList << task;
    m_ui_btnGroup->Set_BtnList(names, taskList);
}

void ReagentConsume::timerEvent(QTimerEvent* event) {
    Reset();
}
}  // namespace WIDGET::REAGENT
