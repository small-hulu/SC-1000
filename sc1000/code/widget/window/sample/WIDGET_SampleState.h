#ifndef WIDGET_SAMPLESTATE_H_1720181260
#define WIDGET_SAMPLESTATE_H_1720181260

#include <QWidget>

#include "state/WIDGET_SampleStateItem.h"
#include "widget/modules/WModule_ButtonBottomGroup.h"
namespace Ui {
class SampleState;
}

namespace WIDGET::SAMPLE {
class SampleState : public QWidget {
    Q_OBJECT
private:
    Ui::SampleState *ui;

private:
    QList<SampleStateItem *> m_tubeList;
    ButtonBottomGroup       *m_ui_btnGroup = nullptr;

public:
    explicit SampleState(QWidget *parent = nullptr);
    ~SampleState();

private:
    void response_tubeClicked(int idx);
};
}  // namespace WIDGET::SAMPLE

#endif  // WIDGET_SAMPLESTATE_H_1720181260
