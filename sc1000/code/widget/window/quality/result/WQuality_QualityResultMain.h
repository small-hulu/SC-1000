#ifndef WQUALITY_QUALITYRESULTMAIN_H_1720095125
#define WQUALITY_QUALITYRESULTMAIN_H_1720095125

#include <QList>
#include <QTableWidget>

#include "controller/CONTROLLER_ExpQuality.h"

namespace Ui {
class QualityResultMain;
}

namespace WIDGET::QUALITY {
class QualityResultMain : public QWidget {
public:
    using entity_item = CONTROLLER::ExpQuality::entity_item;
    using entity_attr = entity_item::element_type::attr;

private:
    Ui::QualityResultMain *ui    = nullptr;
    QTableWidget          *table = nullptr;

private:
    QList<entity_item> m_items;

public:
    explicit QualityResultMain(QWidget *parent = nullptr);
    ~QualityResultMain();

public:
    void Entity_uninstall();

public:
    void Append_tableLine(entity_item entity);

private:
    QList<QString> get_lineStr(entity_item entity);

protected:
    void showEvent(QShowEvent *event) override;
};
}  // namespace WIDGET::QUALITY

#endif  // WQUALITY_QUALITYRESULTMAIN_H_1720095125
