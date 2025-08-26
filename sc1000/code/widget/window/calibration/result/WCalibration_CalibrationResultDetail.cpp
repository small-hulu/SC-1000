#include "WCalibration_CalibrationResultDetail.h"

#include "ui_WCalibration_CalibrationResultDetail.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::CALIBRATION {
CalibrationResultDetail::CalibrationResultDetail(QWidget *parent)
    : QWidget(parent), ui(new Ui::CalibrationResultDetail) {
    ui->setupUi(this);
    table = ui->tableWidget;
    UTILS::resize_layout_stretch(ui->verticalLayout, {1, 1, 8});

    {
        UTILS::init_table_mode(table);
        QList<QString> titles;
        titles << QString("项目");
        titles << QString("浓度");
        titles << QString("发光值");
        titles << QString("结束时间");
        titles << QString("试剂批号");
        titles << QString("试剂瓶号");
        titles << QString("底物批号");
        titles << QString("底物瓶号");
        titles << QString("校准品批号");
        titles << QString("校准时间");

        table->setColumnCount(titles.size());
        table->setHorizontalHeaderLabels(titles);
    }
}

CalibrationResultDetail::~CalibrationResultDetail() {
    delete ui;
}

void CalibrationResultDetail::Set_entity(CalibrationResultDetail::entity_item entity) {
    m_entity = entity;
    refresh();
}

void CalibrationResultDetail::refresh() {
    if (m_entity == nullptr) {
        return;
    }

    refresh_message(m_entity);
    Append_tableLine(m_entity);
}

void CalibrationResultDetail::refresh_message(CalibrationResultDetail::entity_item entity) {
    auto idcal = entity->table[entity_attr::idCalibration];
    auto cal   = CONTROLLER::Calibration::instance().Select_byId_fromDB(idcal);
    if (cal) {
        ui->lab_name_edit->setText(cal->table[CONTROLLER::Calibration::entity_attr::name]);
    } else {
        ui->lab_name_edit->setText(QString("解析错误"));
    }
}

void CalibrationResultDetail::Append_tableLine(CalibrationResultDetail::entity_item entity) {
    if (entity == nullptr) {
        return;
    }
    auto idCal     = entity->table[entity_attr::idCalibration];
    auto cal       = CONTROLLER::Calibration::instance().Select_byId_fromDB(idCal);
    auto idReagent = entity->table[entity_attr::idReagent];
    auto reagent   = CONTROLLER::Reagent::instance().Select_byId_fromDB(idReagent);

    /// 浓度列表
    /// 测试的发光值
    auto          c0nTar = entity->Get_c0n_avgLightValList();
    QList<double> c0nVal;
    if (cal) {
        c0nVal = cal->Get_c0n_ConcentrationList();
    }

    /// todo
    /// 这里做一个长度的扩充，防止下面由于测试阶段的胡乱判断
    while (c0nVal.size() < qMax(c0nTar.size(), c0nVal.size())) {
        c0nVal << 0;
    }
    while (c0nTar.size() < qMax(c0nTar.size(), c0nVal.size())) {
        c0nTar << 0;
    }
    assert(c0nTar.size() == c0nVal.size());

    QList<QList<QString>> mat;
    for (int i = 0; i < c0nTar.size(); i += 1) {
        using attr_cal = CONTROLLER::Calibration::entity_attr;
        using attr_rea = CONTROLLER::Reagent::entity_attr;

        QList<QString> line;
        line << entity->table[entity_attr::project];
        line << c0nTar[i];
        line << QString::number(c0nVal[i]);
        line << entity->table[entity_attr::timeEnd];
        if (reagent) {
            line << reagent->table[attr_rea::batchNum];
            line << reagent->Get_bottleNumber();
        } else {
            line << QString("查询错误");
            line << QString("查询错误");
        }
        line << QString("底物未录入");
        line << QString("底物未录入");
        if (cal) {
            line << cal->table[attr_cal::batchNum];
            line << cal->table[attr_cal::timeEnd];
        } else {
            line << QString("查询错误");
            line << QString("查询错误");
        }

        mat << std::move(line);
    }

    while (table->rowCount()) {
        table->removeRow(table->rowCount() - 1);
    }
    for (int i = 0; i < mat.size(); i += 1) {
        table->insertRow(i);
        table->setRowHeight(i, UTILS::table_auto_rowHeight);
        assert(table->columnCount() == mat[i].size());
        for (int j = 0; j < mat[i].size(); j += 1) {
            auto *item = new QTableWidgetItem(mat[i][j]);
            item->setTextAlignment(Qt::AlignCenter);
            QFont font = item->font();
            font.setPointSize(14);
            item->setFont(font);
            table->setItem(i, j, item);
        }
    }
}

void CalibrationResultDetail::showEvent(QShowEvent *event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}
}  // namespace WIDGET::CALIBRATION
