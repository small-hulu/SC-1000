#include "WQuality_QualityManageDetail.h"

#include <QDateTime>
#include <QTimer>

#include "controller/CONTROLLER_Project.h"
#include "ui_WQuality_QualityManageDetail.h"
#include "widget/modules/WModule_TableSelect.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::QUALITY {
QualityManageDetail::QualityManageDetail(QWidget *parent)
    : QWidget(parent), ui(new Ui::QualityManageDetail) {
    ui->setupUi(this);

    m_ui_tableSelect = new TableSelect(ui->widget_table);
    m_ui_tableSelect->Builder(5, 6);
    auto folder = CONTROLLER::Create_ProjectsFolderController();
    m_ui_tableSelect->Set_tableText(folder.Get_ProjectList());
}

QualityManageDetail::~QualityManageDetail() {
    delete ui;
}

QualityManageDetail::UiInfo QualityManageDetail::Get_UiInfo() {
    UiInfo info;
    info.projectList      = m_ui_tableSelect->Get_tableCheckedString();
    info.name             = ui->edit_name->text();
    info.batchNumber      = ui->edit_batchNumber->text();
    info.periodOfValidity = ui->edit_periodOfValidity->text();
    info.valTargetStart   = ui->edit_valTargetStart->text();
    info.valTargetEnd     = ui->edit_valTargetEnd->text();
    info.valTarget        = ui->edit_valTarget->text();
    info.valSD            = ui->edit_valSD->text();
    info.unit             = ui->edit_unit->text();
    return info;
}

QualityManageDetail::entity_item QualityManageDetail::Btn_save(QualityManageDetail::UiInfo info) {
    entity_item entity;
    //! 已经存在标志
    bool flag = m_entity != nullptr;
    if (flag) {
        entity = m_entity;
    } else {
        entity = CONTROLLER::Quality::Create_oneEntityItem();
    }
    entity->table[entity_attr::project]          = info.projectList.join(UTILS::join_separator);
    entity->table[entity_attr::name]             = info.name;
    entity->table[entity_attr::batchNumber]      = info.batchNumber;
    entity->table[entity_attr::periodOfValidity] = info.periodOfValidity;
    entity->table[entity_attr::valTargetStart]   = info.valTargetStart;
    entity->table[entity_attr::valTargetEnd]     = info.valTargetEnd;
    entity->table[entity_attr::valTarget]        = info.valTarget;
    entity->table[entity_attr::valSD]            = info.valSD;
    entity->table[entity_attr::unit]             = info.unit;
    ui->edit_project->setText(info.projectList.join(UTILS::join_separator));

    //! 存在   update
    //! 不存在 insert
    if (flag) {
        entity->Update_toDB();
    } else {
        entity->Insert_toDB();
    }
    return entity;
}

void QualityManageDetail::refresh_ui() {
    if (m_entity == nullptr) {
        return;
    }

    auto entity = m_entity;
    m_ui_tableSelect->Set_tableClickedList(entity->Get_projectList());
    ui->edit_name->setText(entity->table[entity_attr::name]);
    ui->edit_batchNumber->setText(entity->table[entity_attr::batchNumber]);
    ui->edit_periodOfValidity->setText(entity->table[entity_attr::periodOfValidity]);
    ui->edit_valTarget->setText(entity->table[entity_attr::valTarget]);
    ui->edit_valTargetStart->setText(entity->table[entity_attr::valTargetStart]);
    ui->edit_valTargetEnd->setText(entity->table[entity_attr::valTargetEnd]);
    ui->edit_valTarget->setText(entity->table[entity_attr::valTarget]);
    ui->edit_valSD->setText(entity->table[entity_attr::valSD]);
    ui->edit_unit->setText(entity->table[entity_attr::unit]);
    ui->edit_project->setText(entity->table[entity_attr::project]);
    ui->edit_valTargetStart->setText(
        QString::number(entity->table[entity_attr::valTarget].toDouble() -
                        entity->table[entity_attr::valSD].toDouble()));
    ui->edit_valTargetEnd->setText(
        QString::number(entity->table[entity_attr::valTarget].toDouble() +
                        entity->table[entity_attr::valSD].toDouble()));
}

void QualityManageDetail::showEvent(QShowEvent *event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}
}  // namespace WIDGET::QUALITY
