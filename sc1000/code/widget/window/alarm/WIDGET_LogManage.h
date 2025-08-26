#ifndef WIDGET_LOGMANAGE_H_1720069720
#define WIDGET_LOGMANAGE_H_1720069720

#include <QWidget>

namespace Ui {
class LogManage;
}
namespace WIDGET::ALARM {
class LogManage : public QWidget {
    Q_OBJECT
private:
    Ui::LogManage *ui;

public:
    explicit LogManage(QWidget *parent = nullptr);
    ~LogManage();
};
}  // namespace WIDGET::ALARM

#endif  // WIDGET_LOGMANAGE_H
