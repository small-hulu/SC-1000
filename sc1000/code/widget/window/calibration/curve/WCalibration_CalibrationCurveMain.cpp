#include "WCalibration_CalibrationCurveMain.h"

#include <iostream>

#include "controller/CONTROLLER_Calibration.h"
#include "controller/CONTROLLER_ExpCalibration.h"
#include "controller/CONTROLLER_Reagent.h"
#include "controller/CONTROLLER_Project.h"
#include "ui_WCalibration_CalibrationCurveMain.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::CALIBRATION {

namespace {
constexpr int tmp_start = 0;
constexpr int tmp_end   = 2000;
}  // namespace

void CalibrationCurveMain::init_ui() {
    //combox_inti
    auto fileCon = CONTROLLER::Create_ProjectsFolderController();
    auto names   = fileCon.Get_ProjectList();
    auto combo = ui->comboBox;
    UTILS::init_combo_reset(combo);
    for (const auto& name : names) {
        combo->addItem(name);
    }

    connect(ui->comboBox, &QComboBox::currentTextChanged,
            this, &CalibrationCurveMain::onComboBoxTextChanged);

    auto     customPlot = plot;
    QCPAxis* xAxis      = customPlot->xAxis;
    QCPAxis* yAxis      = customPlot->yAxis;

    xAxis->setRange(tmp_start, tmp_end);
    yAxis->setRange(tmp_start, 10000000 * 1.15);

    xAxis->setLabel("浓度值");
    yAxis->setLabel("发光值");
}

CalibrationCurveMain::CalibrationCurveMain(QWidget* parent)
    : QWidget(parent), ui(new Ui::CalibrationCurveMain) {
    ui->setupUi(this);

    plot = ui->plot;
    init_ui();
}

CalibrationCurveMain::~CalibrationCurveMain() {
    delete ui;
}

CalibrationCurveMain::UiInfo CalibrationCurveMain::Get_uiInfo() {
    UiInfo info{};
    info.projectName  = ui->comboBox->currentText();
    info.reagentBatch = ui->comboBox_batch->currentText();

    using attr_reagent = CONTROLLER::Reagent::entity_attr;
    auto reaList       = CONTROLLER::Reagent::instance().Select_isLoadList();
    for (auto reagent : reaList) {
        if (reagent->table[attr_reagent::project] == info.projectName &&
                reagent->table[attr_reagent::batchNum] == info.reagentBatch) {
            info.plc4 = reagent->Get_4plc();
            //std::string  转 qstring
            qDebug()<<QString::fromStdString(info.plc4.to_string())<<endl;
        }
    }

    return info;
}

void CalibrationCurveMain::Btn_test() {
    refresh_plot();
}

void CalibrationCurveMain::Btn_actuvate() {
    qDebug()<<"激活"<<endl;
}

QVector<double> CalibrationCurveMain::get_mainCurve() {
    const auto info = Get_uiInfo();
    QVector<double> arr;

    for (int i = tmp_start; i <= tmp_end; i += 1) {
        auto ret = alg4plc_concentration_to_rlu(info.plc4, i);
        arr << ret.value;
    }

    return arr;
}

QVector<double> CalibrationCurveMain::get_calibrationCurve() {
    auto info = Get_uiInfo();

    FourPLCWithCal cal4plc;
    cal4plc.fourplc = info.plc4;
    QString projectName = info.projectName;
    using attr_expcal  = CONTROLLER::ExpCalibration::entity_attr;
    auto expCalList    = CONTROLLER::ExpCalibration::instance().Select_all_fromDB();
    CONTROLLER::ExpCalibration::entity_item latestMatch = nullptr;
    QDateTime latestTime;

    for (auto expCal : expCalList) {
        // 先判断项目名是否一致
        QString expProject = expCal->table[attr_expcal::project];
        if (expProject != projectName) {
            continue; // 跳过不是当前项目的
        }

        // 再判断时间
        QString timeEndStr = expCal->table[attr_expcal::timeEnd];
        QDateTime timeEnd = QDateTime::fromString(timeEndStr, "yyyy-MM-dd HH:mm:ss");
        if (!timeEnd.isValid()) continue;

        if (!latestMatch || timeEnd > latestTime) {
            latestMatch = expCal;
            latestTime = timeEnd;
        }
    }

    if (latestMatch) {
        qDebug() << "最新实验 idReagent =" << latestMatch->table[attr_expcal::idReagent]
                 << " project =" << latestMatch->table[attr_expcal::project]
                 << " timeEnd =" << latestTime.toString("yyyy-MM-dd HH:mm:ss");
    }

    if (latestMatch) {
        QString idReagent = latestMatch->table[attr_expcal::idReagent];
        auto reagent = CONTROLLER::Reagent::instance().Select_byId_fromDB(idReagent);
        QString idCal = latestMatch->table[attr_expcal::idCalibration];
        auto cal = CONTROLLER::Calibration::instance().Select_byId_fromDB(idCal);

        auto rluListc0nList = latestMatch->Get_c0n_avgLightValList();
        auto conList = cal->Get_c0n_ConcentrationList();

        auto& pointList = cal4plc.pointList;
        for (int i = 0; i < qMin(rluListc0nList.size(), conList.size()); i++) {
            FourPLCWithCal::Point point;
            point.rlu = rluListc0nList[i].toDouble();
            point.concentration = conList[i];
            pointList.push_back(point);
        }

        std::sort(pointList.begin(), pointList.end(),
                  [](const FourPLCWithCal::Point& a, const FourPLCWithCal::Point& b) {
            return a.rlu < b.rlu;
        });

        qDebug() << "四参数" << QString::fromStdString(cal4plc.fourplc.to_string()) << endl;

        for (auto&& item : pointList) {
            addBlueSolidPoints(plot, pointList, 12);
            qDebug() << "校准rlu=" << item.rlu << " 校准con=" << item.concentration;
        }
    }

    auto& pointList = cal4plc.pointList;
    if (pointList.size() >= 2) {
        auto newPlc = Adjust_AD_FromPointList(pointList, cal4plc.fourplc);
        cal4plc.fitting_fourplc = newPlc;
        new_plc4 = newPlc;
        //入库
    }
    qDebug() << "拟合后的四参数" << QString::fromStdString(cal4plc.fitting_fourplc.to_string())<<endl;
    QVector<double> arr;
    for (int i = tmp_start; i <= tmp_end; i += 1) {
        auto ret = alg4plc_concentration_to_rlu(cal4plc.fitting_fourplc, i);
        arr << ret.value;
    }
    return arr;
}

// 添加单个蓝色实心大圆点到QCustomPlot
QCPGraph* CalibrationCurveMain::addBlueSolidPoints(QCustomPlot* plot, const std::vector<FourPLCWithCal::Point>& points, int size)
{
    QVector<double> xPoints, yPoints;
    for (const auto& item : points) {
        xPoints.append(item.concentration);
        yPoints.append(item.rlu);
    }

    auto graph = plot->addGraph();
    graph->setLineStyle(QCPGraph::lsNone);
    graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, size));
    graph->setPen(QPen(QColor("#0000ff")));
    graph->setBrush(QBrush(QColor("#0000ff")));
    graph->addData(xPoints, yPoints);


    plot->replot();

    return graph;
}
void CalibrationCurveMain::refresh_plot() {
    plot->clearGraphs();


    QVector<double> rluList;
    for (int i = tmp_start; i <= tmp_end; i += 1) {
        rluList << i;
    }

    grap = plot->addGraph();
    {
        QPen pen(QColor("#0087d1"));
        pen.setWidth(4);  // 线宽设置在这里
        grap->setPen(pen);
        auto mainData = get_mainCurve();
        grap->addData(rluList, mainData);
        //qDebug() << "maindata:"<<mainData<<endl;
    }

    {
        grap = plot->addGraph();
        QPen pen(QColor("#ffcc40"));
        pen.setWidth(4);  // 同样设置线宽
        grap->setPen(pen);
        auto calData = get_calibrationCurve();
        grap->addData(rluList, calData);
        //qDebug() << calData;
    }

    plot->rescaleAxes();
    plot->replot();



    auto info = Get_uiInfo();
    ui->textEdit->setText("主曲线四参数:\n");
    ui->textEdit->append(info.plc4.to_string().c_str());
    ui->textEdit->append("\n校准曲线四参数:\n");
    ui->textEdit->append(new_plc4.to_string().c_str());
}

void CalibrationCurveMain::onComboBoxTextChanged(const QString &text)
{
    auto combo          = ui->comboBox_batch;
    UTILS::init_combo_reset(combo);
    using attr_reagent = CONTROLLER::Reagent::entity_attr;
    auto reagentList = CONTROLLER::Reagent::instance().Select_all_fromDB();
    QString selectedProject = text;
    QSet<QString> batchNumSet;
    for (auto& reagent : reagentList) {
        if (reagent->table[attr_reagent::project] == selectedProject) {
            QString batchNum = reagent->table[attr_reagent::batchNum];
            batchNumSet.insert(batchNum);
        }
    }
    for (const auto& batch : batchNumSet) {
        ui->comboBox_batch->addItem(batch);
    }
}

void CalibrationCurveMain::showEvent(QShowEvent* event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}
}  // namespace WIDGET::CALIBRATION
