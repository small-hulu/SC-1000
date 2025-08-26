#ifndef WQUALITY_QUALITYANALYSEMAIN_H_1720144811
#define WQUALITY_QUALITYANALYSEMAIN_H_1720144811

#include <QList>
#include <QTableWidget>
#include <QWidget>

#include "controller/CONTROLLER_ExpQuality.h"
#include "controller/CONTROLLER_Quality.h"
namespace Ui {
class QualityAnalyseMain;
}

namespace WIDGET::QUALITY {
class QualityAnalyseMain : public QWidget {
    Q_OBJECT
public:
    using entity_item = CONTROLLER::ExpQuality::entity_item;
    using entity_attr = entity_item::element_type::attr;

private:
    Ui::QualityAnalyseMain *ui;

private:
    QTableWidget      *table = nullptr;
    QList<entity_item> m_items;

public:
    explicit QualityAnalyseMain(QWidget *parent = nullptr);
    ~QualityAnalyseMain();

public:
    void Append_tableLine(entity_item entity);

    void Entity_uninstall();

private:
    QList<QString> get_lineStr(entity_item entity);

protected:
    void showEvent(QShowEvent *event) override;
};
}  // namespace WIDGET::QUALITY

#endif  // WQUALITY_QUALITYANALYSEMAIN_H_1720144811
