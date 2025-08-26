#ifndef WSETTING_MCUUPGRADE_H_1723702477
#define WSETTING_MCUUPGRADE_H_1723702477

#include <QMessageBox>
#include <QWidget>

#include "controller/IniSC1000.h"
#include "lte/lte.h"

namespace Ui {
class MCUUpgrade;
}

namespace WIDGET::HELP {
class MCUUpgrade : public QWidget {
    Q_OBJECT
private:
    Ui::MCUUpgrade *ui;

public:
    explicit MCUUpgrade(QWidget *parent = nullptr);
    ~MCUUpgrade();

private:
    void exec_upgrade();

private slots:
    void on_pushButton_app_upgrade_clicked();
    void on_btn_upgrade_remote_clicked();

protected:
    void showEvent(QShowEvent *event) override;
};
}  // namespace WIDGET::HELP

#endif  // WSETTING_MCUUPGRADE_H_1723702477
