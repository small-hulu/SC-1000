#ifndef DIALOG_H_1715830020
#define DIALOG_H_1715830020

#include "DIALOG_DInfoBottom.h"
#include "DIALOG_DInputLine.h"
#include "DIALOG_DInputText.h"
#include "DIALOG_DIsVerify.h"
#include "DIALOG_DProgressIndicator.h"
#include "DIALOG_DWarning.h"
#include "widget/Background.h"

/**
 * 目前所有的通用弹窗，默认是直接show的
 */
namespace WIDGET::DIALOG {

inline DInfoDialog* Create_DInfoDialog(const QString& msg, QWidget* parent = nullptr) {
    Background::Exchange(parent);
    auto w = new DInfoDialog(parent, msg);
    w->show();
    return w;
}

inline DProgressIndicator* Create_DProgressIndicator(QWidget* parent = nullptr) {
    Background::Exchange(parent);
    auto w = new DProgressIndicator(parent);
    w->show();
    return w;
}

inline DIsVerify* Create_DIsVerify(const QString& msg, QWidget* parent = nullptr) {
    Background::Exchange(parent);
    auto w = new DIsVerify(msg, parent);
    w->show();
    return w;
}

inline DInputText* Create_DInputText(QWidget* parent = nullptr) {
    Background::Exchange(parent);
    auto w = new DInputText(parent);
    w->show();
    return w;
}

inline DInputLine* Create_DInputLine(QWidget* parent = nullptr) {
    Background::Exchange(parent);
    auto w = new DInputLine(parent);
    w->show();
    return w;
}

inline DWarningDialog* Create_DWarningDialog(const QString& msg, QWidget* parent = nullptr) {
    Background::Exchange(parent);
    auto w = new DWarningDialog(msg, parent);
    w->show();
    return w;
}

}  // namespace WIDGET::DIALOG

#endif  // DIALOG_H
