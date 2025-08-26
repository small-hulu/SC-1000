#ifndef WQUALITY_DQUALITYQUERY_H
#define WQUALITY_DQUALITYQUERY_H

#include <QSharedPointer>
#include <QWidget>

#include "controller/CONTROLLER_ExpQuality.h"
#include "widget/Background.h"

class QLineEdit;
class QLabel;
namespace Ui {
class DQualityQuery;
}

namespace WIDGET::QUALITY {
class DQualityQuery : public QWidget {
    Q_OBJECT
public:
    using entity_item = CONTROLLER::ExpQuality::entity_item;
    using entity_attr = entity_item::element_type::attr;

private:
    QSharedPointer<Ui::DQualityQuery> ui;

    QVector<QLabel*>    m_uiView_title;
    QVector<QLineEdit*> m_uiView_val;

private:
    entity_item    m_entity;
    QList<QString> m_vals;

public:
    DQualityQuery(QWidget* parent = nullptr);
    DQualityQuery(entity_item entity, QWidget* parent = nullptr);

public:
    entity_item Get_entity() {
        return m_entity;
    }

public:
    QList<QString> Get_allMessage();
    QList<QString> Get_viewMessage();

private:
    QList<entity_attr> get_attrList();

signals:
    void signal_yes();
};
}  // namespace WIDGET::QUALITY
#endif  // WQUALITY_DQUALITYQUERY_H
