#ifndef WIDGET_QUALITYMANAGE_H
#define WIDGET_QUALITYMANAGE_H

#include <QPushButton>
#include <QWidget>

#include "manage/WQuality_QualityManageDetail.h"
#include "manage/WQuality_QualityManageMain.h"
#include "manage/WQuality_QualityManageRule.h"
#include "widget/modules/WModule_ButtonBottomGroup.h"

namespace Ui {
class QualityManage;
}
namespace WIDGET::QUALITY {
class QualityManage : public QWidget {
    Q_OBJECT
private:
    Ui::QualityManage *ui = nullptr;

private:
    ButtonBottomGroup *m_ui_btnGroup = nullptr;

private:
    QualityManageMain   *m_manageMain   = nullptr;
    QualityManageDetail *m_manageDetail = nullptr;
    QualityManageDetail *m_manageEdit   = nullptr;
    QualityManageRule   *m_manageRule   = nullptr;

public:
    explicit QualityManage(QWidget *parent = nullptr);
    ~QualityManage();

private:
    /// 刷新底部按钮组
    void refresh_bottomBtnList();

private:
    void show_manageMain();
    void show_manageDetail();
    void show_manageEdit();
    void show_manageRule();
};
}  // namespace WIDGET::QUALITY

#endif  // WIDGET_QUALITYMANAGE_H
