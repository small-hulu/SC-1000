#ifndef WIDGET_HELPUPGRADE_H_1726219758
#define WIDGET_HELPUPGRADE_H_1726219758

#include <QWidget>

namespace Ui {
class HelpUpgrade;
}

namespace WIDGET::HELP {
class HelpUpgrade : public QWidget {
    Q_OBJECT
private:
    Ui::HelpUpgrade *ui;

public:
    explicit HelpUpgrade(QWidget *parent = nullptr);
    ~HelpUpgrade();
};
}  // namespace WIDGET::HELP

#endif  // WIDGET_HELPUPGRADE_H_1726219758
