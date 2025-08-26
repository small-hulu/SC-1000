#include "WReagent_DReagentParams.h"

#include <QListView>

#include "qrcode/QRCODE.hpp"
#include "ui_WReagent_DReagentParams.h"
#include "widget/dialog/DIALOG.h"

namespace WIDGET::REAGENT {
DReagentParams::DReagentParams(QWidget *parent) : QWidget(parent), ui(new Ui::DReagentParams{}) {
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

    /// 让这里有一个非空的实例
    {
        if (m_entity == nullptr) {
            m_entity = CONTROLLER::Reagent::Create_oneEntityItem();
        }
    }

    /// connect 取消 确认
    {
        connect(ui->btn_no, &QPushButton::clicked, this, [this]() { this->close(); });

        connect(ui->btn_yes, &QPushButton::clicked, this, [this]() {
            save_uiToEntity(m_entity);
            emit signal_yes();
            this->close();
        });

        auto &&qrcode = QRCODE::SerialQrcode::instance();
        connect(&qrcode, &QRCODE::SerialQrcode::signal_qrcode_originalData, this,
                [this](QByteArray bytes, int type) {
                    if (type != (int)QRCODE::QrcodeType::reagent) {
                        DIALOG::Create_DWarningDialog("条码匹配错误");
                        return;
                    }

                    m_entity->table[entity_attr::qrcode] = QString(bytes);
                    save_qrcodeToUi(bytes);
                    // todo
                });
    }
}

DReagentParams::DReagentParams(DReagentParams::entity_item entity, QWidget *parent)
    : DReagentParams(parent) {
    m_entity    = entity;
    auto titles = get_titlesList();
    auto vals   = get_valueList(m_entity);
    assert(vals.size() <= m_uiView_val.size());

    for (int i = 0; i < m_uiView_title.size(); i += 1) {
        if (i < titles.size()) {
            m_uiView_title[i]->setText(titles[i]);
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

QList<QString> DReagentParams::Get_viewMessage() {
    QList<QString> ans;
    for (int i = 0; i < m_uiView_val.size() && m_uiView_val[i]->isVisible(); i += 1) {
        ans << m_uiView_val[i]->text();
    }
    return ans;
}

QList<DReagentParams::entity_attr> DReagentParams::get_attrList() {
    QList<entity_attr> attrList;
    attrList << entity_attr::location;
    attrList << entity_attr::project;
    attrList << entity_attr::type;
    attrList << entity_attr::ableMeasureBottleCnt;
    attrList << entity_attr::ableProjectMeasureCnt;
    attrList << entity_attr::periodOfValidity;
    attrList << entity_attr::batchNum;
    attrList << entity_attr::bottleNum;
    attrList << entity_attr::state;
    attrList << entity_attr::bottleOpenTime;
    return attrList;
}

QList<QString> DReagentParams::get_titlesList() {
    QList<QString> lines;
    lines << QString("位置");
    lines << QString("项目");
    lines << QString("类型");
    lines << QString("可测瓶数");
    lines << QString("测试数");
    lines << QString("保质期");
    lines << QString("批号");
    lines << QString("瓶号");
    lines << QString("状态");
    lines << QString("开品时间");
    return lines;
}

QList<QString> DReagentParams::get_valueList(entity_item entity) {
    QList<QString> lines;
    lines << entity->table[entity_attr::location];
    lines << entity->table[entity_attr::project];
    lines << entity->table[entity_attr::type];
    lines << entity->table[entity_attr::ableMeasureBottleCnt];
    lines << entity->table[entity_attr::ableProjectMeasureCnt];
    lines << entity->table[entity_attr::periodOfValidity];
    lines << entity->table[entity_attr::batchNum];
    lines << entity->table[entity_attr::bottleNum];
    lines << entity->table[entity_attr::state];
    lines << entity->table[entity_attr::bottleOpenTime];
    return lines;
}

void DReagentParams::save_uiToEntity(DReagentParams::entity_item entity) {
    QList<entity_attr> attrList;
    attrList << entity_attr::location;
    attrList << entity_attr::project;
    attrList << entity_attr::type;
    attrList << entity_attr::ableMeasureBottleCnt;
    attrList << entity_attr::ableProjectMeasureCnt;
    attrList << entity_attr::periodOfValidity;
    attrList << entity_attr::batchNum;
    attrList << entity_attr::bottleNum;
    attrList << entity_attr::state;
    attrList << entity_attr::bottleOpenTime;

    assert(attrList.size() <= m_uiView_val.size());
    for (int i = 0; i < attrList.size() && m_uiView_val[i]->isVisible(); i += 1) {
        entity->table[attrList[i]] = m_uiView_val[i]->text();
    }

    QRCODE::EReagent qrcode(entity->table[entity_attr::qrcode].toUtf8());
    namespace attr_qr             = QRCODE::KeyRea;
    entity->table[entity_attr::A] = qrcode[attr_qr::project_curvea].toString();
    entity->table[entity_attr::B] = qrcode[attr_qr::project_curveb].toString();
    entity->table[entity_attr::C] = qrcode[attr_qr::project_curvec].toString();
    entity->table[entity_attr::D] = qrcode[attr_qr::project_curved].toString();
    QVariant fillingVariant       = qrcode[attr_qr::base_filling];
    if (fillingVariant.canConvert<QStringList>()) {
        QStringList filling                 = fillingVariant.toStringList();
        QString     fillingText             = filling.join(",");
        entity->table[entity_attr::filling] = fillingText;
    }
    entity->table[entity_attr::liquidConsume] = "0,0,0,0";
}

void DReagentParams::save_qrcodeToUi(const QByteArray &bytes) {
    QRCODE::EReagent qrcode(bytes);
    namespace attr_qr = QRCODE::KeyRea;
    m_uiView_val[1]->setText(qrcode[attr_qr::base_proname].toString());
    m_uiView_val[2]->setText(qrcode[attr_qr::base_reagentstype].toString());
    m_uiView_val[5]->setText(qrcode[attr_qr::base_validity].toString());
    m_uiView_val[6]->setText(qrcode[attr_qr::base_batchnum].toString());
    m_uiView_val[7]->setText(qrcode[attr_qr::base_bottleno].toString());
}
}  // namespace WIDGET::REAGENT
