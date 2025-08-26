#include "WReagent_DReagentDetails.h"

#include <QListView>

#include "ui_WReagent_DReagentDetails.h"

namespace WIDGET::REAGENT {
DReagentDetails::DReagentDetails(QWidget *parent) : QWidget(parent), ui(new Ui::DReagentDetails{}) {
    ui->setupUi(this);

    /// attribute & ui
    {
        if (parent) {
            this->setGeometry(parent->rect());
        }
        this->setAttribute(Qt::WA_StyledBackground);
        this->setAttribute(Qt::WA_DeleteOnClose);

        m_uiView_title.clear();
        m_uiView_title << ui->label_paramsName_0;
        m_uiView_title << ui->label_paramsName_1;
        m_uiView_title << ui->label_paramsName_2;
        m_uiView_title << ui->label_paramsName_3;
        m_uiView_title << ui->label_paramsName_4;
        m_uiView_title << ui->label_paramsName_5;
        m_uiView_title << ui->label_paramsName_6;
        m_uiView_title << ui->label_paramsName_7;
        m_uiView_title << ui->label_paramsName_8;
        m_uiView_title << ui->label_paramsName_9;
        m_uiView_title << ui->label_paramsName_10;
        m_uiView_title << ui->label_A;
        m_uiView_title << ui->label_B;
        m_uiView_title << ui->label_C;
        m_uiView_title << ui->label_D;

        m_uiView_val.clear();
        m_uiView_val << ui->edit_paramsValue_0;
        m_uiView_val << ui->edit_paramsValue_1;
        m_uiView_val << ui->edit_paramsValue_2;
        m_uiView_val << ui->edit_paramsValue_3;
        m_uiView_val << ui->edit_paramsValue_4;
        m_uiView_val << ui->edit_paramsValue_5;
        m_uiView_val << ui->edit_paramsValue_6;
        m_uiView_val << ui->edit_paramsValue_7;
        m_uiView_val << ui->edit_paramsValue_8;
        m_uiView_val << ui->edit_paramsValue_9;
        m_uiView_val << ui->edit_paramsValue_10;
        m_uiView_val << ui->edit_A;
        m_uiView_val << ui->edit_B;
        m_uiView_val << ui->edit_C;
        m_uiView_val << ui->edit_D;

        assert(m_uiView_val.size() == m_uiView_title.size());
    }

    /// connect 确认
    { connect(ui->btn_yes, &QPushButton::clicked, this, &QWidget::close); }
}

DReagentDetails::DReagentDetails(DReagentDetails::entity_item entity, QWidget *parent)
    : DReagentDetails(parent) {
    m_entity           = entity;
    auto attrListName  = get_attrList();
    auto attrListValue = get_attrList();
    auto titles        = m_entity->Get_attrListNames(attrListName);
    m_vals             = m_entity->Get_attrListValues(attrListValue);
    assert(m_vals.size() <= m_uiView_val.size());

    for (int i = 0; i < titles.size(); i += 1) {
        m_uiView_title[i]->setText(titles[i]);
    }
    for (int i = titles.size(); i < m_uiView_title.size(); i += 1) {
        m_uiView_title[i]->hide();
    }
    for (int i = 0; i < m_vals.size(); i += 1) {
        m_uiView_val[i]->setText(m_vals[i]);
    }
    for (int i = m_vals.size(); i < m_uiView_val.size(); i += 1) {
        m_uiView_val[i]->hide();
    }
}

/**
 * 后期根据需要改成定制化的
 */
QList<DReagentDetails::entity_attr> DReagentDetails::get_attrList() {
    QList<entity_attr> attrList;
    attrList << entity_attr::project;
    attrList << entity_attr::mainCurve;
    attrList << entity_attr::calibrationStatus;
    attrList << entity_attr::batchNum;
    attrList << entity_attr::bottleNum;
    attrList << entity_attr::periodOfValidity;
    attrList << entity_attr::type;
    attrList << entity_attr::ableMeasureBottleCnt;
    attrList << entity_attr::ableProjectMeasureCnt;
    attrList << entity_attr::location;
    attrList << entity_attr::bottleOpenTime;
    attrList << entity_attr::A;
    attrList << entity_attr::B;
    attrList << entity_attr::C;
    attrList << entity_attr::D;
    return attrList;
}
}  // namespace WIDGET::REAGENT
