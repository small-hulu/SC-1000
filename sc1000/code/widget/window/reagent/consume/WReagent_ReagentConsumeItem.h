#ifndef WREAGENT_REAGENTCONSUMEITEM_H
#define WREAGENT_REAGENTCONSUMEITEM_H

#include <QLabel>
#include <QWidget>

namespace Ui {
class ReagentConsumeItem;
}

namespace WIDGET::REAGENT {
class ReagentConsumeItem : public QWidget {
    Q_OBJECT
private:
    Ui::ReagentConsumeItem *ui = nullptr;

public:
    explicit ReagentConsumeItem(QWidget *parent = nullptr);
    ~ReagentConsumeItem();

public:
    void Set_title(const QString &title);
    void Set_hint(const QString &msg);
    void Set_ratio(double ratio);
    void Set_descriptorMessage(const QList<QString> &msgList);

signals:
    void signal_clicked();
};
}  // namespace WIDGET::REAGENT

#endif  // WREAGENT_REAGENTCONSUMEITEM_H
