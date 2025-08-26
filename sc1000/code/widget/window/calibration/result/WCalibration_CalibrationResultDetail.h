#ifndef WCALIBRATION_CALIBRATIONRESULTDETAIL_H_1720250254
#define WCALIBRATION_CALIBRATIONRESULTDETAIL_H_1720250254

#include <QTableWidget>
#include <QWidget>

#include "controller/CONTROLLER_ExpCalibration.h"

namespace Ui {
class CalibrationResultDetail;
}

namespace WIDGET::CALIBRATION {
class CalibrationResultDetail : public QWidget {
    Q_OBJECT
private:
    using entity_item = CONTROLLER::ExpCalibration::entity_item;
    using entity_attr = entity_item::element_type::attr;

private:
    Ui::CalibrationResultDetail *ui;
    QTableWidget                *table = nullptr;

private:
    entity_item m_entity;

public:
    explicit CalibrationResultDetail(QWidget *parent = nullptr);
    ~CalibrationResultDetail();

public:
    void Set_entity(entity_item entity);

private:
    void refresh();
    void refresh_message(entity_item entity);

public:
    void Append_tableLine(entity_item entity);

private:
    void showEvent(QShowEvent *event) override;
};
}  // namespace WIDGET::CALIBRATION

#endif  // WCALIBRATION_CALIBRATIONRESULTDETAIL_H_1720250254
