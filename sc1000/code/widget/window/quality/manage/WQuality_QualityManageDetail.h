#ifndef WQUALITY_QUALITYMANAGEDETAIL_H_1720159470
#define WQUALITY_QUALITYMANAGEDETAIL_H_1720159470

#include <QWidget>

#include "controller/CONTROLLER_Quality.h"
#include "widget/modules/WModule_TableSelect.h"

namespace Ui {
class QualityManageDetail;
}

namespace WIDGET::QUALITY {
class QualityManageDetail : public QWidget {
    Q_OBJECT
public:
    using entity_item = CONTROLLER::Quality::entity_item;
    using entity_attr = entity_item::element_type::attr;

public:
    struct UiInfo {
        QList<QString> projectList;
        QString        name;
        QString        batchNumber;
        QString        periodOfValidity;
        QString        valTargetStart;
        QString        valTargetEnd;
        QString        valTarget;
        QString        valSD;
        QString        unit;
    };

private:
    Ui::QualityManageDetail *ui;

private:
    entity_item m_entity;

private:
    TableSelect *m_ui_tableSelect = nullptr;

public:
    explicit QualityManageDetail(QWidget *parent = nullptr);
    ~QualityManageDetail();

public:
    UiInfo      Get_UiInfo();
    entity_item Get_entity() {
        return m_entity;
    }
    void Set_entity(entity_item entity) {
        m_entity = entity;
        refresh_ui();
    }

public:
    entity_item Btn_save(UiInfo info);

private:
    void refresh_ui();

protected:
    void showEvent(QShowEvent *event) override;
};
}  // namespace WIDGET::QUALITY

#endif  // WQUALITY_QUALITYMANAGEDETAIL_H_1720159470
