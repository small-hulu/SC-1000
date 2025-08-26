#include "WCalibration_CalibrationResultMain.h"

#include "3rdparty/frm/frminput.h"
#include "controller/CONTROLLER_Calibration.h"
#include "controller/CONTROLLER_Reagent.h"
#include "ui_WCalibration_CalibrationResultMain.h"
#include "widget/dialog/DIALOG.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::CALIBRATION {

void set_editWidget(QTableWidgetItem *item) {
    if (!(item->flags() & Qt::ItemIsEnabled)) {
        return;
    }

    QTimer::singleShot(30, []() { frmInput::Instance()->hide(); });

    auto obj = DIALOG::Create_DInputLine();
    obj->Set_messge(item->text());
    QObject::connect(obj, &WIDGET::DIALOG::DInputLine::signal_yes,
                     [item, obj]() {
        item->setText(obj->Get_messge());});
    obj->show();
}

CalibrationResultMain::CalibrationResultMain(QWidget *parent)
    : QWidget(parent), ui(new Ui::CalibrationResultMain) {
    ui->setupUi(this);
    table_left  = ui->table_left;
    table_right = ui->table_right;

    /// init-table
    {
        QList<QString> titles;
        titles << QString("项目");
        titles << QString("校准品批号");
        titles << QString("校准状态");
        titles << QString("申请时间");
        titles << QString("结束时间");

        auto table = table_left;
        UTILS::init_table_mode(table);
        UTILS::init_table_titles(table, titles);
    }

    {
        QList<QString> titles;
        titles << QString("水平");
        titles << QString("目标浓度");
        titles << QString("发光值");

        auto table = table_right;
        UTILS::init_table_mode(table);
        UTILS::init_table_titles(table, titles);
        table->setEditTriggers(QAbstractItemView::DoubleClicked);
        connect(table, &QTableWidget::cellDoubleClicked, this, [table](int row, int col) {
            auto item = table->item(row, col);
            set_editWidget(item);
        });
    }

    /// connect
    {
        connect(ui->table_left, &QTableWidget::cellPressed, this,
                [this]() { this->Append_rightTableValue(); });
    }

    /// expCal
    {
        auto calExpList = CONTROLLER::ExpCalibration::instance().Get_theList_expDone();
        for (auto &&calExp : calExpList) {
            Append_leftTableLine(calExp);
        }
    }
}

CalibrationResultMain::~CalibrationResultMain() {
    delete ui;
}

CalibrationResultMain::entity_item CalibrationResultMain::Get_curEntity() {
    const int idx = table_left->currentRow();
    if (idx < 0) {
        return {};
    }
    return m_items[idx];
}
void CalibrationResultMain::Append_leftTableLine(CalibrationResultMain::entity_item entity) {
    m_items << entity;
    auto table = table_left;

    const int insertRow = 0;  // 插入到最上方
    table->insertRow(insertRow);
    table->setRowHeight(insertRow, UTILS::table_auto_rowHeight);

    auto line = get_left_msgList(entity);
    assert(line.size() == table->columnCount());
    for (int i = 0; i < line.size(); ++i) {
        auto* item = new QTableWidgetItem(line[i]);
        item->setTextAlignment(Qt::AlignCenter);
        table->setItem(insertRow, i, item);
    }
}

void CalibrationResultMain::Append_rightTableValue() {
    const int idx = table_left->currentRow();
    if (idx < 0) {
        return;
    }
    const auto& item = m_items[m_items.size() - 1 - idx];
    //auto          item          = m_items[idx];
    auto          cLightValList = item->Get_c0n_originalLightValList();
    auto          cal           = item->Get_relatedCalibration();
    QList<double> cConValList;
    if (cal) {
        cConValList = cal->Get_c0n_ConcentrationList();
        assert(cConValList.size() == cLightValList.size());
    } else {
        while (cConValList.size() < cLightValList.size()) {
            cConValList.push_back({});
        }
    }

    QList<QList<QString>> mat;
    for (int i = 0; i < cLightValList.size(); i += 1) {
        for (int j = 0; j < cLightValList[i].size(); j += 1) {
            QList<QString> line;
            line << QString("C%1").arg(i);
            line << QString::number(cConValList[i]);
            line << cLightValList[i][j];
            mat << std::move(line);
        }
    }

    auto table = table_right;
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
            if (j + 1 < mat[i].size()) {
                UTILS::set_tableWidgetItem_enable(item, false);
            } else {
                UTILS::set_tableWidgetItem_enable(item, true);
            }
            table->setItem(i, j, item);
        }
    }
}

QList<QString> CalibrationResultMain::get_left_msgList(CalibrationResultMain::entity_item entity) {
    using attr     = CONTROLLER::ExpCalibration::entity_attr;
    auto idCal     = entity->table[attr::idCalibration];
    auto cal       = CONTROLLER::Calibration::instance().Select_byId_fromDB(idCal);
    auto idReagent = entity->table[attr::idReagent];
    auto reagent   = CONTROLLER::Reagent::instance().Select_byId_fromDB(idReagent);

    QList<QString> res;
    res << entity->table[attr::project];
    if (cal) {
        /// 校准品批号
        /// from 校准品
        res << cal->table[CONTROLLER::Calibration::entity_attr::batchNum];
    } else {
        res << QString("数据错误");
    }
    if (reagent) {
        /// 校准状态
        /// from 试剂
        res << reagent->table[CONTROLLER::Reagent::entity_attr::calibrationStatus];
    } else {
        res << QString("数据错误");
    }
    res << entity->table[attr::timeStart];
    res << entity->table[attr::timeEnd];

    return res;
}

void CalibrationResultMain::Btn_save() {
    const int index = table_left->currentRow();
    const int realIndex = m_items.size() - 1 - index;  // 转换索引

    if (index < 0 || index >= m_items.size()) {
        return;
    }

    QList<QString> vals;
    auto           table = table_right;
    for (int i = 0; i < table->rowCount(); i += 1) {
        vals << table->item(i, table->columnCount() - 1)->text();
    }

    auto item = m_items[realIndex];
    item->Set_expData(vals);
    item->Update_toDB();
    DIALOG::Create_DInfoDialog("保存完成");
}

void CalibrationResultMain::Btn_delete() {
    const int index = table_left->currentRow();
    if (index < 0 || index >= m_items.size()) {
        return;
    }

    auto dialog = DIALOG::Create_DIsVerify("请确认删除");
    connect(dialog, &DIALOG::DIsVerify::signal_yes, this, [this]() {
        const int index  = table_left->currentRow();
        qDebug()<<"index"<<index<<endl;
        int reverseIndex = m_items.size() - 1 - index;
        auto entity = m_items[reverseIndex];
        entity->Delete_toDB();
        m_items.removeOne(entity);
        table_left->removeRow(index);
    });
}

void CalibrationResultMain::showEvent(QShowEvent *event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}
}  // namespace WIDGET::CALIBRATION
