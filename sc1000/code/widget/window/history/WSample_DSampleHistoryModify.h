#ifndef WIDGET_RESULTPARAMSETTINGS_H_1686813004_2024516_1716611386_1722328927
#define WIDGET_RESULTPARAMSETTINGS_H_1686813004_2024516_1716611386_1722328927

#include <QSharedPointer>
#include <QWidget>

#include "controller/CONTROLLER_ExpSample.h"
#include "widget/Background.h"

class QLineEdit;
class QLabel;
namespace Ui {
class DSampleHistoryModify;
}

namespace WIDGET::HISTORY {
class DSampleHistoryModify : public QWidget {
    Q_OBJECT
public:
    using entity_item = CONTROLLER::ExpSample::entity_item;
    using entity_attr = entity_item::element_type::attr;

private:
    QSharedPointer<Ui::DSampleHistoryModify> ui;

    QVector<QLabel*>    m_uiView_title;
    QVector<QLineEdit*> m_uiView_val;

private:
    entity_item m_entity;

public:
    DSampleHistoryModify(QWidget* parent = nullptr);
    DSampleHistoryModify(entity_item entity, QWidget* parent = nullptr);

public:
    entity_item Get_entity() {
        return m_entity;
    }

public:
    QList<QString> Get_allMessage();
    QList<QString> Get_viewMessage();

public:
    QList<QString> get_titles();
    QList<QString> get_values();

signals:
    void signal_yes();
};

}  // namespace WIDGET::HISTORY

#endif  // WIDGET_RESULTPARAMSETTINGS_H_1686813004_2024516
