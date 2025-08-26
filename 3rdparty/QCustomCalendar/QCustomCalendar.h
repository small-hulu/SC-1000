/******************************************************************************
 * Copyright © 2023
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       QCustomCalendar.h
 * @brief      注意！这是pmd魔改的三方组件！
 *             也不是完全魔改，就是改动了什么都不说明
 *             也找不打原始出处
 *****************************************************************************/
#ifndef EXTERNAL_QCUSTOMCALENDARWIDGET_H
#define EXTERNAL_QCUSTOMCALENDARWIDGET_H

#include <QCalendarWidget>
class QPushButton;
class QLabel;

class QCustomCalendarWidget : public QCalendarWidget {
    Q_OBJECT
public:
    QCustomCalendarWidget(QWidget* parent);
    ~QCustomCalendarWidget();

private:
    void initControl();
    void initTopWidget();
    void initBottomWidget();
    void setDataLabelTimeText(int year, int month);

signals:
    void signalSetCalendarTime(const QDate& data);
    void signal_close();

private slots:
    void onbtnClicked();

protected:
    void paintCell(QPainter* painter, const QRect& rect, const QDate& date) const;

private:
    QPushButton* m_leftYearBtn;
    QPushButton* m_leftMonthBtn;
    QPushButton* m_rightYearBtn;
    QPushButton* m_rightMonthBtn;
    QPushButton* m_ensureBtn;
    QPushButton* m_toDayBtn;
    QPushButton* btn_close;

    QLabel* m_dataLabel;
    int     m_month;  // 界面切换到的月份
};

#endif  // QCUSTOMCALENDARWIDGET_H
