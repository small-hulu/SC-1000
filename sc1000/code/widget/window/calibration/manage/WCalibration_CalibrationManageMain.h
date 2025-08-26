#ifndef WCALIBRATION_CALIBRATIONMANAGEMAIN_H_1720247585
#define WCALIBRATION_CALIBRATIONMANAGEMAIN_H_1720247585

#include <QTableWidget>
#include <QWidget>

#include "controller/CONTROLLER_Calibration.h"

namespace Ui {
class CalibrationManageMain;
}

namespace WIDGET::CALIBRATION {
class CalibrationManageMain : public QWidget {
    Q_OBJECT
    using entity_item = CONTROLLER::Calibration::entity_item;
    using entity_attr = entity_item::element_type::attr;

private:
    Ui::CalibrationManageMain *ui;
    QTableWidget              *table_left  = nullptr;
    QTableWidget              *table_right = nullptr;

private:
    QList<entity_item> m_items;

public:
    explicit CalibrationManageMain(QWidget *parent = nullptr);
    ~CalibrationManageMain();

public:
    /// 底部按钮组的对应功能
    void Btn_delete();
    void Btn_edit();
    void Btn_addNew();

private:
    void Append_tableLine(entity_item entity);
    /// 显示右侧
    void Append_tableValue();

private:
    QList<QString> get_entityShowMsgList(entity_item entity);

protected:
    void showEvent(QShowEvent *event) override;
};
}  // namespace WIDGET::CALIBRATION

#endif  // WCALIBRATION_CALIBRATIONMANAGEMAIN_H_1720247585
