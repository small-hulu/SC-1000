#ifndef MCUUPGRADE_H_1728884537
#define MCUUPGRADE_H_1728884537

#include <QMessageBox>
#include <QWidget>

namespace Ui {
class MCUUpgrade;
}

class MCUUpgrade : public QWidget {
    Q_OBJECT
private:
    Ui::MCUUpgrade *ui;

public:
    explicit MCUUpgrade(QWidget *parent = nullptr);
    ~MCUUpgrade();

private:
    void refresh_port();

    void open_modbus();

private:
    void exec_upgrade();
};

#endif  // MCUUPGRADE_H_1728884537
