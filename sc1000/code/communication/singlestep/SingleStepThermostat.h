#ifndef CONTROL_THERMOSTAT_H_1723605955
#define CONTROL_THERMOSTAT_H_1723605955

#include <QWidget>

namespace Ui {
class SingleStepThermostat;
}

namespace COM::SINGLEWSTEP {
class SingleStepThermostat : public QWidget {
    Q_OBJECT
private:
    Ui::SingleStepThermostat *ui;

public:
    explicit SingleStepThermostat(QWidget *parent = nullptr);
    ~SingleStepThermostat();

private:
    void operator_once();

protected:
    void showEvent(QShowEvent *event) override;
};
}  // namespace COM::SINGLEWSTEP

#endif  // CONTROL_THERMOSTAT_H_1723605955
