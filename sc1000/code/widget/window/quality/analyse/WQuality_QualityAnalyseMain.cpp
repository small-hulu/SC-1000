#include "WQuality_QualityAnalyseMain.h"

#include "ui_WQuality_QualityAnalyseMain.h"
#include "widget/dialog/DIALOG.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::QUALITY {

namespace {
const QString qualifiedOk    = "合格";
const QString qualifiedNotOk = "不合格";
}  // namespace

QualityAnalyseMain::QualityAnalyseMain(QWidget* parent)
    : QWidget(parent), ui(new Ui::QualityAnalyseMain) {
    ui->setupUi(this);

    /// table
    {
        table = ui->tableWidget;
        UTILS::init_table_mode(table);

        QList<QString> titles;
        titles << QString("项目");
        titles << QString("质控品");
        titles << QString("质控品批号");
        titles << QString("试剂批号");
        titles << QString("申请时间");
        titles << QString("结束时间");
        titles << QString("结果");
        titles << QString("目标值");
        titles << QString("SD值");
        titles << QString("状态");
        titles << QString("判定");

        table->setColumnCount(titles.size());
        table->setHorizontalHeaderLabels(titles);

        /// TODO
        /// todo and test
        /// 设置缓存内容
        for (auto&& entity : CONTROLLER::ExpQuality::instance().Get_theList_expDone()) {
            this->Append_tableLine(entity);
        }
    }
}

QualityAnalyseMain::~QualityAnalyseMain() {
    delete ui;
}
void QualityAnalyseMain::Append_tableLine(QualityAnalyseMain::entity_item entity) {
    m_items.push_back(entity);
    auto line = get_lineStr(entity);

    assert(line.size() == table->columnCount());

    const int insertRow = 0;  // 插入到最上方
    table->insertRow(insertRow);
    table->setRowHeight(insertRow, UTILS::table_auto_rowHeight);

    for (int i = 0; i < line.size(); ++i) {
        auto* item = new QTableWidgetItem(line[i]);
        item->setTextAlignment(Qt::AlignCenter);
        table->setItem(insertRow, i, item);
    }

    /// 最后的判断结果单独做特殊处理
    int judgeIdx = line.size() - 1;
    auto item = table->item(insertRow, judgeIdx);
    if (item->text() == qualifiedNotOk) {
        item->setForeground(QBrush(Qt::red));
    }
}

//void QualityAnalyseMain::Append_tableLine(QualityAnalyseMain::entity_item entity) {
//    m_items.push_back(entity);
//    auto line = get_lineStr(entity);

//    assert(line.size() == table->columnCount());
//    const int curRowCnt = table->rowCount();
//    table->insertRow(curRowCnt);
//    table->setRowHeight(curRowCnt, UTILS::table_auto_rowHeight);
//    for (int i = 0; i < line.size(); i += 1) {
//        auto* item = new QTableWidgetItem(line[i]);
//        item->setTextAlignment(Qt::AlignCenter);
//        table->setItem(curRowCnt, i, item);
//    }

//    /// 最后的判断结果单独做特殊处理
//    int  judgeIdx = line.size() - 1;
//    auto item     = table->item(curRowCnt, judgeIdx);
//    if (item->text() == qualifiedNotOk) {
//        item->setForeground(QBrush(Qt::red));
//    }
//}

void QualityAnalyseMain::Entity_uninstall() {
    const int idx = table->currentRow();
    if (idx < 0) {
        DIALOG::Create_DWarningDialog("未选中有效项");
        return;
    }

    auto dialog = DIALOG::Create_DIsVerify("确认需要卸载？");
    connect(dialog, &DIALOG::DIsVerify::signal_yes, this, [this, idx]() {
        auto item = m_items[idx];
        m_items.removeOne(item);
        item->Delete_toDB();
        table->removeRow(idx);
    });
}

QList<QString> QualityAnalyseMain::get_lineStr(QualityAnalyseMain::entity_item entity) {
    QList<QString> res;
    res << entity->table[entity_attr::project];
    res << entity->table[entity_attr::name];
    res << entity->table[entity_attr::batchNum];
    auto reagent = entity->Get_relatedReagent();
    if (reagent) {
        res << reagent->table[ENTITY::EReagent::attr::batchNum];
    } else {
        res << "";
    }
    res << entity->table[entity_attr::timeStart];
    res << entity->table[entity_attr::timeEnd];
    res << entity->table[entity_attr::valResult];
    res << entity->table[entity_attr::valTarget];
    res << entity->table[entity_attr::valSD];
    res << Type::Get_TubeStateInfo((Type::ETubeState)entity->table[entity_attr::state].toInt())
               .nameChinese;
    if (entity->table[entity_attr::state].toInt() == (int)Type::ETubeState::Complete) {
        res << (entity->Is_qualified() ? qualifiedOk : qualifiedNotOk);
    } else {
        res << "";
    }
    return res;
}

void QualityAnalyseMain::showEvent(QShowEvent* event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}
}  // namespace WIDGET::QUALITY
