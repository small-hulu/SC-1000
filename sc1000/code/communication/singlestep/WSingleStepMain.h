#ifndef MAINWINDOW_H_1723606018
#define MAINWINDOW_H_1723606018

#include <QMainWindow>
#include <QModbusClient>

QT_BEGIN_NAMESPACE
namespace Ui {
class SingleStepMain;
}
QT_END_NAMESPACE

namespace COM::SINGLEWSTEP {
class SingleStepMain : public QMainWindow {
    Q_OBJECT
private:
    Ui::SingleStepMain *ui = nullptr;

private:
    int m_timeId = -1;

public:
    SingleStepMain(QWidget *parent = nullptr);
    virtual ~SingleStepMain();

private:
    void refresh_port();

private:
    void open_modbus();

private:
    void read_modbus();
    void write_modbus();

protected:
    virtual void timerEvent(QTimerEvent *event) override;
};
}  // namespace COM::SINGLEWSTEP

#endif  // MAINWINDOW_H_1723606018
