#ifndef WCALIBRATION_CALIBRATIONCURVEMAIN_H_1720614266
#define WCALIBRATION_CALIBRATIONCURVEMAIN_H_1720614266

#include <QWidget>
#include "qcustomplot.h"
#include "algorithm/algorithm.hpp"

namespace Ui {
class CalibrationCurveMain;
}
class QCustomPlot;

namespace WIDGET::CALIBRATION {
class CalibrationCurveMain : public QWidget {
    Q_OBJECT
    struct UiInfo {
        QString projectName;
        QString reagentBatch;
        FourPLC plc4;
    };

private:
    Ui::CalibrationCurveMain *ui;
    QCustomPlot              *plot = nullptr;
    FourPLC new_plc4;
    QMap<int, QString> cali_combox_pro;
private:
    void init_ui();

public:
    explicit CalibrationCurveMain(QWidget *parent = nullptr);
    ~CalibrationCurveMain();

public:
    UiInfo Get_uiInfo();

public:
    void Btn_test();
    void Btn_actuvate();
    QCPGraph* addBlueSolidPoints(QCustomPlot* plot, const std::vector<FourPLCWithCal::Point>& points, int size);
private:
    QVector<double> get_mainCurve();
    QVector<double> get_calibrationCurve();
    QCPGraph* grap = nullptr;
    void refresh_plot();
public slots:
    void onComboBoxTextChanged(const QString &text);

protected:
    void showEvent(QShowEvent *event) override;
};
}  // namespace WIDGET::CALIBRATION

#endif  // WCALIBRATION_CALIBRATIONCURVEMAIN_H_1720614266
