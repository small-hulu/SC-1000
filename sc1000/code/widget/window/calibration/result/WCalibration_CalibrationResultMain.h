#ifndef WCALIBRATION_CALIBRATIONRESULTMAIN_H_1720249595
#define WCALIBRATION_CALIBRATIONRESULTMAIN_H_1720249595

#include <QTableWidget>
#include <QWidget>

#include "controller/CONTROLLER_ExpCalibration.h"

namespace Ui {
class CalibrationResultMain;
}

namespace WIDGET::CALIBRATION {
class CalibrationResultMain : public QWidget {
    Q_OBJECT
    using entity_item = CONTROLLER::ExpCalibration::entity_item;
    using entity_attr = entity_item::element_type::attr;

private:
    Ui::CalibrationResultMain *ui = nullptr;

    QTableWidget *table_left  = nullptr;
    QTableWidget *table_right = nullptr;

private:
    QList<entity_item> m_items;

public:
    explicit CalibrationResultMain(QWidget *parent = nullptr);
    ~CalibrationResultMain();

public:
    entity_item Get_curEntity();
    void        Btn_save();
    void        Btn_delete();

private:
    void Append_leftTableLine(entity_item entity);
    /// 右侧显示
    void Append_rightTableValue();

private:
    QList<QString> get_left_msgList(entity_item entity);

private:
    void showEvent(QShowEvent *event) override;
};
}  // namespace WIDGET::CALIBRATION

#endif  // WCALIBRATION_CALIBRATIONRESULTMAIN_H
