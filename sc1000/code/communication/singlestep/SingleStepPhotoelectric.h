#ifndef CONTROL_PHOTOELECTRIC_H_1723605842
#define CONTROL_PHOTOELECTRIC_H_1723605842

#include <QWidget>

namespace Ui {
class SingleStepPhotoelectric;
}

namespace COM::SINGLEWSTEP {
class SingleStepPhotoelectric : public QWidget {
    Q_OBJECT
private:
    Ui::SingleStepPhotoelectric *ui = nullptr;

public:
    explicit SingleStepPhotoelectric(QWidget *parent = nullptr);
    ~SingleStepPhotoelectric();

private:
    void operator_once();

protected:
    void showEvent(QShowEvent *event) override;
};
}  // namespace COM::SINGLEWSTEP

#endif  // CONTROL_PHOTOELECTRIC_H_1723605842
