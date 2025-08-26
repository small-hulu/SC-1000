#include "WSample_DSampleApplyParams.h"

#include <QListView>

#include "ui_WSample_DSampleApplyParams.h"

namespace WIDGET::SAMPLE {

/**
 * @brief ResultDReagentParams::ResultDReagentParams
 * constructor
 */
DSampleApplyParams::DSampleApplyParams(QWidget *parent)
    : QWidget(parent), ui(new Ui::DSampleApplyParams{}) {
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
                /// 这里仅单纯的向对象进行简单的赋值
                /// 不要做多余的操作
                auto attrList = get_attrList();
                for (int i = 0; i < attrList.size(); i += 1) {
                    m_entity->table[entity_attr(i)] = m_uiView_val[i]->text();
                }
            }
            emit signal_yes();
            this->close();
        });
    }
}

DSampleApplyParams::DSampleApplyParams(DSampleApplyParams::entity_item entity, QWidget *parent)
    : DSampleApplyParams(parent) {
    m_entity      = entity;
    auto attrList = get_attrList();
    auto title    = entity->Get_attrListNames(attrList);
    auto vals     = entity->Get_attrListValues(attrList);
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

QList<DSampleApplyParams::entity_attr> DSampleApplyParams::get_attrList() {
    QList<entity_attr> attrList;
    attrList << entity_attr::number;
    attrList << entity_attr::project;
    attrList << entity_attr::location;
    attrList << entity_attr::qrcode;
    attrList << entity_attr::detectType;
    attrList << entity_attr::sampleTube;
    attrList << entity_attr::dilutionRatio;
    return attrList;
}
}  // namespace WIDGET::SAMPLE
