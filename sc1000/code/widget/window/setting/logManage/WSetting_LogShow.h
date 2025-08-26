#ifndef WIDGET_LogShow_H_1724305768
#define WIDGET_LogShow_H_1724305768

#include <QTableWidget>
#include <QWidget>

#include "widget/modules/WModule_TablePageTurnSwitch.h"

namespace Ui {
class LogShow;
}

namespace WIDGET::SETTING {

class LogShow : public QWidget {
    Q_OBJECT
private:
    static constexpr int Row_Count = 6;

private:
    Ui::LogShow* ui = nullptr;

private:
    TablePageTurnSwitch* pageSwitch = nullptr;
    QTableWidget*        table      = nullptr;

public:
    LogShow(QWidget* parent = nullptr);
    ~LogShow();

private:
    void show_range(int start, int end);
    void switch_type();

private:
    void clear_log();

protected:
    void showEvent(QShowEvent* event) override;
};

}  // namespace WIDGET::SETTING

#endif  // WIDGET_LogShow_H_1724305768
