#ifndef GRIPPER_5_H
#define GRIPPER_5_H

#include <QWidget>

#include "util/Interface_widget.hpp"

namespace Ui {
class Gripper_5;
}

class Gripper_5
    : public QWidget
    , public Interface_Widget {
    Q_OBJECT

    using Self = Gripper_5;

private:
    Ui::Gripper_5 *ui = nullptr;

private:
    int m_timeId = -1;

public:
    explicit Gripper_5(QWidget *parent = nullptr);
    ~Gripper_5();

private:
    void mark_message(const QString &msg) override;

private:
    /// 单次操作
    void operator_once() override;
    /// 开始执行
    void start_loop() override;

protected:
    virtual void timerEvent(QTimerEvent *event) override;
};

#endif  // GRIPPER_5_H
