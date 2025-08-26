#ifndef WIDGET_MAIN_H_1695621593
#define WIDGET_MAIN_H_1695621593

#include <QWidget>

#include "top/WIDGET_Main_MTop.h"

namespace Ui {
class Main;
}

namespace WIDGET {
class Main final : public QWidget {
    Q_OBJECT
private:
    Ui::Main *ui = nullptr;

private:
    MAIN::MTop *topWidget = nullptr;

public:
    explicit Main(QWidget *parent = nullptr);
    virtual ~Main();

private:
    void jump_page(int idx1, int idx2);
    void switch_page(int index);

protected:
    void showEvent(QShowEvent *event) override;
};
}  // namespace WIDGET

#endif  // WIDGET_MAIN_H_1695621593
