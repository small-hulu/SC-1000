#ifndef WIDGET_ALARMALARM_H_1720069584
#define WIDGET_ALARMALARM_H_1720069584

#include <QWidget>

namespace Ui {
class AlarmAlarm;
}

namespace WIDGET::ALARM {
class AlarmAlarm : public QWidget {
    Q_OBJECT
private:
    Ui::AlarmAlarm *ui;

public:
    explicit AlarmAlarm(QWidget *parent = nullptr);
    ~AlarmAlarm();
};
}  // namespace WIDGET::ALARM

#endif  // WIDGET_ALARMALARM_H_1720069584
