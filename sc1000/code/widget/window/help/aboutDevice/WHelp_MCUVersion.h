#ifndef WSETTING_MCUVERSION_H_1723702490
#define WSETTING_MCUVERSION_H_1723702490

#include <QWidget>

namespace Ui {
class MCUVersion;
}

namespace WIDGET::HELP {
class MCUVersion : public QWidget {
    Q_OBJECT
private:
    static constexpr int Line_Height = 70;

private:
    Ui::MCUVersion *ui;

public:
    explicit MCUVersion(QWidget *parent = nullptr);
    ~MCUVersion();

protected:
    void showEvent(QShowEvent *event) override;
};
}  // namespace WIDGET::HELP

#endif  // WSETTING_MCUVERSION_H_1723702490
