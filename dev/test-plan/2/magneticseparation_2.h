#ifndef MAGNETICSEPARATION_2_H_1729844773
#define MAGNETICSEPARATION_2_H_1729844773

#include <QWidget>

#include "util/Interface_widget.hpp"

namespace Ui {
class MagneticSeparation_2;
}

class MagneticSeparation_2
    : public QWidget
    , public Interface_Widget {
    Q_OBJECT

    using Self = MagneticSeparation_2;

private:
    Ui::MagneticSeparation_2 *ui = nullptr;

public:
    explicit MagneticSeparation_2(QWidget *parent = nullptr);
    ~MagneticSeparation_2();

private:
    void mark_message(const QString &msg) override;

private:
    /// 单次操作
    void operator_once() override;
    /// 开始执行
    void start_loop() override;
};

#endif  // MAGNETICSEPARATION_2_H_1729844773
