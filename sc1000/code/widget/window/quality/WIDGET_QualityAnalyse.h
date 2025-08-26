#ifndef WIDGET_QUALITYANALYSE_H
#define WIDGET_QUALITYANALYSE_H

#include <QPushButton>
#include <QWidget>

#include "analyse/WQuality_QualityAnalyseMain.h"
#include "widget/modules/WModule_ButtonBottomGroup.h"

namespace Ui {
class QualityAnalyse;
}

namespace WIDGET::QUALITY {
class QualityAnalyse : public QWidget {
    Q_OBJECT
private:
    Ui::QualityAnalyse *ui;

private:
    ButtonBottomGroup *m_ui_btnGroup = nullptr;

private:
    QualityAnalyseMain *m_analyseMain = nullptr;

public:
    explicit QualityAnalyse(QWidget *parent = nullptr);
    ~QualityAnalyse();

private:
    /// 刷新底部按钮组
    void refresh_bottomBtnList();

private:
    void show_analyseMain();
};
}  // namespace WIDGET::QUALITY

#endif  // WIDGET_QUALITYANALYSE_H
