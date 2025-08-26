#ifndef WSETTING_GENERALSETTINGS_H
#define WSETTING_GENERALSETTINGS_H

#include <QWidget>

namespace Ui {
class WSetting_GeneralSettings;
}
namespace WIDGET::SETTING {
class WSetting_GeneralSettings : public QWidget {
    Q_OBJECT

public:
    explicit WSetting_GeneralSettings(QWidget *parent = nullptr);
    ~WSetting_GeneralSettings();

private:
    bool level_tracking_state   = 0;  // level tracking
    bool auto_mix_state         = 0;  // auto mix
    bool auto_load_state        = 0;  // auto load
    bool auto_fill_bottle_state = 0;  // auto fill bottle

private slots:
    void on_pushButton_level_tracking_clicked();

private slots:
    void on_auto_mix_clicked();

    void on_auto_load_clicked();

    void on_auto_fill_bottle_clicked();

private:
private:
    Ui::WSetting_GeneralSettings *ui;
};
}  // namespace WIDGET::SETTING
#endif  // WSETTING_GENERALSETTINGS_H
