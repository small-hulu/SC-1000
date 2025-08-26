#include "WIDGET_CalibrationApplyMain.h"

#include "controller/CONTROLLER_Calibration.h"
#include "controller/CONTROLLER_ExpCalibration.h"
#include "controller/CONTROLLER_Project.h"
#include "controller/CONTROLLER_Reagent.h"
#include "ui_WIDGET_CalibrationApplyMain.h"
#include "widget/dialog/DIALOG.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::CALIBRATION {
CalibrationApplyMain::CalibrationApplyMain(QWidget *parent)
    : QWidget(parent), ui(new Ui::CalibrationApplyMain) {
    ui->setupUi(this);
    m_ui_reagentBatchTable = ui->tableWidget;

    /// ui-init
    {
        m_ui_calBatch = new SwitchList(ui->widget_switch_cal);

        m_ui_projectTable = new TableSelect(ui->widget_left);
        m_ui_projectTable->Builder(8, 2);
        auto folder = CONTROLLER::Create_ProjectsFolderController();
        m_ui_projectTable->Set_tableText(folder.Get_ProjectList());
        auto isLoadList     = CONTROLLER::Reagent::instance().Select_isLoadList();
        auto curProjectList = CONTROLLER::Reagent::instance().Filter_StringList_bybyAttribute(
            isLoadList, ENTITY::EReagent::attr::project);
        m_ui_projectTable->Set_tableEnableList(curProjectList);

        connect(m_ui_projectTable, &TableSelect::signal_cellSelected, this, [this]() {
            refresh_reagentBatchNumber();

            QString        project = m_ui_projectTable->Get_lastSeletedInfo().txt;
            QList<QString> msg;
            auto         &&cal = CONTROLLER::Calibration::instance();
            for (auto node : cal.Select_all_fromDB()) {
                if (project == node->table[ENTITY::ECalibration::attr::project]) {
                    msg << node->table[ENTITY::ECalibration::attr::batchNum];
                }
            }
            m_ui_calBatch->Set_msgList(msg);
        });
    }

    {
        QList<QString> titles;
        titles << QString("试剂批号");
        auto table = m_ui_reagentBatchTable;

        UTILS::init_table_mode(table);
        UTILS::init_table_titles(table, titles);
        UTILS::resize_table_auto(table);

        table->horizontalHeader()->setVisible(true);
    }
}

CalibrationApplyMain::~CalibrationApplyMain() {
    delete ui;
}

/**
 * ui上的当前数据
 */
CalibrationApplyMain::UiInfo CalibrationApplyMain::Get_uiInfo() {
    UiInfo info;

    /// 项目名
    info.projectName = m_ui_projectTable->Get_lastSeletedInfo().txt;

    /// 试剂批号
    auto tableItem = m_ui_reagentBatchTable->currentItem();
    if (tableItem) {
        info.reagentBatch = tableItem->text();
    }

    /// 校准批号
    info.calBatch = m_ui_calBatch->Get_curMessage();

    /// c0~c5的数据
    QList<QLineEdit *> editList;
    editList << ui->edit_c0;
    editList << ui->edit_c1;
    editList << ui->edit_c2;
    editList << ui->edit_c3;
    editList << ui->edit_c4;
    editList << ui->edit_c5;
    for (auto edit : editList) {
        QString s = edit->text();
        if (s.isEmpty() == false) {
            info.c0nList << s;
        } else {
            break;
        }
    }

    return info;
}

void CalibrationApplyMain::Btn_save(CalibrationApplyMain::UiInfo info) {
    auto entity = CONTROLLER::ExpCalibration::Create_oneEntityItem();

    using attr_cal = CONTROLLER::ExpCalibration::entity_attr;

    /// 保存项目
    entity->table[attr_cal::project] = info.projectName;

    /// 保存试剂号
    /// 根据试剂批号
    auto reagentEntity =
        CONTROLLER::Reagent::instance().Filter_by_batchNum(info.reagentBatch, info.projectName);
    if (reagentEntity) {
        entity->table[attr_cal::idReagent] = reagentEntity->Get_idString();
    }

    /// 保存校准品号
    /// 根据校准品批号
    auto calEntity = CONTROLLER::Calibration::instance().Filter_by_batchNum(info.calBatch);
    if (calEntity) {
        entity->table[attr_cal::idCalibration] = calEntity->Get_idString();
    }

    /// c0-c5的位置
    entity->Set_c0n_LocationList(info.c0nList);

    /// * 项目
    /// * 试剂号
    /// * 校准号
    for (auto key : {attr_cal::project, attr_cal::idReagent, attr_cal::idCalibration}) {
        if (entity->table[key].isNull()) {
            qDebug() << entity->to_string();
            qDebug() << "必填数据为空，错误";
            return;
        }
    }

    entity->Builder();
    entity->table[attr_cal::state] = QString::number((int)Type::ETubeState::Apply);
    entity->Insert_toDB();
    DIALOG::Create_DInfoDialog("添加成功");
}

/**
 * 判断每个输入框是否有焦点
 * 有的话直接填入
 */
void CalibrationApplyMain::Set_tubeIndex(QString msg) {
    /// c0~c5的数据
    QList<QLineEdit *> editList;
    editList << ui->edit_c0;
    editList << ui->edit_c1;
    editList << ui->edit_c2;
    editList << ui->edit_c3;
    editList << ui->edit_c4;
    editList << ui->edit_c5;

    for (auto edit : editList) {
        if (edit->hasFocus()) {
            edit->setText(msg);
            edit->clearFocus();
            break;
        }
    }
}

/**
 * 点击左侧的项目，刷新右侧的试剂批号
 */
void CalibrationApplyMain::refresh_reagentBatchNumber() {
    auto table = m_ui_reagentBatchTable;
    while (table->rowCount()) {
        table->removeRow(table->rowCount() - 1);
    }

    auto curPro      = m_ui_projectTable->Get_lastSeletedInfo();
    auto reagentList = CONTROLLER::Reagent::instance().FilterList_by_project(curPro.txt);
    if (reagentList.isEmpty()) {
        m_ui_projectTable->Clear_tableCellClicked(curPro.x, curPro.y);
        DIALOG::Create_DWarningDialog("校准品信息未录入");
        return;
    }

    UTILS::init_table_rowCount(table, reagentList.size());
    auto ver = table->verticalHeader();
    ver->setSectionResizeMode(QHeaderView::Custom);
    for (int i = 0; i < table->rowCount(); i += 1) {
        QString msg = reagentList[i]->table[CONTROLLER::Reagent::entity_attr::batchNum];
        table->item(i, 0)->setTextAlignment(Qt::AlignCenter);
        table->item(i, 0)->setText(msg);
        table->setRowHeight(i, 100);
    }
}

void CalibrationApplyMain::showEvent(QShowEvent *event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}
}  // namespace WIDGET::CALIBRATION
