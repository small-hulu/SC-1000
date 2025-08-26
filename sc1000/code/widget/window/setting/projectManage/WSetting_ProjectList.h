#ifndef WSETTING_PROJECTLIST_H
#define WSETTING_PROJECTLIST_H

#include <QLabel>
#include <QTableWidget>
#include <QWidget>

#include "widget/modules/WModule_ButtonBottomGroup.h"
#include "widget/modules/WModule_TableSelectSingle.h"

namespace Ui {
class ProjectList;
}
namespace WIDGET::SETTING {
class ProjectList : public QWidget {
    Q_OBJECT
private:
    static constexpr int Row_Count = 6;
    static constexpr int Col_Count = 7;

private:
    Ui::ProjectList *ui = nullptr;

private:
    TableSelectSingle *table         = nullptr;
    ButtonBottomGroup *m_ui_btnGroup = nullptr;

public:
    explicit ProjectList(QWidget *parent = nullptr);
    ~ProjectList();

public:
    void show_range(int start, int end);
    bool delete_project();
    void import_project();
    void detail_page();
};
}  // namespace WIDGET::SETTING
#endif  // WSETTING_PROJECTLIST_H
