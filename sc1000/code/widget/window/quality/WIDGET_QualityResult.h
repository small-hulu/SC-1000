#ifndef WIDGET_QUALITYRESULT_H_1720069125
#define WIDGET_QUALITYRESULT_H_1720069125

#include <QPushButton>
#include <QWidget>

#include "result/WQuality_QualityResultDetail.h"
#include "result/WQuality_QualityResultMain.h"
#include "widget/modules/WModule_ButtonBottomGroup.h"

namespace Ui {
class QualityResult;
}

namespace WIDGET::QUALITY {
class QualityResult : public QWidget {
    Q_OBJECT
public:
    using entity_item = CONTROLLER::ExpQuality::entity_item;
    using entity_attr = entity_item::element_type::attr;

private:
    Ui::QualityResult *ui;

private:
    ButtonBottomGroup *m_ui_btnGroup = nullptr;

private:
    QualityResultMain   *m_ui_resultMain   = nullptr;
    QualityResultDetail *m_ui_resultDetail = nullptr;

public:
    explicit QualityResult(QWidget *parent = nullptr);
    ~QualityResult();

private:
    void refresh_bottomBtnList();

private:
    void show_resultMain();
    void show_resultDetail();
};
}  // namespace WIDGET::QUALITY

#endif  // WIDGET_QUALITYRESULT_H_1720069125
