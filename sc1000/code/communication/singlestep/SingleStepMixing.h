#ifndef CONFIG_MIXING_H_1723605827
#define CONFIG_MIXING_H_1723605827

#include <QWidget>

namespace Ui {
class SingleStepMixing;
}

namespace COM::SINGLEWSTEP {
class SingleStepMixing : public QWidget {
    Q_OBJECT
private:
    Ui::SingleStepMixing *ui = nullptr;

public:
    explicit SingleStepMixing(QWidget *parent = nullptr);
    ~SingleStepMixing();

private:
    void operator_once();

protected:
    void showEvent(QShowEvent *event) override;
};
}  // namespace COM::SINGLEWSTEP

#endif  // CONFIG_MIXING_H_1723605827
