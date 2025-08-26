#ifndef PHOTOELECTRIC_9_H
#define PHOTOELECTRIC_9_H

#include <QWidget>

#include "util/Interface_widget.hpp"

namespace Ui {
class Photoelectric_9;
}

class Photoelectric_9
    : public QWidget
    , public Interface_Widget {
    Q_OBJECT

    using Self = Photoelectric_9;

private:
    Ui::Photoelectric_9 *ui = nullptr;

public:
    explicit Photoelectric_9(QWidget *parent = nullptr);
    ~Photoelectric_9();

private:
    void mark_message(const QString &msg) override;

private:
    /// 单次操作
    void operator_once() override;
    /// 开始执行
    void start_loop() override;
};

#endif  // PHOTOELECTRIC_9_H
