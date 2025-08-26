#ifndef WREAGENT_REAGENTTABLE_H_1715665625
#define WREAGENT_REAGENTTABLE_H_1715665625

#include <QTableWidget>
#include <QWidget>

#include "controller/CONTROLLER_Reagent.h"

namespace Ui {
class ReagentTable;
}

namespace WIDGET::REAGENT {
class ReagentTable : public QWidget {
    Q_OBJECT
public:
    using entity_item = CONTROLLER::Reagent::entity_item;
    using entity_attr = entity_item::element_type::attr;

private:
    Ui::ReagentTable* ui    = nullptr;
    QTableWidget*     table = nullptr;

private:
    QList<entity_item> m_items;

public:
    ReagentTable(QWidget* parent = nullptr);
    ~ReagentTable();

public:
    void Entity_install(int curIdx);
    void Entity_uninstall();
    void Entity_modify();
    void Entity_details();

public:
    void Append_tableLine(entity_item item);

private:
    QList<QString> get_lineValues(ReagentTable::entity_item item);

protected:
    void showEvent(QShowEvent* event) override;
    void timerEvent(QTimerEvent* event) override;
};
}  // namespace WIDGET::REAGENT

#endif  // WREAGENT_REAGENTTABLE_H_1715665625
