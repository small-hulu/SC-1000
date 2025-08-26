#include "WIDGET_SettingProjectManage.h"

#include <QBoxLayout>
#include <QDebug>
#include <QScroller>

#include "controller/CONTROLLER_Project.h"
#include "global/GLOBAL_Path.hpp"
#include "modules/WSetting_SettingOption.h"
#include "projectManage/WSetting_CombineProject.h"
#include "projectManage/WSetting_DetailPage.h"
#include "projectManage/WSetting_GeneralSettings.h"
#include "projectManage/WSetting_ProjectList.h"
#include "ui_WIDGET_SettingProjectManage.h"
#include "utility/UTILITY_File.h"
#include "widget/dialog/DIALOG.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::SETTING {
ProjectManagement::ProjectManagement(QWidget* parent) : QWidget(parent), ui(new Ui::ProjectManage) {
    ui->setupUi(this);

    /// init-list
    {
        /// 等ui确定后，再调整这部分的架构
        QVector<QString> btnList;
        btnList << "1.项目列表";
        btnList << "2.项目组合";
        btnList << "3.通用设置";
        auto list = ui->listWidget;
        UTILS::init_list_vertical(list);
        for (int i = 0; i < btnList.size(); i += 1) {
            auto item   = new QListWidgetItem();
            auto widget = new SettingOption(btnList[i], list);
            list->addItem(item);
            list->setItemWidget(item, widget);

            connect(widget, &SettingOption::signal_selected, this,
                    [this, i]() { switch_widget(i); });
        }
    }

    switch_widget(0);
}

ProjectManagement::~ProjectManagement() {
    if (m_subWidget) {
        m_subWidget->close();
        m_subWidget = nullptr;
    }
    delete ui;
}

void ProjectManagement::build_optionList(QListWidget* list, QVector<QString> btnList) {
    //    list->setStyleSheet("QListWidget::item { height: 70px; }");
    for (int i = 0; i < btnList.size(); i += 1) {
        auto item   = new QListWidgetItem();
        auto widget = new SettingOption(btnList[i], list);
        list->addItem(item);
        list->setItemWidget(item, widget);

        connect(widget, &SettingOption::signal_selected, this, [this, i]() { switch_widget(i); });
    }
}

void ProjectManagement::switch_widget(int idx) {
    if (m_subWidget) {
        m_subWidget->close();
        m_subWidget = nullptr;
    }

    QTimer::singleShot(0, [this, idx]() {
        auto parent = ui->widget_project_right;
        switch (idx) {
        case 0: {
            m_subWidget = new ProjectList(parent);
        } break;
        case 1: {
            m_subWidget = new CombineProject(parent);
        } break;
        case 2: {
            m_subWidget = new WSetting_GeneralSettings(parent);
        } break;
        }

        if (m_subWidget) {
            m_subWidget->setAttribute(Qt::WA_DeleteOnClose);
            m_subWidget->show();
            UTILS::resize_widget_fromParent(m_subWidget);
        } else {
            DIALOG::Create_DWarningDialog("todo 未实现 空指针异常");
        }
    });
}

}  // namespace WIDGET::SETTING
