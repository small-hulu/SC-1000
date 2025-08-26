#include "WSample_DSampleHistoryModify.h"

#include <QListView>

#include "ui_WSample_DSampleHistoryModify.h"

namespace WIDGET::HISTORY {

/**
 * @brief ResultDReagentParams::ResultDReagentParams
 * constructor
 */
DSampleHistoryModify::DSampleHistoryModify(QWidget *parent)
    : QWidget(parent), ui(new Ui::DSampleHistoryModify{}) {
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
                m_entity->table[entity_attr::expData] = m_uiView_val[1]->text();
                m_entity->table[entity_attr::remark]  = m_uiView_val[2]->text();
            }
            emit signal_yes();
            this->close();
        });
    }
}

DSampleHistoryModify::DSampleHistoryModify(DSampleHistoryModify::entity_item entity,
                                           QWidget                          *parent)
    : DSampleHistoryModify(parent) {
    m_entity   = entity;
    auto title = get_titles();
    auto vals  = get_values();
    assert(title.size() == vals.size());
    assert(title.size() <= m_uiView_val.size());

    for (int i = 0; i < m_uiView_title.size(); i += 1) {
        if (i < title.size()) {
            m_uiView_title[i]->setText(title[i]);
        } else {
            m_uiView_title[i]->hide();
        }
    }
    for (int i = 0; i < m_uiView_val.size(); i += 1) {
        if (i < vals.size()) {
            m_uiView_val[i]->setText(vals[i]);
        } else {
            m_uiView_val[i]->hide();
        }
    }
}

QList<QString> DSampleHistoryModify::get_titles() {
    m_uiView_val[0]->setReadOnly(true);

    QList<QString> res;
    res << QString("编号");
    res << QString("发光值");
    res << QString("备注");
    return res;
}

QList<QString> DSampleHistoryModify::get_values() {
    QList<QString> res;
    res << m_entity->Get_idString();
    res << m_entity->table[entity_attr::expData];
    res << m_entity->table[entity_attr::remark];
    return res;
}

}  // namespace WIDGET::HISTORY
