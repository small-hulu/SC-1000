#ifndef WSETTING_COMBINEPROJECT_H_1725508690
#define WSETTING_COMBINEPROJECT_H_1725508690

#include <QWidget>

#include "widget/modules/WModule_ButtonBottomGroup.h"
#include "widget/modules/WModule_TableSelect.h"
#include "widget/modules/WModule_TableSelectSingle.h"

namespace Ui {
class CombineProject;
}

namespace WIDGET::SETTING {
class CombineProject : public QWidget {
    Q_OBJECT
private:
    static constexpr int Left_Row_Count  = 6;
    static constexpr int Left_Col_Count  = 2;
    static constexpr int Right_Row_Count = 6;
    static constexpr int Right_Col_Count = 6;

private:
    Ui::CombineProject* ui = nullptr;

private:
    TableSelectSingle* leftTable  = nullptr;
    TableSelect*       rightTable = nullptr;

private:
    ButtonBottomGroup* m_ui_btnGroup = nullptr;

public:
    explicit CombineProject(QWidget* parent = nullptr);
    ~CombineProject();

private:
    void show_range_left(int start, int end);
    void show_range_right(int start, int end);

private:
    void add_combine();
    void delete_combine();
};
}  // namespace WIDGET::SETTING

#endif  // WSETTING_COMBINEPROJECT_H_1725508690
