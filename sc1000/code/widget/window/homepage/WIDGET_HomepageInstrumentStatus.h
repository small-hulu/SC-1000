#ifndef WIDGET_START_INSTRUMENTSTATUS_H_1697013998
#define WIDGET_START_INSTRUMENTSTATUS_H_1697013998

#include <QWidget>

namespace Ui {
class InstrumentStatus;
}

namespace WIDGET::HOMEPAGE {
class InstrumentStatus final : public QWidget {
    Q_OBJECT
private:
    Ui::InstrumentStatus *ui;

public:
    explicit InstrumentStatus(QWidget *parent = nullptr);
    virtual ~InstrumentStatus();
};
}  // namespace WIDGET::HOMEPAGE

#endif  // WIDGET_START_INSTRUMENTSTATUS_H_1697013998
