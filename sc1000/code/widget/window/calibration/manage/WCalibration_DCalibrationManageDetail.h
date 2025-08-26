#ifndef WCALIBRATION_DCALIBRATIONDETAIL_H_1720580655
#define WCALIBRATION_DCALIBRATIONDETAIL_H_1720580655

#include <QWidget>

#include "controller/CONTROLLER_Calibration.h"

namespace Ui {
class DCalibrationManageDetail;
}

namespace WIDGET::CALIBRATION {
class DCalibrationManageDetail : public QWidget {
    Q_OBJECT
    using entity_item = CONTROLLER::Calibration::entity_item;
    using entity_attr = entity_item::element_type::attr;

private:
    Ui::DCalibrationManageDetail *ui;

private:
    entity_item m_entity;

public:
    explicit DCalibrationManageDetail(QWidget *parent = nullptr);
    ~DCalibrationManageDetail();

public:
    void Set_entity(entity_item entity);

private:
    void record_ui_to_entity();
    void record_entity_to_ui();

private:
    QList<double> merge_c0n_fromui();

private:
    void save_qrcodeToUi();

signals:
    void signal_yes();
};
}  // namespace WIDGET::CALIBRATION

#endif  // WCALIBRATION_DCALIBRATIONDETAIL_H_1720580655
