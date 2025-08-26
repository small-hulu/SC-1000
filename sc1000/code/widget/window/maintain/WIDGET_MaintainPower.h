#ifndef WIDGET_MAINTAIN_ONDEMAND_H_1717405794
#define WIDGET_MAINTAIN_ONDEMAND_H_1717405794

#include <QTableWidget>
#include <QWidget>

namespace Ui {
class MaintainPower;
}

namespace WIDGET {
namespace MAINTAIN {

class MaintainPower final : public QWidget {
    Q_OBJECT

private:
    Ui::MaintainPower *ui = nullptr;

private:
    QTableWidget *table = nullptr;

private:
    void init_ui();

public:
    explicit MaintainPower(QWidget *parent = nullptr);
    virtual ~MaintainPower();

private:
    void append_fixtureWork(const QString &txt, std::function<void(void)> task);

private:
    void task_powerOn();
    void task_powerOff();
};
}  // namespace MAINTAIN
}  // namespace WIDGET
#endif  // WIDGET_MAINTAIN_ONDEMAND_H
