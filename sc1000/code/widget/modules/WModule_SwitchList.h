#ifndef WMODULE_SWITCHLIST_H_1721283900
#define WMODULE_SWITCHLIST_H_1721283900

#include <QWidget>

namespace Ui {
class SwitchList;
}

namespace WIDGET {
class SwitchList : public QWidget {
    Q_OBJECT
private:
    Ui::SwitchList *ui;

private:
    QList<QString> m_msgList;
    int            m_listIndex = 0;

public:
    explicit SwitchList(QWidget *parent = nullptr);
    ~SwitchList();

public:
    void    Set_msgList(const QList<QString> &msgList);
    void    Set_curMessage(const QString &msg);
    QString Get_curMessage();

protected:
    void showEvent(QShowEvent *event) override;

signals:
    void signal_curMsg(const QString &msg);
};
}  // namespace WIDGET

#endif  // WMODULE_SWITCHLIST_H_1721283900
