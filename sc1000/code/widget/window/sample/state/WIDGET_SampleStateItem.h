#ifndef WIDGET_SAMPLESTATEITEM_H_1726041460
#define WIDGET_SAMPLESTATEITEM_H_1726041460

#include <QWidget>

namespace Ui {
class SampleStateItem;
}

namespace WIDGET::SAMPLE {
class SampleStateItem : public QWidget {
    Q_OBJECT
private:
    Ui::SampleStateItem *ui = nullptr;

public:
    explicit SampleStateItem(QWidget *parent = nullptr);
    ~SampleStateItem();

public:
    QString to_string() const;

public:
    void Set_labelBottom(const QString &txt);
    void Set_labelBottom(int type);
    void Set_StateType(int type);
    void Set_ExpType(int type);

public:
    QString Get_labelBottom();

signals:
    void signal_clicked(SampleStateItem *);
};
}  // namespace WIDGET::SAMPLE
#endif  // WIDGET_SAMPLESTATEITEM_H_1726041460
