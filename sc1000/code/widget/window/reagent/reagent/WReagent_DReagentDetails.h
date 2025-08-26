#ifndef WIDGET_DREAGENDETAILS_H_1721109117
#define WIDGET_DREAGENDETAILS_H_1721109117

#include <QLineEdit>
#include <QPushButton>
#include <QSharedPointer>
#include <QWidget>

#include "controller/CONTROLLER_Reagent.h"
#include "widget/Background.h"

class QLineEdit;
class QLabel;
namespace Ui {
class DReagentDetails;
}

namespace WIDGET::REAGENT {
class DReagentDetails : public QWidget {
    Q_OBJECT
public:
    using entity_item = CONTROLLER::Reagent::entity_item;
    using entity_attr = entity_item::element_type::attr;

private:
    Ui::DReagentDetails *ui;

    QVector<QLabel *>    m_uiView_title;
    QVector<QLineEdit *> m_uiView_val;

private:
    entity_item    m_entity;
    QList<QString> m_vals;

public:
    DReagentDetails(QWidget *parent = nullptr);
    DReagentDetails(entity_item entity, QWidget *parent = nullptr);

public:
    entity_item Get_entity() {
        return m_entity;
    }

private:
    QList<entity_attr> get_attrList();
};
}  // namespace WIDGET::REAGENT
#endif  // WIDGET_DREAGENDETAILS_H
