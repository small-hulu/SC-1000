#include "QCustomCalendar.h"

#include <QLabel>
#include <QLayout>
#include <QLocale>
#include <QPainter>
#include <QProxyStyle>
#include <QPushButton>
#include <QTableView>
#include <QTextCharFormat>

class QCustomStyle : public QProxyStyle {
public:
    QCustomStyle(QWidget *parent) {
        setParent(parent);
    };

private:
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter,
                       const QWidget *widget) const {
        if (element == PE_FrameFocusRect) {
            return;
        }
        QProxyStyle::drawPrimitive(element, option, painter, widget);
    }
};

QCustomCalendarWidget::QCustomCalendarWidget(QWidget *parent) : QCalendarWidget(parent) {
    initControl();
}

QCustomCalendarWidget::~QCustomCalendarWidget() {}

void QCustomCalendarWidget::initControl() {
    layout()->setSizeConstraint(QLayout::SetNoConstraint);
    setLocale(QLocale(QLocale::Chinese));
    setNavigationBarVisible(false);
    setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    setHorizontalHeaderFormat(QCalendarWidget::SingleLetterDayNames);
    setStyle(new QCustomStyle(this));

    QTextCharFormat format;
    format.setForeground(QColor(160, 160, 160));
    format.setBackground(QColor(255, 255, 255));
    setHeaderTextFormat(format);
    setWeekdayTextFormat(Qt::Saturday, format);
    setWeekdayTextFormat(Qt::Sunday, format);
    setWeekdayTextFormat(Qt::Monday, format);
    setWeekdayTextFormat(Qt::Tuesday, format);
    setWeekdayTextFormat(Qt::Wednesday, format);
    setWeekdayTextFormat(Qt::Thursday, format);
    setWeekdayTextFormat(Qt::Friday, format);

    initTopWidget();
    initBottomWidget();

    connect(this, &QCalendarWidget::currentPageChanged, [this](int year, int month) {
        setDataLabelTimeText(year, month);
        m_month = month;
    });
}

void QCustomCalendarWidget::paintCell(QPainter *painter, const QRect &rect,
                                      const QDate &date) const {
    if (date == selectedDate()) {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(0, 145, 255));
        painter->drawRoundedRect(rect.x(), rect.y() + 3, rect.width(), rect.height() - 6, 3, 3);
        painter->setPen(QColor(255, 255, 255));
        painter->drawText(rect, Qt::AlignCenter, QString::number(date.day()));
        painter->restore();
    }

    else if (date == QDate::currentDate()) {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(0, 161, 255));
        painter->drawRoundedRect(rect.x(), rect.y() + 3, rect.width(), rect.height() - 6, 3, 3);
        painter->setBrush(QColor(255, 255, 255));
        painter->drawRoundedRect(rect.x() + 1, rect.y() + 4, rect.width() - 2, rect.height() - 8, 2,
                                 2);
        painter->setPen(QColor(0, 161, 255));
        painter->drawText(rect, Qt::AlignCenter, QString::number(date.day()));
        painter->restore();
    }

    else if (date < minimumDate() || date > maximumDate()) {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(249, 249, 249));
        painter->drawRect(rect.x(), rect.y() + 3, rect.width(), rect.height() - 6);
        painter->setPen(QColor(220, 220, 220));
        painter->drawText(rect, Qt::AlignCenter, QString::number(date.day()));
        painter->restore();
    }

    else {
        //        this->paintCell(painter, rect, date);  //调用基类的函数
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);

        /* 绘制背景 */
        //            painter->setPen(Qt::NoPen);
        //            painter->setBrush(QColor(0, 145, 255));
        //            painter->drawRoundedRect(rect.x()+1, rect.y()+1, rect.width()-2,
        //            rect.height()-2, 3, 3);

        /* 绘制前景 */
        QFont dateFont = painter->font();
        dateFont.setPixelSize(20);
        //            dateFont.setFamily("Microsoft YaHei");
        painter->setFont(dateFont);
        painter->setPen(QColor(0, 0, 0));
        if (date.month() != m_month) {
            painter->setPen(QColor(145, 145, 145));
        }

        QRect dateRect = QRect(rect.x() + 3, rect.y() + 3, rect.width() - 6, rect.height() - 6);
        painter->drawText(dateRect, Qt::AlignCenter, QString::number(date.day()));
        painter->restore();
    }
}

void QCustomCalendarWidget::initTopWidget() {
    QWidget *topWidget = new QWidget(this);
    topWidget->setObjectName("CalendarTopWidget");
    topWidget->setFixedHeight(60);
    topWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    QHBoxLayout *hboxLayout = new QHBoxLayout;
    hboxLayout->setContentsMargins(5, 5, 5, 5);
    hboxLayout->setSpacing(10);

    m_leftYearBtn   = new QPushButton(this);
    m_leftMonthBtn  = new QPushButton(this);
    m_rightYearBtn  = new QPushButton(this);
    m_rightMonthBtn = new QPushButton(this);
    m_dataLabel     = new QLabel(this);

    m_leftYearBtn->setObjectName("CalendarLeftYearBtn");
    m_leftMonthBtn->setObjectName("CalendarLeftMonthBtn");
    m_rightYearBtn->setObjectName("CalendarRightYearBtn");
    m_rightMonthBtn->setObjectName("CalendarRightMonthBtn");
    m_dataLabel->setObjectName("CalendarDataLabel");

    m_leftYearBtn->setFixedSize(45, 45);
    m_leftMonthBtn->setFixedSize(45, 45);
    m_rightYearBtn->setFixedSize(45, 45);
    m_rightMonthBtn->setFixedSize(45, 45);

    // 设置样式
    m_leftYearBtn->setText("<<");
    m_leftMonthBtn->setText("<");
    m_rightYearBtn->setText(">>");
    m_rightMonthBtn->setText(">");

    m_leftYearBtn->setStyleSheet(R"(
            background: transparent;
            color: #333333;
            border: none;
            image: none;
            font-size: 30px;
                                  )");
    m_leftMonthBtn->setStyleSheet(R"(
                                   background: transparent;
                                   color: #333333;
                                   border: none;
                                   image: none;
                                   font-size: 30px;
                                                         )");

    m_rightYearBtn->setStyleSheet(R"(
                                   background: transparent;
                                   color: #333333;
                                   border: none;
                                   image: none;
                                   font-size: 30px;
                                                         )");

    m_rightMonthBtn->setStyleSheet(R"(
                                    background: transparent;
                                    color: #333333;
                                    border: none;
                                    image: none;
                                    font-size: 30px;
                                                          )");

    m_dataLabel->setStyleSheet(
        "QLabel{\
                                  border-style:none;\
                                  background: transparent;\
                                  color:#000000;\
                                  font-size:24px;\
                                  font-family:Microsoft YaHei;\
                                  }");

    hboxLayout->addWidget(m_leftYearBtn);
    hboxLayout->addWidget(m_leftMonthBtn);
    hboxLayout->addStretch();
    hboxLayout->addWidget(m_dataLabel);
    hboxLayout->addStretch();
    hboxLayout->addWidget(m_rightMonthBtn);
    hboxLayout->addWidget(m_rightYearBtn);
    topWidget->setLayout(hboxLayout);
    topWidget->setStyleSheet(
        "QWidget{\
                             background: transparent;\
                             color:#000000;\
                             border: 1px solid #dadde3;\
                             font-size:24px;\
                             font-family:Microsoft YaHei;\
                             background-color: rgb(255, 255, 255);\
                             }");

    QVBoxLayout *vBodyLayout = qobject_cast<QVBoxLayout *>(layout());
    vBodyLayout->insertWidget(0, topWidget);
    connect(m_leftYearBtn, SIGNAL(clicked()), this, SLOT(onbtnClicked()));
    connect(m_leftMonthBtn, SIGNAL(clicked()), this, SLOT(onbtnClicked()));
    connect(m_rightYearBtn, SIGNAL(clicked()), this, SLOT(onbtnClicked()));
    connect(m_rightMonthBtn, SIGNAL(clicked()), this, SLOT(onbtnClicked()));
    setDataLabelTimeText(selectedDate().year(), selectedDate().month());
    m_month = selectedDate().month();
}

void QCustomCalendarWidget::initBottomWidget() {
    QWidget *bottomWidget = new QWidget(this);
    bottomWidget->setObjectName("CalendarBottomWidget");
    bottomWidget->setFixedHeight(60);
    bottomWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    QHBoxLayout *hboxLayout = new QHBoxLayout;
    hboxLayout->setContentsMargins(5, 5, 5, 5);
    hboxLayout->setSpacing(10);

    btn_close = new QPushButton(this);
    btn_close->setFixedSize(140, 45);
    //    btn_close->setText(QStringLiteral("取消"));
    btn_close->setText(tr("取消"));
    btn_close->setStyleSheet(R"(
           background: transparent;
           background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(244, 96, 117, 255), stop:1 rgba(201, 67, 86, 255));
           border-radius: 6px;
           color: white;
           font-size: 22px;
           font-family: "PingFang SC Bold";
                               )");
    connect(btn_close, &QPushButton::clicked, this, [this]() { emit signal_close(); });

    m_ensureBtn = new QPushButton(this);
    m_ensureBtn->setObjectName("CalendarEnsureBtn");
    m_ensureBtn->setFixedSize(140, 45);
    //    m_ensureBtn->setText(QStringLiteral("确定"));
    m_ensureBtn->setText(tr("确定"));

    m_toDayBtn = new QPushButton(this);
    m_toDayBtn->setObjectName("CalendarTodayBtn");
    m_toDayBtn->setFixedSize(140, 45);
    //    m_toDayBtn->setText(QStringLiteral("今天"));
    m_toDayBtn->setText(tr("今天"));

    hboxLayout->addStretch();
    hboxLayout->addWidget(m_toDayBtn);
    hboxLayout->addWidget(btn_close);
    hboxLayout->addWidget(m_ensureBtn);
    bottomWidget->setLayout(hboxLayout);

    m_ensureBtn->setStyleSheet(R"(
            background: transparent;
            background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(85, 149, 255, 255), stop:1 rgba(45, 128, 245, 255));
            border-radius: 6px;
            color: white;
            font-size: 22px;
            font-family: "PingFang SC Bold";
                                )");

    m_toDayBtn->setStyleSheet(R"(
           background: transparent;
           background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(43, 185, 190, 255), stop:1 rgba(23, 146, 150, 255));
           border-radius: 6px;
           color: white;
           font-size: 22px;
           font-family: "PingFang SC Bold";
                               )");

    bottomWidget->setStyleSheet(
        "QWidget{\
                             background: transparent;\
                             color:#000000;\
                             border: 1px solid #dadde3;\
                             font-size:14px;\
                             font-family:Microsoft YaHei;\
                             background-color: rgb(255, 255, 255);\
                             }");

    QVBoxLayout *vBodyLayout = qobject_cast<QVBoxLayout *>(layout());
    vBodyLayout->addWidget(bottomWidget);

    connect(m_ensureBtn, &QPushButton::clicked, [this]() {
        emit signalSetCalendarTime(selectedDate());
        emit activated(selectedDate());
    });

    connect(m_toDayBtn, &QPushButton::clicked, [this]() {
        setSelectedDate(QDate::currentDate());
        showToday();
    });
}

void QCustomCalendarWidget::setDataLabelTimeText(int year, int month) {
    //    m_dataLabel->setText(QStringLiteral("%1年%2月").arg(year).arg(month));
    m_dataLabel->setText(tr("%1年%2月").arg(year).arg(month));
}

void QCustomCalendarWidget::onbtnClicked() {
    QPushButton *senderBtn = qobject_cast<QPushButton *>(sender());
    if (senderBtn == m_leftYearBtn) {
        showPreviousYear();
    }

    else if (senderBtn == m_leftMonthBtn) {
        showPreviousMonth();
    }

    else if (senderBtn == m_rightYearBtn) {
        showNextYear();
    }

    else if (senderBtn == m_rightMonthBtn) {
        showNextMonth();
    }
}
