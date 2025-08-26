#ifndef WIDGETTEST_H_1723606001
#define WIDGETTEST_H_1723606001

#include <QWidget>
#include <thread>

namespace Ui {
class MultStpeTrigger;
}

namespace COM::SINGLEWSTEP {
class MultStpeTrigger : public QWidget {
    Q_OBJECT
private:
    Ui::MultStpeTrigger *ui = nullptr;

public:
    std::thread taskTh;
    bool        keepThreadRunning = true;

public:
    explicit MultStpeTrigger(QWidget *parent = nullptr);
    ~MultStpeTrigger();

private:
    void fun_test();
    void fun_mult();

private:
    void join_thread();

protected:
    void showEvent(QShowEvent *event) override;
};
}  // namespace COM::SINGLEWSTEP

#endif  // WIDGETTEST_H_1723606001
