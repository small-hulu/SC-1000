#include "widget_table.h"

#include <QHeaderView>

namespace UTIL {
void table_init(QTableWidget *table) {
    auto hor = table->horizontalHeader();
    /// 自动拉伸满整个框 （此处全部手动）
    hor->setSectionResizeMode(QHeaderView::Stretch);
}
}  // namespace UTIL
