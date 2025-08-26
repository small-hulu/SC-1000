#ifndef WIDGET_SETTINGHOSPITALMANAGE_H_1724232985
#define WIDGET_SETTINGHOSPITALMANAGE_H_1724232985

#include <QWidget>

namespace Ui {
class HospitalManage;
}

namespace WIDGET::SETTING {
class HospitalManage : public QWidget {
    Q_OBJECT
private:
    Ui::HospitalManage *ui;

public:
    explicit HospitalManage(QWidget *parent = nullptr);
    ~HospitalManage();
};
}  // namespace WIDGET::SETTING

#endif  // WIDGET_SETTINGHOSPITALMANAGE_H_1724232985
