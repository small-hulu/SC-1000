#include "WIDGET_Main.h"

#include <QDebug>
#include <QMessageBox>
#include <QThread>
#include <QTimer>

#include "WIDGET_SubPage.h"
#include "bottom/WIDGET_Main_MBottom.h"
#include "controller/AppEvent.h"
#include "top/WIDGET_Main_MTop.h"
#include "ui_WIDGET_Main.h"
#include "widget/factory/WIDGET_Factory_SwitchPageMain.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET {
Main::Main(QWidget* parent) : QWidget(parent), ui(new Ui::Main{}) {
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    /// ui
    {
        /// 三块部分的拉伸距离
        constexpr int height = 1080;
        constexpr int top    = 118;
        constexpr int bottom = 82;
        UTILS::resize_layout_stretch(ui->verticalLayout, {top, height - top - bottom, bottom});
    }

    topWidget = new MAIN::MTop{ui->widget_top};
    new MAIN::MBottom{ui->widget_bottom};

    /// connect
    {
        connect(topWidget, &MAIN::MTop::signal_switchPage, this, &Main::switch_page);
        auto* appEvent = &CONTROLLER::AppEvent::instance();
        connect(appEvent, &CONTROLLER::AppEvent::signal_switchPage, this, &Main::jump_page);
    }

    /// init
    {
        this->show();
        this->hide();
        switch_page(-1);
    }
}

Main::~Main() {
    delete ui;
}

void Main::jump_page(int idx1, int idx2) {
    switch_page(idx1);
    QTimer::singleShot(20, [this, idx2]() {
        for (const QObject* obj : ui->widget_mid->children()) {
            auto widget = const_cast<QObject*>(obj);
            static_cast<SubPage*>(widget)->Switch_page(idx2);
        }
    });
}

/**
 * @brief Main::switch_page
 * @param index
 * 界面切换
 * *旧界面 close
 * *新界面 create
 * ========================
 * 注意obj->children()
 * 确实是获取对象树的子组件的数目，但是widget的close&destroy
 * 不是线性同步的，因此如果直接用debug查看会发现children出现2的可能
 * 但是不用担心，当widget确实destroyed会自动减到1
 */
void Main::switch_page(int index) {
    /// 注意这里用static和&
    static int lastIndex = 0;
    /// 重复点击，特判直接走
    if (index == lastIndex) {
        return;
    }
    /// 表示刚创建的时，读取上次的缓存
    if (index < 0) {
        index = lastIndex;
    }
    lastIndex         = index;
    auto parentWidget = ui->widget_mid;

    /// 第一次执行是0个
    /// 后续执行都是1个
    for (const QObject* obj : parentWidget->children()) {
        auto widget = const_cast<QObject*>(obj);
        static_cast<QWidget*>(widget)->close();
    }

    /// 注意设置setAttribute(Qt::WA_DeleteOnClose)
    auto widget = FSwitchPageMain().Create_bySwitchEnum(index, parentWidget);
    if (nullptr == widget) {
        QMessageBox::warning(nullptr, "警告", "空指针异常 | 该界面还未开发");
        return;
    }

    topWidget->Click_indexBtn(index);
    widget->setAttribute(Qt::WA_DeleteOnClose);
    UTILS::resize_widget_fromParent(widget);
    widget->show();
}

void Main::showEvent(QShowEvent* event) {
    UTILS::resize_widget_fromParent(this);
    UTILS::resize_widget_toChildren(ui->widget_top);
    UTILS::resize_widget_toChildren(ui->widget_bottom);

    QWidget::showEvent(event);
}
}  // namespace WIDGET
