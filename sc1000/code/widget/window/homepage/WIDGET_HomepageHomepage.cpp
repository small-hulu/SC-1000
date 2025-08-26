/** ***************************************************************************
 * Copyright © 2024 cuber-lotus
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       WIDGET_Homepage_Homepage.cpp
 * @date       2024-05-10
 * @version    1.0.0
 *
 * @author     cuber-lotus
 * @Email      1539349804@qq.com
 *
 * @brief
 * 目前202405紧急开发ui
 * 一般来说从解耦层面讲，这里的homgpage分为三个部分
 * 应该分3个模块来写，然后set进去
 * 但是目前需要紧急开发，直接写道一起了，但也请注意耦合度和规范
 * 如果以后空出时间允许，则再度解耦
 *****************************************************************************/
#include "WIDGET_HomepageHomepage.h"

#include <QDateTime>

#include "homepage/WHomepage_DeviceState.h"
#include "homepage/WHomepage_ShortcutOpts.h"
#include "ui_WIDGET_HomepageHomepage.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::HOMEPAGE {
Homepage::Homepage(QWidget *parent) : QWidget(parent), ui(new Ui::Homepage) {
    ui->setupUi(this);

    UTILS::resize_layout_stretch(ui->horizontalLayout, {2, 1, 1});
    auto right = new ShortcutOpts(ui->widget_right);
    UTILS::resize_widget_fromParent(right);

    //! TODO
    //! 初版不显示
    //! auto mid = new DeviceState(ui->widget_mid);
    //! UTILS::resize_widget_fromParent(mid);
}

Homepage::~Homepage() {
    delete ui;
}
}  // namespace WIDGET::HOMEPAGE
