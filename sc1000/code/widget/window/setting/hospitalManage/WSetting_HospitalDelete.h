#ifndef WSETTING_HOSPITALDELETE_H_1726119053
#define WSETTING_HOSPITALDELETE_H_1726119053

#include <QList>
#include <QWidget>

namespace Ui {
class HospitalDelete;
}

namespace WIDGET::SETTING {
class HospitalDelete : public QWidget {
    Q_OBJECT
private:
    Ui::HospitalDelete *ui;

private:
    QList<QString> m_message;

public:
    explicit HospitalDelete(QWidget *parent = nullptr);
    ~HospitalDelete();

public:
    void Set_message(const QList<QString> &msg) {
        m_message = msg;
    }
    const QList<QString> &Get_message() {
        return m_message;
    }

signals:
    void signal_clicked();
};
}  // namespace WIDGET::SETTING

#endif  // WSETTING_HOSPITALDELETE_H_1726119053
