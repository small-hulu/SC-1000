#ifndef INIT_H_1720091045
#define INIT_H_1720091045

#include <QComboBox>
#include <QListWidget>
#include <QTableWidget>

namespace WIDGET::UTILS {
void init_table_clear(QTableWidget* table);
void init_table_clear_content(QTableWidget* table);
void init_table_no_header(QTableWidget* table);
void init_table_mode(QTableWidget* table);
void init_table_titles(QTableWidget* table, const QList<QString>& titles);
void init_table_rowCount(QTableWidget* table, int row);
void init_table_fix_content(QTableWidget* table, const QList<QList<QString>>& content);

void init_table_rowHeight(QTableWidget* table);
void init_table_colWidth(QTableWidget* table, const QList<int>& widthList);
void init_table_colWidth(QTableWidget* table, const QList<QString>& titles);

void init_table_turnForm_init(QTableWidget* table);

void init_table_as_list(QTableWidget* table);

void init_table_select(QTableWidget* table);
void init_table_extend(QTableWidget* table);

void set_tableWidgetItem_enable(QTableWidgetItem* item, bool flag);

void init_list_vertical(QListWidget* list);

void init_combo_reset(QComboBox* combo);
}  // namespace WIDGET::UTILS

#endif  // INIT_H_1720091045
