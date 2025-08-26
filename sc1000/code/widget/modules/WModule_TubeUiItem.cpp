#include "WModule_TubeUiItem.h"

#include <QMouseEvent>
#include <QPainter>

#include "ui_WModule_TubeUiItem.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET {
TubeUiItem::TubeUiItem(QWidget *parent) : QWidget(parent), ui(new Ui::TubeUiItem) {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground, true);

    Set_lableRight("");
    Set_labelBottom("");
    Set_clicked(false);
}

TubeUiItem::~TubeUiItem() {
    delete ui;
}

void TubeUiItem::Set_lableRight(const QString &txt) {
    auto w = ui->label_right;
    if (txt.isEmpty()) {
        w->hide();
    } else {
        w->setText(txt);
        w->show();
    }
}

void TubeUiItem::Set_labelBottom(const QString &txt) {
    auto w = ui->label_bottom;
    if (txt.isEmpty()) {
        w->hide();
    } else {
        w->setText(txt);
        w->show();
    }
}

QString TubeUiItem::Get_labelRightStr() {
    return ui->label_right->text();
}

QString TubeUiItem::Get_labelBottomStr() {
    return ui->label_bottom->text();
}

void TubeUiItem::Set_clicked(int flag) {
    this->setProperty("state", QString::number(flag));
    UTILS::style_polish(this);
}

void TubeUiItem::Set_StateType(int type) {
    m_StateType             = static_cast<Type::ETubeState>(type);
    const QString imagePath = Type::Get_TubeStateInfo(m_StateType).pathImage;
    QString       qss       = QString("image: url(%1);");
    ui->widget_image->setStyleSheet(qss.arg(imagePath));
}

void TubeUiItem::mousePressEvent(QMouseEvent *event) {
    QWidget::mousePressEvent(event);
    if (event->button() == Qt::LeftButton) {
        emit signal_clicked(this);
    }
}

void TubeUiItem::showEvent(QShowEvent *event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}
}  // namespace WIDGET
