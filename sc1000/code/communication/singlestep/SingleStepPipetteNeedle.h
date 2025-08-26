#ifndef CONTROL_PIPETTINGNEEDLE_H_1723605890
#define CONTROL_PIPETTINGNEEDLE_H_1723605890

#include <QWidget>

namespace Ui {
class SingleStepPipettingNeedle;
}

namespace COM::SINGLEWSTEP {
class SingleStepPipetteNeedle : public QWidget {
    Q_OBJECT
private:
    Ui::SingleStepPipettingNeedle *ui = nullptr;

public:
    explicit SingleStepPipetteNeedle(QWidget *parent = nullptr);
    ~SingleStepPipetteNeedle();

private:
    void operator_once();

protected:
    void showEvent(QShowEvent *event) override;
};
}  // namespace COM::SINGLEWSTEP

#endif  // CONTROL_PIPETTINGNEEDLE_H_1723605890
