#ifndef WIDGET_RESULTPARAMSETTINGS_H_1686813004_2024516_1716611386
#define WIDGET_RESULTPARAMSETTINGS_H_1686813004_2024516_1716611386

#include <QSharedPointer>
#include <QWidget>

#include "controller/CONTROLLER_Sample.h"
#include "widget/Background.h"

class QLineEdit;
class QLabel;
namespace Ui {
class DSampleApplyParams;
}

namespace WIDGET::SAMPLE {
class DSampleApplyParams : public QWidget {
    Q_OBJECT
public:
    using entity_item = CONTROLLER::Sample::entity_item;
    using entity_attr = entity_item::element_type::attr;

private:
    QSharedPointer<Ui::DSampleApplyParams> ui;

    QVector<QLabel*>    m_uiView_title;
    QVector<QLineEdit*> m_uiView_val;

private:
    entity_item m_entity;

public:
    DSampleApplyParams(QWidget* parent = nullptr);
    DSampleApplyParams(entity_item entity, QWidget* parent = nullptr);

public:
    entity_item Get_entity() {
        return m_entity;
    }

private:
    QList<entity_attr> get_attrList();

signals:
    void signal_yes();
};

}  // namespace WIDGET::SAMPLE

#endif  // WIDGET_RESULTPARAMSETTINGS_H_1686813004_2024516
