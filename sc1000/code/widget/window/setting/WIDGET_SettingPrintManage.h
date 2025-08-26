#ifndef WIDGET_SETTINGPRINTMANAGE_H_1724232985
#define WIDGET_SETTINGPRINTMANAGE_H_1724232985

#include <QWidget>

namespace Ui {
class PrintManage;
}

namespace WIDGET::SETTING {
class PrintManage : public QWidget {
    Q_OBJECT
private:
    Ui::PrintManage *ui;

public:
    explicit PrintManage(QWidget *parent = nullptr);
    ~PrintManage();

protected:
    virtual void showEvent(QShowEvent *event) override;
};
}  // namespace WIDGET::SETTING

#endif  // WIDGET_SETTINGPRINTMANAGE_H_1724232985
