#include "WSetting_UserShow.h"

#include "WSetting_DEditUser.h"
#include "controller/CONTROLLER_LoginUser.h"
#include "ui_WSetting_UserShow.h"
#include "widget/Background.h"
#include "widget/dialog/DIALOG.h"
#include "widget/modules/WModule_TablePageTurnSwitch.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::SETTING {
UserShow::UserShow(QWidget* parent) : QWidget(parent), ui(new Ui::UserShow{}) {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);

    /// ui-init
    {
        QList<QString> titles;
        titles << QString("账户");
        titles << QString("权限");
        titles << QString("创建时间");
        table = ui->tableWidget;
        UTILS::init_table_clear(table);
        UTILS::init_table_turnForm_init(table);
        UTILS::init_table_titles(table, titles);
        UTILS::init_table_rowCount(table, Row_Count);
        UTILS::init_table_colWidth(table, {1, 1, 4});
    }

    /// page-switch
    {
        auto pageSwitch = new TablePageTurnSwitch(ui->widget_switch);
        connect(pageSwitch, &TablePageTurnSwitch::signal_curRange, this, &UserShow::show_range);
        const int all = CONTROLLER::LoginUser::Get_allCount();
        pageSwitch->Build_By_ItemCount(all, Row_Count);
        pageSwitch->signal_curRange(1, qMin(all, Row_Count));
    }

    /// 按钮组
    if (CONTROLLER::LoginUser::instance().user.Get_level()) {
        m_ui_btnGroup = ButtonBottomGroup::Get();
        m_ui_btnGroup->show();
        m_ui_btnGroup->Set_refreshFunc({});
        QList<QString> titles;
        titles << QString("新增");
        QList<std::function<void()>> taskList;
        taskList << std::bind(&UserShow::new_user, this);
        m_ui_btnGroup->Set_BtnList(titles, taskList);
    }
}

UserShow::~UserShow() {
    m_ui_btnGroup->hide();
    delete ui;
}

void UserShow::new_user() {
    ENTITY::User user;
    user.table[entity_attr::level] =
        CONTROLLER::LoginUser::instance().user.table[entity_attr::level];

    auto dialog = new DEditUser(Background::Get());
    dialog->Set_user(user);
    connect(dialog, &DEditUser::signal_yes, this, [dialog]() {
        ENTITY::User user;
        dialog->Get_user(user);
        user.table[entity_attr::createtime] = user.Generate_curTime();
        CONTROLLER::LoginUser::Create_UserToDB(user);
        sLog_test() << "新建用户" << user.to_string();
        DIALOG::Create_DInfoDialog("新建用户成功");
    });
    dialog->show();
}

void UserShow::show_range(int start, int end) {
    if (start <= 0 || end <= 0 || end < start) {
        UTILS::init_table_clear_content(table);
        return;
    }

    auto res = CONTROLLER::LoginUser::Get_rangeEntity(start, end);
    assert(res.size() <= table->rowCount());
    for (int i = 0; i < table->rowCount(); i += 1) {
        if (i < res.size()) {
            using attr = ENTITY::User::attr;

            table->item(i, 0)->setText(res[i].table[attr::username]);
            table->item(i, 1)->setText(res[i].Get_levelString());
            table->item(i, 2)->setText(res[i].table[attr::createtime]);
        } else {
            for (int j = 0; j < table->columnCount(); j += 1) {
                auto item = table->item(i, j);
                item->setText("");
            }
        }
    }
}

void UserShow::showEvent(QShowEvent* event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}

}  // namespace WIDGET::SETTING
