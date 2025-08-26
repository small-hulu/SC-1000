#ifndef WIDGET_HELPUSINGHELP_H_1724225162
#define WIDGET_HELPUSINGHELP_H_1724225162

#include <QWidget>

namespace Ui {
class Specification;
}

namespace WIDGET::HELP {
class Specification : public QWidget {
    Q_OBJECT
private:
    Ui::Specification *ui;

public:
    explicit Specification(QWidget *parent = nullptr);
    ~Specification();
};
}  // namespace WIDGET::HELP

#endif  // WIDGET_HELPUSINGHELP_H_1724225162
