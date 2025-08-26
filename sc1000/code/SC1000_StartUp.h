#ifndef STARTUP_H_SC1000_1695619989
#define STARTUP_H_SC1000_1695619989

#include <QObject>

class StartUp final : public QObject {
    Q_OBJECT
private:
    enum class BaseWidget : int {
        BW_Login = 0,
        BW_Main,

        StartUp_BWCount
    };

private:
    QVector<QWidget *> m_widgetList;

public:
    StartUp();
    virtual ~StartUp();

private:
    void login_ok();
    void link_communication();
};

#endif  // STARTUP_H_SC1000_1695619989
