#ifndef WIDGET_REAGENT_CONSUME_H_1715592340
#define WIDGET_REAGENT_CONSUME_H_1715592340

#include <QWidget>

#include "consume/WReagent_ReagentConsumeItem.h"
#include "widget/modules/WModule_ButtonBottomGroup.h"

namespace Ui {
class ReagentConsume;
}

namespace WIDGET::REAGENT {
class ReagentConsume : public QWidget {
    Q_OBJECT

private:
    enum class Consume_Type {
        EType_washLiquidEnhance = 0,
        EType_washLiquid,
        EType_cup,
        EType_wasteTank,
        EType_wasteLiquidTank,
        EType_A,
        EType_B,

        EType_Count
    };

private:
    Ui::ReagentConsume *ui = nullptr;

private:
    QList<ReagentConsumeItem *> m_ui_consumeItemList;
    Consume_Type                m_curType = Consume_Type::EType_washLiquidEnhance;

private:
    ButtonBottomGroup *m_ui_btnGroup = nullptr;

public:
    explicit ReagentConsume(QWidget *parent = nullptr);
    ~ReagentConsume();

public:
    void Reset();

private:
    /// 刷新底部按钮组
    void refresh_bottomBtnList();

protected:
    void timerEvent(QTimerEvent *event) override;
};
}  // namespace WIDGET::REAGENT
#endif  // WIDGET_REAGENT_CONSUME_H_1715592340
