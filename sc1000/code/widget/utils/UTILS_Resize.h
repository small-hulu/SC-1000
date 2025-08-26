#ifndef HELPER_H_1695624276
#define HELPER_H_1695624276

#include <QBoxLayout>
#include <QListWidget>
#include <QTableWidget>
#include <QWidget>

namespace WIDGET::UTILS {
void resize_show(QWidget* widget);

void resize_widget_fromParent(QWidget* widget);
void resize_widget_toChildren(QWidget* parent);

void resize_table_auto(QTableWidget* table);

void resize_list_toItem(QListWidget* list, int width = -1, int height = -1);

void resize_layout_stretch(QBoxLayout* layout, QList<int> stretch);
}  // namespace WIDGET::UTILS

#endif  // HELPER_H_1695624276
