#include "WQuality_DQualityQuery.h"

#include "ui_WQuality_DQualityQuery.h"

namespace WIDGET::QUALITY {
DQualityQuery::DQualityQuery(QWidget *parent) : QWidget(parent), ui(new Ui::DQualityQuery) {
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
        m_uiView_title << ui->label_paramsName_11;

        m_uiView_val.clear();
        m_uiView_val << ui->edit_params_0;
        m_uiView_val << ui->edit_params_1;
        m_uiView_val << ui->edit_params_2;
        m_uiView_val << ui->edit_params_3;
        m_uiView_val << ui->edit_params_4;
        m_uiView_val << ui->edit_params_5;
        m_uiView_val << ui->edit_params_6;
        m_uiView_val << ui->edit_params_7;
        m_uiView_val << ui->edit_params_8;
        m_uiView_val << ui->edit_params_9;
        m_uiView_val << ui->edit_params_10;
        m_uiView_val << ui->edit_params_11;

        assert(m_uiView_val.size() == m_uiView_title.size());
    }

    /// connect 取消 确认
    {
        connect(ui->btn_no, &QPushButton::clicked, this, [this]() { this->close(); });

        connect(ui->btn_yes, &QPushButton::clicked, this, [this]() {
            if (m_entity) {
                auto attrList = get_attrList();
                m_entity->Set_attrListValues(attrList, this->Get_viewMessage());
            }
            emit signal_yes();
            this->close();
        });
    }
}

DQualityQuery::DQualityQuery(DQualityQuery::entity_item entity, QWidget *parent)
    : DQualityQuery(parent) {
    m_entity      = entity;
    auto attrList = get_attrList();
    auto titles   = m_entity->Get_attrListNames(attrList);
    m_vals        = m_entity->Get_attrListValues(attrList);
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

QList<QString> DQualityQuery::Get_allMessage() {
    for (int i = 0; i < m_vals.size(); i += 1) {
        m_vals[i] = m_uiView_val[i]->text();
    }
    return m_vals;
}

QList<QString> DQualityQuery::Get_viewMessage() {
    QList<QString> ans;
    for (int i = 0; i < m_vals.size() && m_uiView_val[i]->isVisible(); i += 1) {
        ans << m_uiView_val[i]->text();
    }
    return ans;
}

QList<DQualityQuery::entity_attr> DQualityQuery::get_attrList() {
    QList<entity_attr> attrList;
    attrList << entity_attr::name;
    attrList << entity_attr::batchNum;
    attrList << entity_attr::state;
    attrList << entity_attr::location;
    attrList << entity_attr::project;
    return attrList;
}
}  // namespace WIDGET::QUALITY
