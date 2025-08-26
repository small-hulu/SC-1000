#ifndef WIDGET_QUALITYCURVE_H_1724145139
#define WIDGET_QUALITYCURVE_H_1724145139

#include <QListWidgetItem>
#include <QWidget>

#include "controller/CONTROLLER_ExpQuality.h"
#include "controller/CONTROLLER_Quality.h"

class QCustomPlot;
namespace Ui {
class QualityCurve;
}

namespace WIDGET::QUALITY {
class QualityCurve : public QWidget {
    Q_OBJECT
public:
    using entity_item = CONTROLLER::ExpQuality::entity_item;
    using entity_attr = entity_item::element_type::attr;

public:
    explicit QualityCurve(QWidget *parent = nullptr);
    ~QualityCurve();

private:
    Ui::QualityCurve *ui;
    QCustomPlot      *customPlot = nullptr;

private:
    void handle_listClicked();

private:
    void refresh_time(const QString &name);
    void refresh_message(const QString &name);
    void refresh_customplot(const QString &name);

protected:
    void showEvent(QShowEvent *event) override;
};
}  // namespace WIDGET::QUALITY
#endif  // WIDGET_QUALITYCURVE_H_1724145139
