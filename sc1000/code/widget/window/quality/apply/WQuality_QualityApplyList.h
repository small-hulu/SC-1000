#ifndef WQUALITY_QUALITYAPPLYLIST_H_1720084897
#define WQUALITY_QUALITYAPPLYLIST_H_1720084897

#include <QList>
#include <QTableWidget>

#include "controller/CONTROLLER_ExpQuality.h"
#include "widget/modules/WModule_ButtonBottomGroup.h"

namespace Ui {
class QualityApplyList;
}

namespace WIDGET::QUALITY {
class QualityApplyList : public QWidget {
public:
    using entity_item = CONTROLLER::ExpQuality::entity_item;
    using entity_attr = entity_item::element_type::attr;

private:
    Ui::QualityApplyList *ui            = nullptr;
    QTableWidget         *m_table       = nullptr;
    ButtonBottomGroup    *m_ui_btnGroup = nullptr;

private:
    QList<entity_item> m_items;

public:
    explicit QualityApplyList(QWidget *parent = nullptr);
    ~QualityApplyList();

public:
    void Btn_delete();

public:
    void Append_tableLine(entity_item entity);
    /// 刷新底部按钮组
    void refresh_bottomBtnList();

private:
    QList<QString> get_entityShowMsgList(entity_item entity);

protected:
    void showEvent(QShowEvent *event) override;
};
}  // namespace WIDGET::QUALITY

#endif  // WQUALITY_QUALITYAPPLYLIST_H_1720084897
