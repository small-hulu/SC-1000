#ifndef WIDGET_SETTINGLISMANAGE_H_1724232985
#define WIDGET_SETTINGLISMANAGE_H_1724232985

#include <QWidget>

namespace Ui {
class LISManage;
}

namespace WIDGET::SETTING {
class LISManage : public QWidget {
    Q_OBJECT
private:
    Ui::LISManage *ui;

public:
    explicit LISManage(QWidget *parent = nullptr);
    ~LISManage();
};
}  // namespace WIDGET::SETTING

#endif  // WIDGET_SETTINGLISMANAGE_H_1724232985
