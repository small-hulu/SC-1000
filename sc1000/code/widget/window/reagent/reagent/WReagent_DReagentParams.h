#ifndef WIDGET_RESULTPARAMSETTINGS_H_1686813004_2024516
#define WIDGET_RESULTPARAMSETTINGS_H_1686813004_2024516

#include <QSharedPointer>
#include <QWidget>

#include "controller/CONTROLLER_Reagent.h"
#include "widget/Background.h"

class QLineEdit;
class QLabel;
namespace Ui {
class DReagentParams;
}

namespace WIDGET::REAGENT {
class DReagentParams : public QWidget {
    Q_OBJECT
public:
    using entity_item = CONTROLLER::Reagent::entity_item;
    using entity_attr = entity_item::element_type::attr;

private:
    QSharedPointer<Ui::DReagentParams> ui;

    QVector<QLabel*>    m_uiView_title;
    QVector<QLineEdit*> m_uiView_val;

private:
    entity_item m_entity;

public:
    DReagentParams(QWidget* parent = nullptr);
    DReagentParams(entity_item entity, QWidget* parent = nullptr);

public:
    entity_item Get_entity() {
        return m_entity;
    }

public:
    QList<QString> Get_viewMessage();

private:
    QList<entity_attr> get_attrList();

    QList<QString> get_titlesList();
    QList<QString> get_valueList(entity_item entity);
    void           save_uiToEntity(entity_item entity);
    void           save_qrcodeToUi(const QByteArray& bytes);

signals:
    void signal_yes();
};

}  // namespace WIDGET::REAGENT

#endif  // WIDGET_RESULTPARAMSETTINGS_H_1686813004_2024516
