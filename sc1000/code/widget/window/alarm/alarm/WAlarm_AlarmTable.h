#ifndef WALARM_ALARMTABLE_H_1720186450
#define WALARM_ALARMTABLE_H_1720186450

#include <QWidget>

namespace Ui {
class AlarmTable;
}

namespace WIDGET::ALARM {
class AlarmTable : public QWidget {
    Q_OBJECT
private:
    Ui::AlarmTable *ui;

public:
    explicit AlarmTable(QWidget *parent = nullptr);
    ~AlarmTable();

protected:
    void showEvent(QShowEvent *event) override;
};
}  // namespace WIDGET::ALARM

#endif  // WALARM_ALARMTABLE_H_1720186450
