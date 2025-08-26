#ifndef WIDGET_PRINTVIEWSAMPLE_H
#define WIDGET_PRINTVIEWSAMPLE_H

#include <QJsonObject>
#include <QPrintPreviewWidget>
#include <QWidget>
namespace Ui {
class WIDGET_PrintViewSample;
}

class WIDGET_PrintViewSample : public QWidget {
    Q_OBJECT

public:
    explicit WIDGET_PrintViewSample(QWidget *parent = nullptr);

    explicit WIDGET_PrintViewSample(QWidget             *parent = nullptr,
                                    QPrintPreviewWidget *widget = nullptr);
    ~WIDGET_PrintViewSample();

    void setTemp_json(const QJsonObject &value);

    QJsonObject getTemp_json() const;

private:
    Ui::WIDGET_PrintViewSample *ui;
    QJsonObject                 temp_json;
    QWidget                    *m_widget;
    QPrintPreviewWidget        *pre_widget;

public slots:
    void on_pushButton_print_clicked();
    void on_pushButton_back_clicked();
    void on_pushButton_narrow_btn_clicked();
    void on_pushButton_enlarge_btn_clicked();
signals:
    void send_print_data(QJsonObject temp_json);
};

#endif  // WIDGET_PRINTVIEWSAMPLE_H
