#ifndef WIDGET_LOGIN_H_1695621916
#define WIDGET_LOGIN_H_1695621916

#include <QWidget>

namespace Ui {
class Login;
}

namespace WIDGET {
class Login final : public QWidget {
    Q_OBJECT
private:
    Ui::Login *ui;

public:
    explicit Login(QWidget *parent = nullptr);
    virtual ~Login();

private:
    void check_log();

protected:
    void showEvent(QShowEvent *event) override;

signals:
    void signal_loginOk();
};
}  // namespace WIDGET

#endif  // WIDGET_LOGIN_H_1695621916
