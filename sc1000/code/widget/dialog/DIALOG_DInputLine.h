#ifndef WIDGET_TEXTINPUT_H_1715828666_1725600770
#define WIDGET_TEXTINPUT_H_1715828666_1725600770

#include <QSharedPointer>
#include <QWidget>

namespace Ui {
class DInputLine;
}

namespace WIDGET::DIALOG {

class DInputLine : public QWidget {
    Q_OBJECT
private:
    QSharedPointer<Ui::DInputLine> ui;

public:
    DInputLine(QWidget* parent);

public:
    void    Set_messge(const QString& msg);
    QString Get_messge() const;

signals:
    void signal_yes();
};

}  // namespace WIDGET::DIALOG

#endif  // WIDGET_TEXTINPUT_H_1715828666_1725600770
