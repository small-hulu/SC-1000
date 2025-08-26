#ifndef MIXING_2_H
#define MIXING_2_H

#include <QWidget>

#include "util/Interface_widget.hpp"

namespace Ui {
class Mixing_2;
}

class Mixing_2
    : public QWidget
    , public Interface_Widget {
    Q_OBJECT

    using Self = Mixing_2;

private:
    Ui::Mixing_2 *ui = nullptr;

public:
    explicit Mixing_2(QWidget *parent = nullptr);
    ~Mixing_2();

private:
    void mark_message(const QString &msg) override;

private:
    void operator_once() override;
    void start_loop() override;
};

#endif  // MIXING_2_H
