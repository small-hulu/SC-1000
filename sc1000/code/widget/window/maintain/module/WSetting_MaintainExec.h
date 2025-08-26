#ifndef WSETTING_MAINTAINEXEC_H_1726122408
#define WSETTING_MAINTAINEXEC_H_1726122408

#include <QWidget>

namespace Ui {
class MaintainExec;
}

namespace WIDGET::MAINTAIN {

class MaintainExec : public QWidget {
    Q_OBJECT
private:
    Ui::MaintainExec *ui = nullptr;

public:
    explicit MaintainExec(QWidget *parent = nullptr);
    ~MaintainExec();

signals:
    void signal_clicked();
};
}  // namespace WIDGET::MAINTAIN

#endif  // WSETTING_MAINTAINEXEC_H_1726122408
