#ifndef WIDGET_SAMPLE_APPLY_H_1697014458
#define WIDGET_SAMPLE_APPLY_H_1697014458

#include <QPushButton>
#include <QVector>
#include <QWidget>

#include "InfoSample.h"
#include "controller/CONTROLLER_Config.h"
#include "lis/Lis_SC1000.h"
#include "widget/modules/WModule_ButtonBottomGroup.h"
#include "widget/modules/WModule_ReagentBinSample.h"
#include "widget/modules/WModule_TubeGroup3.h"

namespace Ui {
class SampleApply;
}

namespace WIDGET::SAMPLE {

class SampleApplyMain;
class SampleApplyPatient;

class SampleApply final : public QWidget {
    Q_OBJECT
public:
    constexpr static const int Sync_IntervalTime = 3000;

private:
    Ui::SampleApply *ui = nullptr;

private:
    SampleApplyMain    *m_ui_applyMain    = nullptr;
    SampleApplyPatient *m_ui_applyPatient = nullptr;
    LIS::Lis           *lis_con;

private:
    ButtonBottomGroup *m_ui_btnGroup       = nullptr;
    ReagentBinSample  *m_ui_reagentBinDial = nullptr;
    TubeGroup3        *m_ui_tubeGroup3     = nullptr;

private:
    InfoSample::ApplyInfo m_info;

public:
    explicit SampleApply(QWidget *parent = nullptr);
    virtual ~SampleApply();

private:
    void refresh_tubeGroup(int groupNum);
    void refresh_into_toUi();

private:
    void refresh_bottomBtnList();

private:
    void save_experiment();

private:
    void Bidirectional_Lis_Slot(const QByteArray &bytes);
    void show_applyMain();
    void show_applyPatient(QString tube_num);
    void lis_obtain();
    void releaseAll();
    void Multiple_Apply();

protected:
    void timerEvent(QTimerEvent *event) override;
};
}  // namespace WIDGET::SAMPLE

#endif  // WIDGET_SAMPLE_APPLY_H_1697014458
