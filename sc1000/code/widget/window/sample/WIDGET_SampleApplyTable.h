#ifndef APPLYTABLE_H_1720419940
#define APPLYTABLE_H_1720419940

#include <QList>
#include <QTableWidget>

#include "controller/CONTROLLER_SampleApply.h"
#include "widget/modules/WModule_ButtonBottomGroup.h"

namespace Ui {
class SampleApplyTable;
}

namespace WIDGET::SAMPLE {
class SampleApplyTable : public QWidget {
public:
    using entity_item = CONTROLLER::SampleApply::entity_item;
    using entity_attr = entity_item::element_type::attr;

public:
    enum class TableAttribute {
        id = 0,
        type,
        isEmergency,
        project,
        location,
        tubeType,

        Enum_Count
    };

private:
    Ui::SampleApplyTable* ui            = nullptr;
    QTableWidget*         table         = nullptr;
    ButtonBottomGroup*    m_ui_btnGroup = nullptr;

public:
    SampleApplyTable(QWidget* parent = nullptr);
    ~SampleApplyTable();

private:
    void refresh_bottomBtnList();

public:
    void Append_oneLine(entity_item entity);
    void Delete_oneLine();

protected:
    void showEvent(QShowEvent* event) override;
};
}  // namespace WIDGET::SAMPLE

#endif  // APPLYTABLE_H_1720419940
