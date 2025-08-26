#ifndef WIDGET_TEXTINPUT_H_1715828666
#define WIDGET_TEXTINPUT_H_1715828666

#include <QSharedPointer>
#include <QWidget>

namespace Ui {
class DInputText;
}

namespace WIDGET::DIALOG {

class DInputText : public QWidget {
    Q_OBJECT
private:
    QSharedPointer<Ui::DInputText> ui;

public:
    DInputText(QWidget* parent);

public:
    void           Set_messgeList(const QList<QString>& msg);
    QList<QString> Get_messgeList() const;

signals:
    void signal_yes();
};

}  // namespace WIDGET::DIALOG

#endif  // WIDGET_TEXTINPUT_H_1715828666
