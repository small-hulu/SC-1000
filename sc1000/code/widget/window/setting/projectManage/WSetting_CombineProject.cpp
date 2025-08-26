#include "WSetting_CombineProject.h"

#include "WSetting_DEditCombine.h"
#include "controller/CONTROLLER_Config.h"
#include "controller/CONTROLLER_Project.h"
#include "ui_WSetting_CombineProject.h"
#include "widget/dialog/DIALOG.h"
#include "widget/modules/WModule_TablePageTurnSwitch.h"

namespace WIDGET::SETTING {
CombineProject::CombineProject(QWidget* parent) : QWidget(parent), ui(new Ui::CombineProject) {
    ui->setupUi(this);

    /// 项目列表
    {
        rightTable = new TableSelect(ui->widget_right);
        rightTable->Builder(Right_Row_Count, Right_Col_Count);
    }

    /// 组合项目列表
    {
        leftTable = new TableSelectSingle(ui->widget_left);
        leftTable->Builder(Left_Row_Count, Left_Col_Count);

        connect(leftTable, &TableSelectSingle::signal_cellSelected, this,
                [this](const QString& msg) {
                    auto fileCon = CONTROLLER::Config::ProjectCombine{};
                    auto group   = fileCon.Get_CombineByName(msg);
                    rightTable->Set_tableClickedList(group.projectList);
                });
    }

    /// 按钮组
    {
        m_ui_btnGroup = ButtonBottomGroup::Get();
        m_ui_btnGroup->show();
        m_ui_btnGroup->Set_refreshFunc({});
        QList<QString> titles;
        titles << QString("删除");
        titles << QString("添加");
        QList<std::function<void()>> taskList;
        taskList << std::bind(&CombineProject::delete_combine, this);
        taskList << std::bind(&CombineProject::add_combine, this);
        m_ui_btnGroup->Set_BtnList(titles, taskList);
    }

    /// 翻页功能 (左)
    {
        auto pageSwitch = new TablePageTurnSwitch(ui->widget_left_switch);
        connect(pageSwitch, &TablePageTurnSwitch::signal_curRange, this,
                &CombineProject::show_range_left);
        auto      fileCon = CONTROLLER::Create_ProjectsFolderController();
        auto      names   = fileCon.Get_ProjectList();
        const int all     = names.size();
        pageSwitch->Build_By_ItemCount(all, Left_Row_Count * Left_Col_Count);
        pageSwitch->signal_curRange(1, qMin(all, Left_Row_Count));
    }

    /// 翻页功能 (右)
    {
        auto pageSwitch = new TablePageTurnSwitch(ui->widget_right_switch);
        connect(pageSwitch, &TablePageTurnSwitch::signal_curRange, this,
                &CombineProject::show_range_right);
        auto      fileCon = CONTROLLER::Create_ProjectsFolderController();
        auto      names   = fileCon.Get_ProjectList();
        const int all     = names.size();
        pageSwitch->Build_By_ItemCount(all, Right_Row_Count * Right_Col_Count);
        pageSwitch->signal_curRange(1, qMin(all, Right_Row_Count));
    }
}

CombineProject::~CombineProject() {
    m_ui_btnGroup->hide();
    delete ui;
}

void CombineProject::show_range_left(int start, int end) {
    auto fileCon = CONTROLLER::Config::ProjectCombine{};
    auto names   = fileCon.Get_CombineNames();
    if (start <= 0 && end <= 0) {
        // pass
    } else {
        names.mid(start, end - start + 1);
    }
    leftTable->Set_tableText(names);
}

void CombineProject::show_range_right(int start, int end) {
    auto fileCon = CONTROLLER::Create_ProjectsFolderController();
    auto names   = fileCon.Get_ProjectList();
    if (start <= 0 && end <= 0) {
        // pass
    } else {
        names.mid(start, end - start + 1);
    }
    rightTable->Set_tableText(names);
}

void CombineProject::add_combine() {
    auto msgList = rightTable->Get_tableCheckedString();
    auto dialog  = new DEditCombine(Background::Get());
    dialog->Set_listMessage(msgList);
    /// 已经在窗口内部对数据进行存储了
    connect(dialog, &DEditCombine::siganl_yes, this, [this]() {
        auto com = CONTROLLER::Config::ProjectCombine{};
        leftTable->Set_tableText(com.Get_CombineNames());
        show_range_left(-1, -1);
    });
    dialog->show();
}

void CombineProject::delete_combine() {
    const QString name = leftTable->Get_curChecked();
    if (name.isEmpty()) {
        DIALOG::Create_DWarningDialog("请选中组合项");
        return;
    }
    auto com = CONTROLLER::Config::ProjectCombine{};
    com.Remove_Combine(name);
    show_range_left(-1, -1);
    DIALOG::Create_DInfoDialog("删除完成");
}
}  // namespace WIDGET::SETTING
