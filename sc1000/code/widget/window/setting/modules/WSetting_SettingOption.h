#ifndef WSETTING_SETTINGOPTION_H_1724297135
#define WSETTING_SETTINGOPTION_H_1724297135

#include <QWidget>

namespace Ui {
class SettingOption;
}

namespace WIDGET::SETTING {

class SettingOption : public QWidget {
    Q_OBJECT
private:
    Ui::SettingOption *ui;

public:
    explicit SettingOption(QWidget *parent = nullptr);
    SettingOption(QString btnText, QWidget *parent = nullptr);
    ~SettingOption();

signals:
    void signal_selected();
};

}  // namespace WIDGET::SETTING

#endif  // WSETTING_SETTINGOPTION_H_1724297135
