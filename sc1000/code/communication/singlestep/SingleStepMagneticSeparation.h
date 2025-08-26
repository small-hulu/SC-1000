#ifndef CONTROL_MAGNETICSEPARATION_H_1723605797
#define CONTROL_MAGNETICSEPARATION_H_1723605797

#include <QWidget>

namespace Ui {
class SingleStepMagneticSeparation;
}

namespace COM::SINGLEWSTEP {
class SingleStepMagneticSeparation : public QWidget {
    Q_OBJECT
private:
    Ui::SingleStepMagneticSeparation *ui = nullptr;

public:
    explicit SingleStepMagneticSeparation(QWidget *parent = nullptr);
    ~SingleStepMagneticSeparation();

private:
    void operator_once();

protected:
    void showEvent(QShowEvent *event) override;
};
}  // namespace COM::SINGLEWSTEP

#endif  // CONTROL_MAGNETICSEPARATION_H_1723605797
