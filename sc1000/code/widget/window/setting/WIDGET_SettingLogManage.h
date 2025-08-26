#ifndef WIDGET_SETTINGLOGMANAGE_H_1724232985
#define WIDGET_SETTINGLOGMANAGE_H_1724232985

#include <QWidget>

namespace Ui {
class LogManage;
}

namespace WIDGET::SETTING {
class LogManage : public QWidget {
    Q_OBJECT
private:
    Ui::LogManage *ui;

public:
    explicit LogManage(QWidget *parent = nullptr);
    ~LogManage();
};
}  // namespace WIDGET::SETTING

#endif  // WIDGET_SETTINGLOGMANAGE_H_1724232985
