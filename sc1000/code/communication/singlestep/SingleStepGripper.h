#ifndef CONTROL_GRIPPER_H_1723605776
#define CONTROL_GRIPPER_H_1723605776

#include <QWidget>

namespace Ui {
class SingleStepGripper;
}

namespace COM::SINGLEWSTEP {
class SingleStepGripper : public QWidget {
    Q_OBJECT
private:
    Ui::SingleStepGripper *ui = nullptr;

public:
    explicit SingleStepGripper(QWidget *parent = nullptr);
    ~SingleStepGripper();

private:
    void operator_once();

protected:
    void showEvent(QShowEvent *event) override;
};
}  // namespace COM::SINGLEWSTEP

#endif  // CONTROL_GRIPPER_H_1723605776
