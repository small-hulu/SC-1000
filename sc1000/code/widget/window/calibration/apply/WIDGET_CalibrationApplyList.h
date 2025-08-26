#ifndef WIDGET_CALIBRATIONAPPLYLIST_H_1720236465
#define WIDGET_CALIBRATIONAPPLYLIST_H_1720236465

#include <QTableWidget>
#include <QWidget>

#include "controller/CONTROLLER_Calibration.h"
#include "controller/CONTROLLER_ExpCalibration.h"
#include "widget/modules/WModule_ButtonBottomGroup.h"

namespace Ui {
class CalibrationApplyList;
}

namespace WIDGET::CALIBRATION {
class CalibrationApplyList : public QWidget {
    Q_OBJECT
    using entity_item = CONTROLLER::ExpCalibration::entity_item;
    using entity_attr = entity_item::element_type::attr;

private:
    Ui::CalibrationApplyList *ui            = nullptr;
    QTableWidget             *m_table       = nullptr;
    ButtonBottomGroup        *m_ui_btnGroup = nullptr;

private:
    QList<entity_item> m_items;

public:
    explicit CalibrationApplyList(QWidget *parent = nullptr);
    ~CalibrationApplyList();

public:
    void Btn_delete();

private:
    void Append_tableLine(entity_item entity);
    /// 刷新底部按钮组
    void refresh_bottomBtnList();

private:
    QList<QString> get_entityShowMsgList(entity_item entity);

protected:
    void showEvent(QShowEvent *event) override;
};
}  // namespace WIDGET::CALIBRATION
#endif  // WIDGET_CALIBRATIONAPPLYLIST_H_1720236465
