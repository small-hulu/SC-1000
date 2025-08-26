#ifndef WQUALITY_DPatientDetail_H_1724221323
#define WQUALITY_DPatientDetail_H_1724221323

#include <QWidget>

#include "widget/Background.h"

namespace Ui {
class DHSampleDetail;
}

namespace WIDGET::HISTORY {
class DHSampleDetail : public QWidget {
    Q_OBJECT

private:
    Ui::DHSampleDetail *ui;

public:
    explicit DHSampleDetail(QWidget *parent = nullptr);
    ~DHSampleDetail();

protected:
    void showEvent(QShowEvent *event) override;

signals:
    void signal_yes();
};
}  // namespace WIDGET::HISTORY

#endif  // WQUALITY_DPatientDetail_H_1724221323
