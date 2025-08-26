#include "WSetting_ProjectList.h"

#include <QDebug>
#include <QHeaderView>
#include <QScroller>

#include "WSetting_DetailPage.h"
#include "controller/CONTROLLER_Project.h"
#include "ui_WSetting_ProjectList.h"
#include "utility/UTILITY_File.h"
#include "widget/dialog/DIALOG.h"
#include "widget/modules/WModule_TablePageTurnSwitch.h"
#include "widget/utils/UTILS_Resize.h"

namespace WIDGET::SETTING {

ProjectList::ProjectList(QWidget *parent) : QWidget(parent), ui(new Ui::ProjectList{}) {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);

    /// 单选表格
    {
        table = new TableSelectSingle(ui->widget_projectList);
        table->Builder(Row_Count, Col_Count);
        table->show();
    }

    /// 按钮栏
    {
        QList<QString>               titles;
        QList<std::function<void()>> taskList;
        titles << QString("导入");
        taskList << std::bind(&ProjectList::import_project, this);
        titles << QString("删除");
        taskList << std::bind(&ProjectList::delete_project, this);
        titles << QString("详情");
        taskList << std::bind(&ProjectList::detail_page, this);

        m_ui_btnGroup = ButtonBottomGroup::Get();
        m_ui_btnGroup->show();
        m_ui_btnGroup->Set_refreshFunc({});
        m_ui_btnGroup->Set_BtnList(titles, taskList);
    }

    /// 翻页功能
    {
        auto pageSwitch = new TablePageTurnSwitch(ui->widget_switch);
        connect(pageSwitch, &TablePageTurnSwitch::signal_curRange, this, &ProjectList::show_range);
        auto      fileCon = CONTROLLER::Create_ProjectsFolderController();
        auto      names   = fileCon.Get_ProjectList();
        const int all     = names.size();
        pageSwitch->Build_By_ItemCount(all, Row_Count * Col_Count);
        pageSwitch->signal_curRange(1, qMin(all, Row_Count));
    }
}

ProjectList::~ProjectList() {
    m_ui_btnGroup->hide();
    delete ui;
}

void ProjectList::show_range(int start, int end) {
    auto fileCon = CONTROLLER::Create_ProjectsFolderController();
    auto names   = fileCon.Get_ProjectList();
    if (start <= 0 && end <= 0) {
        // pass
    } else {
        names.mid(start, end - start + 1);
    }
    table->Set_tableText(names);
}

void ProjectList::import_project() {
    QString path = UTILITY::Get_USBDiskPath();
    if (path.isEmpty() == false) {
        path += Path_Projects;
        qInfo() << QString("导入项目U盘路径：[%1]").arg(path);
        auto fileCon = CONTROLLER::Create_ProjectsFolderController();
        if (fileCon.Processing_folder(path)) {
            DIALOG::Create_DInfoDialog("导入成功");
            this->show_range(-1, -1);
        } else {
            DIALOG::Create_DWarningDialog("导入失败");
        }
    } else {
        DIALOG::Create_DWarningDialog("未检测到U盘");
    }
}

void ProjectList::detail_page() {
    if (table->Get_curChecked() == "") {
        DIALOG::Create_DWarningDialog("请选中项目");
        return;
    }

    auto    fileCon = CONTROLLER::Create_ProjectsFolderController();
    QString path    = fileCon.Get_projectPath(table->Get_curChecked());
    QFile   file(path);
    if (false == file.exists() || false == file.open(QIODevice::ReadOnly)) {
        qWarning() << QString("File exist[%1] open[%2]")  //
                          .arg(file.exists())             //
                          .arg(file.isOpen());
        DIALOG::Create_DWarningDialog("项目打开失败");
    } else {
        auto    fileData   = file.readAll();
        QString str        = QString(fileData);
        auto    detailPage = new DetailPage(str, this);
        connect(detailPage, &QWidget::destroyed, [this]() {
            QList<QString> titles;
            titles << QString("导出");
            titles << QString("删除");
            titles << QString("详情");
            QList<std::function<void()>> taskList;
            taskList << std::bind(&ProjectList::import_project, this);
            taskList << std::bind(&ProjectList::delete_project, this);
            taskList << std::bind(&ProjectList::detail_page, this);
            m_ui_btnGroup->Set_BtnList(titles, taskList);
        });
        detailPage->show();
    }
}

bool ProjectList::delete_project() {
    if (table->Get_curChecked() == "") {
        DIALOG::Create_DWarningDialog("请选中项目");
        return false;
    }

    auto dialog = DIALOG::Create_DIsVerify("是否确认删除");
    connect(dialog, &DIALOG::DIsVerify::signal_yes, this, [this]() {
        auto exec_del = [this]() -> bool {
            auto fileCon = CONTROLLER::Create_ProjectsFolderController();
            fileCon.Remove_oldFile(table->Get_curChecked());
            show_range(-1, -1);
            return true;
        };

        if (exec_del()) {
            DIALOG::Create_DInfoDialog("删除成功");
        } else {
            DIALOG::Create_DWarningDialog("删除失败");
        }
    });

    return true;
}

}  // namespace WIDGET::SETTING
