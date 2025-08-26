#ifndef FORM_H_1723605924
#define FORM_H_1723605924

#include <QWidget>

namespace Ui {
class SingleStepReagentBin;
}

namespace COM::SINGLEWSTEP {
class SingleStepReagentBin : public QWidget {
    Q_OBJECT
private:
    Ui::SingleStepReagentBin *ui = nullptr;

public:
    explicit SingleStepReagentBin(QWidget *parent = nullptr);
    ~SingleStepReagentBin();

private:
    void operator_once();

protected:
    void showEvent(QShowEvent *event) override;
};
}  // namespace COM::SINGLEWSTEP

#endif  // FORM_H_1723605924
