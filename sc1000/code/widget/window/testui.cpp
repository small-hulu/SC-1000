/******************************************************************************
 * Copyright © 2023 luyinfeng
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       testui.cpp
 * @brief
 * 专门用来测试的ui
 * 可以直接无脑添加到工厂中来测试
 *
 * @author     luyinfeng
 * @Email      luyinfeng@smtbio.com
 *
 * @date       2023-10-13
 * @version    0.0.0
 *****************************************************************************/
#include "testui.h"

#include <QDateTime>
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>

#include "ui_testui.h"

TestUi::TestUi(QWidget *parent) : QWidget(parent), ui(new Ui::TestUi) {
    ui->setupUi(this);

#ifdef _WIN64
    bool ok = QDesktopServices::openUrl(QUrl("osk.exe", QUrl::TolerantMode));
#else
    /// Note
    /// 注意，目前使用的环境是64位的windows
    /// 但是编译器使用的是32位，这里会出现错误
    /// 简单说就是调用失败
    /// Note
    bool ok = QDesktopServices::openUrl(QUrl("osk.exe"));
    qDebug() << "QDesktopServices::openUrl(QUrl('osk.exe')) = " << ok;
#endif
}

TestUi::~TestUi() {
    delete ui;
}
