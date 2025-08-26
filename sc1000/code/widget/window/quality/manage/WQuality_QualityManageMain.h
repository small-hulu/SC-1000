#ifndef WQUALITY_QUALITYMANAGEMAIN_H_1720148165
#define WQUALITY_QUALITYMANAGEMAIN_H_1720148165

#include <QTableWidget>
#include <QWidget>

#include "controller/CONTROLLER_Quality.h"

namespace Ui {
class QualityManageMain;
}

namespace WIDGET::QUALITY {
class QualityManageMain : public QWidget {
public:
    using entity_item = CONTROLLER::Quality::entity_item;
    using entity_attr = entity_item::element_type::attr;

private:
    Ui::QualityManageMain* ui          = nullptr;
    QTableWidget*          table_left  = nullptr;
    QTableWidget*          table_right = nullptr;

private:
    CONTROLLER::Quality* m_view_entityController = nullptr;
    QList<entity_item>   m_items;

public:
    explicit QualityManageMain(QWidget* parent = nullptr);
    ~QualityManageMain();

public:
    entity_item Get_curLineEntity();
    void        Refresh();

public:
    /// 卸载
    void Entity_uninstall();
    /// 扫码新增
    void Entity_install_byScan();

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

#endif  // WQUALITY_QUALITYMANAGEMAIN_H_1720148165
