#ifndef WIDGET_CAROUSEL_H_1690882127
#define WIDGET_CAROUSEL_H_1690882127

#include <QSharedPointer>
#include <QTimer>
#include <QWidget>

namespace Ui {
class DProgressIndicator;
}

namespace WIDGET::DIALOG {

class DProgressIndicator : public QWidget {
    Q_OBJECT
private:
    QSharedPointer<Ui::DProgressIndicator> ui;

private:
    QTimer m_timer;

public:
    DProgressIndicator(QWidget *parent = nullptr);

public:
    void Set_words(const QString &msg);
    void Set_percentHint(const QString &msg);

private:
    void start_Carousel(const int time = 500);
    void close_Carousel();

protected:
    void showEvent(QShowEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

signals:
    void signal_showed();
};

}  // namespace WIDGET::DIALOG

#endif  // WIDGET_CAROUSEL_H_1690882127
