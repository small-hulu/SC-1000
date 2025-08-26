#ifndef WQUALITY_QUALITYAPPLYMAIN_H_1720083818
#define WQUALITY_QUALITYAPPLYMAIN_H_1720083818

#include <QTableWidget>
#include <QWidget>

#include "controller/CONTROLLER_ExpQuality.h"
#include "controller/CONTROLLER_Quality.h"

namespace Ui {
class QualityApplyMain;
}

namespace WIDGET::QUALITY {
class QualityApplyMain : public QWidget {
public:
    using entity_item = CONTROLLER::Quality::entity_item;
    using entity_attr = entity_item::element_type::attr;

    using entity_itemExp = CONTROLLER::ExpQuality::entity_item;
    using entity_attrExp = entity_itemExp::element_type::attr;

public:
    struct UiInfo {
        QString idQuality;
        QString idReagent;
        QString location;
        QString name;
        QString batchNumber;
        QString timeStart;
        QString state;
        QString project;
        QString valTarget;
        QString valSD;

        void Reset() {
            *this = UiInfo{};
        }

        bool is_valid() const {
            return idQuality.size() && idReagent.size() && location.size();
        }
    };

private:
    Ui::QualityApplyMain* ui;
    QTableWidget*         table_left  = nullptr;
    QTableWidget*         table_right = nullptr;

private:
    QList<entity_item> m_items;

public:
    explicit QualityApplyMain(QWidget* parent = nullptr);
    ~QualityApplyMain();

public:
    UiInfo Get_UiInfo();

public:
    entity_itemExp Btn_save(UiInfo info);

public:
    void Append_leftTableLine(entity_item entity);
    void Append_rightTableValue();

private:
    QList<QString>        get_leftLineStr(entity_item entity);
    QList<QList<QString>> get_rightLineStr(entity_item entity);

protected:
    void showEvent(QShowEvent* event) override;
};
}  // namespace WIDGET::QUALITY

#endif  // WQUALITY_QUALITYAPPLYMAIN_H
