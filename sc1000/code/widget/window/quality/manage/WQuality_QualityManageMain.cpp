#include "WQuality_QualityManageMain.h"

#include "WQuality_DQualityManageInstallByScan.h"
#include "controller/CONTROLLER_Quality.h"
#include "ui_WQuality_QualityManageMain.h"
#include "widget/dialog/DIALOG.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::QUALITY {
QualityManageMain::QualityManageMain(QWidget* parent)
    : QWidget(parent), ui(new Ui::QualityManageMain) {
    ui->setupUi(this);

    m_view_entityController = &CONTROLLER::Quality::instance();

    /// table-left
    {
        table_left = ui->table_left;
        UTILS::init_table_mode(table_left);

        QList<QString> titles;
        titles << QString("质控名称");
        titles << QString("批号");
        titles << QString("有效期");

        table_left->setColumnCount(titles.size());
        table_left->setHorizontalHeaderLabels(titles);
    }

    /// table-right
    {
        table_right = ui->table_right;
        UTILS::init_table_mode(table_right);

        QList<QString> titles;
        titles << QString("项目");
        titles << QString("靶值");
        titles << QString("SD值");
        table_right->setColumnCount(titles.size());
        table_right->setHorizontalHeaderLabels(titles);
    }

    /// 设置缓存内容
    for (auto&& entity : m_view_entityController->Select_all_fromDB()) {
        this->Append_leftTableLine(entity);
    }

    connect(ui->table_left, &QTableWidget::cellPressed, this,
            [this]() { this->Append_rightTableValue(); });
}

QualityManageMain::~QualityManageMain() {
    delete ui;
}

QualityManageMain::entity_item QualityManageMain::Get_curLineEntity() {
    const int idx = table_left->currentRow();
    if (idx < 0) {
        return nullptr;
    }

    return m_items[idx];
}

void QualityManageMain::Refresh() {
    assert(table_left->rowCount() == m_items.size());
    for (int row = 0; row < table_left->rowCount(); row += 1) {
        auto strList = get_leftLineStr(m_items[row]);
        assert(table_left->columnCount() == strList.size());
        for (int col = 0; col < table_left->columnCount(); col += 1) {
            table_left->item(row, col)->setText(strList[col]);
        }
    }
}

void QualityManageMain::Entity_uninstall() {
    const int idx = table_left->currentRow();
    if (idx < 0) {
        DIALOG::Create_DWarningDialog("未选中有效项");
        return;
    }

    auto dialog = DIALOG::Create_DIsVerify("确认需要删除？");
    connect(dialog, &DIALOG::DIsVerify::signal_yes, this, [this, idx]() {
        auto item = m_items[idx];
        m_items.removeOne(item);
        item->Delete_toDB();
        table_left->removeRow(idx);
    });
}

void QualityManageMain::Entity_install_byScan() {
    auto dialog = new DQualityManageInstallByScan(Background::Get());
    connect(dialog, &DQualityManageInstallByScan::signal_yes, this, [dialog, this]() {
        auto entityList = dialog->Get_curEntityItem();
        for (auto entity : entityList) {
            entity->Insert_toDB();
            Append_leftTableLine(entity);
        }
    });
    dialog->show();
}

void QualityManageMain::Append_leftTableLine(QualityManageMain::entity_item entity) {
    m_items.push_back(entity);
    auto line  = get_leftLineStr(entity);
    auto table = table_left;
    assert(line.size() == table->columnCount());
    const int curRowCnt = table->rowCount();
    table->insertRow(curRowCnt);
    table->setRowHeight(curRowCnt, UTILS::table_auto_rowHeight);
    for (int i = 0; i < line.size(); i += 1) {
        auto* item = new QTableWidgetItem(line[i]);
        item->setTextAlignment(Qt::AlignCenter);
        table->setItem(curRowCnt, i, item);
    }
}

void QualityManageMain::Append_rightTableValue() {
    auto table = table_right;
    while (table->rowCount()) {
        table->removeRow(table->rowCount() - 1);
    }
    const int idx = table_left->currentRow();
    if (idx < 0) {
        return;
    }

    auto lines = get_rightLineStr(m_items[idx]);
    for (auto&& line : lines) {
        const int curRowCnt = table->rowCount();
        table->insertRow(curRowCnt);
        table->setRowHeight(curRowCnt, UTILS::table_auto_rowHeight);

        for (int j = 0; j < line.size(); j += 1) {
            auto* item = new QTableWidgetItem(line[j]);
            item->setTextAlignment(Qt::AlignCenter);
            table->setItem(curRowCnt, j, item);
        }
    }
}

QList<QString> QualityManageMain::get_leftLineStr(QualityManageMain::entity_item entity) {
    QList<QString> res;
    res << entity->table[entity_attr::name];
    res << entity->table[entity_attr::batchNumber];
    res << entity->Get_expirationDate().toString(QRCODE::time_format);
    return res;
}

QList<QList<QString>> QualityManageMain::get_rightLineStr(QualityManageMain::entity_item entity) {
    QList<QList<QString>> res;
    for (auto pro : entity->table[entity_attr::project].split(ENTITY::EQuality::join_separator,
                                                              QString::SkipEmptyParts)) {
        QList<QString> line;
        line << pro;
        line << entity->table[entity_attr::valTarget];
        line << entity->table[entity_attr::valSD];
        res << std::move(line);
    }
    return res;
}

void QualityManageMain::showEvent(QShowEvent* event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}
}  // namespace WIDGET::QUALITY
