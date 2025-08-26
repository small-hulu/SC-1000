#include "UTILS_Resize.h"

#include <QDebug>
#include <QHeaderView>
#include <QTimer>

namespace WIDGET::UTILS {

/**
 * 便于尺寸的自动缩放
 */
void resize_show(QWidget *widget) {
    assert(widget);
    /// 注意线程的安全性
    QTimer::singleShot(0, QThread::currentThread(), [widget]() {
        widget->hide();
        widget->show();
    });
}

/**
 * 设置适应父窗口
 */
void resize_widget_fromParent(QWidget *widget) {
    assert(widget);
    auto parent = widget->parentWidget();
    if (parent) {
        widget->setGeometry(parent->rect());
    }
}

/**
 * 设置子组件的显示
 */
void resize_widget_toChildren(QWidget *parent) {
    assert(parent);
    for (const QObject *obj : parent->children()) {
        try {
            auto object = const_cast<QObject *>(obj);
            auto widget = dynamic_cast<QWidget *>(object);
            if (widget) {
                resize_widget_fromParent(widget);
            }
        } catch (...) {
            qWarning() << "dynamic_cast<QWidget *>(children) 异常";
        }
    }
}

/**
 * list 的宽高设置
 */
void resize_list_toItem(QListWidget *list, int width, int height) {
    assert(list);
    for (int i = 0; i < list->count(); i += 1) {
        auto item       = list->item(i);
        auto itemWidget = list->itemWidget(item);
        if (-1 != width) {
            itemWidget->setFixedWidth(width);
        }
        if (-1 != height) {
            itemWidget->setFixedHeight(height);
        }
        item->setSizeHint(itemWidget->size());
        item->setHidden(false);
    }
}

/**
 * BoxLayout 只作用于一个维度
 */
void resize_layout_stretch(QBoxLayout *layout, QList<int> stretch) {
    assert(layout->count() == stretch.size());
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    for (int i = 0; i < stretch.size(); i += 1) {
        layout->setStretch(i, stretch[i]);
    }
}

/**
 * 横纵自动排布
 */
void resize_table_auto(QTableWidget *table) {
    assert(table);

    auto ver = table->verticalHeader();
    auto hor = table->horizontalHeader();
    ver->setVisible(false);
    hor->setVisible(false);
    ver->setSectionResizeMode(QHeaderView::Stretch);
    hor->setSectionResizeMode(QHeaderView::Stretch);
}

}  // namespace WIDGET::UTILS
