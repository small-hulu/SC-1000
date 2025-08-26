#ifndef WIDGET_QUALITYAPPLY_H_1720069000
#define WIDGET_QUALITYAPPLY_H_1720069000

#include <QPushButton>
#include <QWidget>

#include "widget/modules/WModule_ButtonBottomGroup.h"
#include "widget/modules/WModule_ReagentBinSample.h"
#include "widget/modules/WModule_TubeGroup3.h"
#include "widget/modules/WModule_TubeUiItem.h"

namespace Ui {
class QualityApply;
}

namespace WIDGET::QUALITY {

class QualityApplyMain;

class QualityApply : public QWidget {
    Q_OBJECT
private:
    Ui::QualityApply *ui;

private:
    ButtonBottomGroup *m_ui_btnGroup       = nullptr;
    ReagentBinSample  *m_ui_reagentBinDial = nullptr;
    TubeGroup3        *m_ui_tubeGroup3     = nullptr;

private:
    QualityApplyMain *m_ui_applyMain = nullptr;

public:
    explicit QualityApply(QWidget *parent = nullptr);
    ~QualityApply();

private:
    void refresh_tubeGroup(int groupNum);

private:
    /// 刷新底部按钮组
    void refresh_bottomBtnList();

private:
    void show_applyMain();
};
}  // namespace WIDGET::QUALITY

#endif  // WIDGET_QUALITYAPPLY_H_1720069000
