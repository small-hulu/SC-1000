#ifndef WQUALITY_QUALITYMANAGEMAINRULE_H_1722474916
#define WQUALITY_QUALITYMANAGEMAINRULE_H_1722474916

#include <QWidget>

namespace Ui {
class QualityManageRule;
}

namespace WIDGET::QUALITY {
class QualityManageRule : public QWidget {
    Q_OBJECT
private:
    Ui::QualityManageRule *ui = nullptr;

public:
    explicit QualityManageRule(QWidget *parent = nullptr);
    ~QualityManageRule();

protected:
    void showEvent(QShowEvent *event) override;
};
}  // namespace WIDGET::QUALITY

#endif  // WQUALITY_QUALITYMANAGEMAINRULE_H_1722474916
