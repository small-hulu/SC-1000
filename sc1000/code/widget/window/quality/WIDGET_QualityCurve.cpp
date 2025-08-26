#include "WIDGET_QualityCurve.h"

#include <QCalendarWidget>
#include <QDateTimeEdit>
#include <QDebug>

#include "frm/Frm.h"
#include "qcustomplot.h"
#include "ui_WIDGET_QualityCurve.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::QUALITY {

QualityCurve::QualityCurve(QWidget *parent) : QWidget(parent), ui(new Ui::QualityCurve) {
    ui->setupUi(this);

    {
        customPlot = ui->customPlot;

        QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
        textTicker->addTick(-3, "-3SD");
        textTicker->addTick(-2, "-2SD");
        textTicker->addTick(-1, "-1SD");
        textTicker->addTick(0, "X");
        textTicker->addTick(1, "1SD");
        textTicker->addTick(2, "2SD");
        textTicker->addTick(3, "3SD");
        customPlot->yAxis->setTicker(textTicker);
        customPlot->yAxis->setRange(-3, 3);
        customPlot->yAxis->setBasePen(QPen(Qt::SolidLine));
        customPlot->yAxis->grid()->setSubGridVisible(false);
        customPlot->yAxis->grid()->setPen(QPen(Qt::SolidLine));
        customPlot->yAxis->grid()->setZeroLinePen(QPen(Qt::black, 2, Qt::SolidLine));

        customPlot->xAxis->setTicks(false);
        customPlot->xAxis->setTickLabels(false);
        customPlot->xAxis->setBasePen(QPen(Qt::SolidLine));
        customPlot->xAxis->grid()->setVisible(false);

        customPlot->clearGraphs();
        //        customPlot->addGraph();
        //        customPlot->graph(0)->setPen(QPen(QColor(25, 188, 144)));
        //        customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc,
        //        14)); customPlot->graph(0)->setLineStyle(QCPGraph::lsLine);

        customPlot->replot();
    }

    {
        ui->dateEdit_start->setSpecialValueText("请选择项目");
        ui->dateEdit_end->setSpecialValueText("请选择项目");
        Frm::set_noinput(ui->dateEdit_start);
        Frm::set_noinput(ui->dateEdit_end);
    }

    {
        auto table = ui->tableWidget;
        UTILS::init_table_mode(table);
        UTILS::init_table_titles(table, {"质控项目"});

        auto entityController = &CONTROLLER::ExpQuality::instance();
        for (auto name : entityController->Filter_StringListUnique_bybyAttribute(
                 entityController->Select_all_fromDB(), entity_attr::name)) {
            auto *item = new QTableWidgetItem(name);
            item->setTextAlignment(Qt::AlignCenter);
            const int curRowCnt = table->rowCount();
            table->insertRow(curRowCnt);
            table->setRowHeight(curRowCnt, UTILS::table_auto_rowHeight);
            table->setItem(curRowCnt, 0, item);
        }
    }

    {
        connect(ui->tableWidget, &QTableWidget::itemClicked, this,
                &QualityCurve::handle_listClicked);

        connect(ui->btn_searchDate, &QPushButton::clicked, this, [this]() {
            auto item = ui->tableWidget->currentItem();
            if (item == nullptr) {
                return;
            }
            QString name = item->text();
            refresh_customplot(name);
        });
    }
}

QualityCurve::~QualityCurve() {
    delete ui;
}

void QualityCurve::handle_listClicked() {
    auto item = ui->tableWidget->currentItem();
    if (item == nullptr) {
        return;
    }

    QString name = item->text();
    ui->label_project_val->setText(name);
    refresh_time(name);
    refresh_message(name);
    refresh_customplot(name);
}

void QualityCurve::refresh_time(const QString &name) {
    auto &&expQua = CONTROLLER::ExpQuality::instance();
    auto   items  = expQua.Get_theList_expDone();
    items         = expQua.Filter_byAttribute_isEqual(items, entity_attr::name, name);
    if (items.size() == 0) {
        return;
    }

    QList<QDateTime> dateList;
    QDateTime        dateTime;
    auto timeStrList = expQua.Filter_StringList_bybyAttribute(items, entity_attr::timeStart);
    for (auto &&s : timeStrList) {
        dateList << QDateTime::fromString(s, ENTITY::EExpQuality::time_format);
    }
    dateTime = *std::min_element(dateList.begin(), dateList.end());
    ui->dateEdit_start->setDate(dateTime.date());

    dateList.clear();
    timeStrList = expQua.Filter_StringList_bybyAttribute(items, entity_attr::timeEnd);
    for (auto &&s : timeStrList) {
        dateList << QDateTime::fromString(s, ENTITY::EExpQuality::time_format);
    }
    dateTime = *std::max_element(dateList.begin(), dateList.end());
    ui->dateEdit_end->setDate(dateTime.date());
}

void QualityCurve::refresh_message(const QString &name) {
    auto &&expQua = CONTROLLER::ExpQuality::instance();
    auto   items  = expQua.Get_theList_expDone();
    items         = expQua.Filter_byAttribute_isEqual(items, entity_attr::name, name);
    if (items.size() == 0) {
        return;
    }

    auto item             = items[0];
    auto get_target_range = [item]() -> QString {
        double target = item->table[entity_attr::valTarget].toDouble();
        double sd     = item->table[entity_attr::valSD].toDouble();
        return QString::asprintf("%.4lf ~ %.4lf", target - sd, target + sd);
    };

    ui->label_name_val->setText(name);
    ui->label_SD_val->setText(item->table[entity_attr::valSD]);
    ui->label_target_val->setText(item->table[entity_attr::valTarget]);
    ui->label_targetRange_val->setText(get_target_range());
}

void QualityCurve::refresh_customplot(const QString &name) {
    auto &&expQua = CONTROLLER::ExpQuality::instance();
    auto   items  = expQua.Get_theList_expDone();
    items         = expQua.Filter_byAttribute_isEqual(items, entity_attr::name, name);
    if (items.size() == 0) {
        return;
    }

    auto get_yAxis_list = [this, item = items[0]]() -> void {
        double                            target = item->table[entity_attr::valTarget].toDouble();
        double                            sd     = item->table[entity_attr::valSD].toDouble();
        QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
        textTicker->addTick((target - sd * 3), "-3SD");
        textTicker->addTick((target - sd * 2), "-2SD");
        textTicker->addTick((target - sd * 1), "-1SD");
        textTicker->addTick((target - sd * 0), "X");
        textTicker->addTick((target - sd * -1), "1SD");
        textTicker->addTick((target - sd * -2), "2SD");
        textTicker->addTick((target - sd * -3), "3SD");
        customPlot->yAxis->setTicker(textTicker);
        customPlot->yAxis->setRange((target - sd * -3), (target - sd * 3));
    };
    get_yAxis_list();

    auto get_x_y_Axis = [items, this]() -> std::pair<QVector<double>, QVector<double> > {
        auto dateStart = ui->dateEdit_start->date();
        auto dateEnd   = ui->dateEdit_end->date();

        QVector<double> x;
        QVector<double> y;
        int             pos = 0;
        for (auto item : items) {
            if (dateStart.isValid() && dateEnd.isValid()) {
                auto start =
                    item->Generate_stringToDateTime(item->table[entity_attr::timeStart]).date();
                auto end =
                    item->Generate_stringToDateTime(item->table[entity_attr::timeEnd]).date();

                if (dateStart <= start && end <= dateEnd) {
                    x << (pos++);
                    y << item->table[entity_attr::valResult].toDouble();
                }
            } else {
                x << (pos++);
                y << item->table[entity_attr::valResult].toDouble();
            }
        }

        return {x, y};
    };

    auto [x, y] = get_x_y_Axis();
    if (x.size() == 1) {
        x << (x.back() + 1);
        y << y.back();
    }

    customPlot->clearGraphs();
    if (x.size()) {
        customPlot->xAxis->setRange(0, x.size() - 1);
        customPlot->addGraph()->setData(x, y);
    }
    customPlot->replot();
}

void QualityCurve::showEvent(QShowEvent *event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}
}  // namespace WIDGET::QUALITY
