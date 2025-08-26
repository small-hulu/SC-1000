/** *************************************************************************
 * Copyright © 2022 luyinfeng.
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       WIDGET_ChooseAlternatively.h
 *
 * @author     luyinfeng (luyinfeng@smtbio.com)
 * @brief      再次确认，提醒用的弹窗
 * 例如：在窗口右上角点击关闭按钮的时候，弹出再次确认
 * 若确认了，则发送 signal_yes() 的信号
 *
 *
 * @Email      luyinfeng@smtbio.com
 *
 * @date       2022-08-15
 * @version    0.0.1
 ***************************************************************************/
#ifndef WIDGET_ChooseAlternatively_H_1715828467
#define WIDGET_ChooseAlternatively_H_1715828467

#include <QSharedPointer>
#include <QWidget>

namespace Ui {
class DIsVerify;
}

namespace WIDGET::DIALOG {

class DIsVerify : public QWidget {
    Q_OBJECT
private:
    QSharedPointer<Ui::DIsVerify> ui;

public:
    explicit DIsVerify(QWidget *parent);
    DIsVerify(const QString &msg, QWidget *parent);

protected:
    void showEvent(QShowEvent *event) override;

signals:
    void signal_yes();
};

}  // namespace WIDGET::DIALOG

#endif  // WIDGET_ChooseAlternatively_H_1715828467
