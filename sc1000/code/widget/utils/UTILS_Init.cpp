#include "UTILS_Init.h"

#include <QHeaderView>
#include <QListView>
#include <QScroller>
#include <QtMath>

#include "UTILS_Params.h"

namespace WIDGET::UTILS {
void init_table_clear(QTableWidget *table) {
    table->clearContents();
    while (table->rowCount()) {
        table->removeRow(table->rowCount() - 1);
    }
    while (table->columnCount()) {
        table->removeColumn(table->columnCount() - 1);
    }
    table->clear();
}

void init_table_clear_content(QTableWidget *table) {
    for (int i = 0; i < table->rowCount(); i += 1) {
        for (int j = 0; j < table->columnCount(); j += 1) {
            auto item = table->item(i, j);
            item->setText("");
        }
    }
}

void init_table_no_header(QTableWidget *table) {
    auto ver = table->verticalHeader();
    auto hor = table->horizontalHeader();

    ver->setVisible(false);
    hor->setVisible(false);
}

void init_table_mode(QTableWidget *table) {
    assert(table);

    QScroller::grabGesture(table, QScroller::LeftMouseButtonGesture);
    /// table header
    {
        auto ver = table->verticalHeader();
        auto hor = table->horizontalHeader();

        /// 左侧列属性 false
        /// 顶部行属性 true
        ver->setVisible(false);
        hor->setVisible(true);

        /// 横纵属性不可获取
        ver->setEnabled(false);
        hor->setEnabled(false);

        /// 属性不可编辑
        ver->setSectionsClickable(false);
        hor->setSectionsClickable(false);

        /// 行列表头字体居中
        ver->setDefaultAlignment(Qt::AlignCenter);
        hor->setDefaultAlignment(Qt::AlignCenter);

        /// 设定行属性的高和列属性的宽的最小值 width||height
        hor->setMinimumHeight(50);

        /// 自动拉伸满整个框 （此处全部手动）
        hor->setSectionResizeMode(QHeaderView::Stretch);

    }  // header

    /// 横纵滚动条关闭
    table->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    /// 滚动模式
    table->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    table->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    /// 设置不能编辑
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    /// 无焦点
    table->setFocusPolicy(Qt::NoFocus);
    /// 单选
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    /// 选择行
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    /// 不显示网格
    table->setShowGrid(false);
    /// 设置隔行变色
    table->setAlternatingRowColors(true);

    init_table_clear(table);
}

void init_table_titles(QTableWidget *table, const QList<QString> &titles) {
    assert(table);
    table->setColumnCount(titles.size());
    table->setHorizontalHeaderLabels(titles);
}

void init_table_rowCount(QTableWidget *table, int row) {
    assert(table);
    while (table->rowCount()) {
        table->removeRow(table->rowCount() - 1);
    }
    table->setRowCount(row);

    for (int i = 0; i < table->rowCount(); i += 1) {
        for (int j = 0; j < table->columnCount(); j += 1) {
            auto item = new QTableWidgetItem();
            item->setTextAlignment(Qt::AlignCenter);
            table->setItem(i, j, item);
        }
    }

    auto ver = table->verticalHeader();
    ver->setSectionResizeMode(QHeaderView::Stretch);
    init_table_rowHeight(table);
}

void init_table_fix_content(QTableWidget *table, const QList<QList<QString> > &content) {
    assert(table);
    while (table->rowCount()) {
        table->removeRow(table->rowCount() - 1);
    }

    for (int i = 0; i < content.size(); i += 1) {
        assert(content[i].size() == table->columnCount());
        table->insertRow(i);
        for (int j = 0; j < content[i].size(); j += 1) {
            auto item = new QTableWidgetItem(content[i][j]);
            item->setTextAlignment(Qt::AlignCenter);
            table->setItem(i, j, item);
        }
    }
}

void init_combo_reset(QComboBox *combo) {
    combo->clear();
    combo->setView(new QListView{});
}

void init_table_rowHeight(QTableWidget *table) {
    const int rowCnt = table->rowCount();
    const int height = (table->height() - table->horizontalHeader()->height()) / rowCnt;
    for (int i = 0; i < rowCnt; i += 1) {
        table->setRowHeight(i, height);
    }
}

void init_table_colWidth(QTableWidget *table, const QList<int> &widthList) {
    const int rowCnt = table->columnCount();
    assert(rowCnt == widthList.size());

    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    const int  allWidth = table->width() - table->verticalHeader()->width();
    const int  sum      = std::accumulate(widthList.begin(), widthList.end(), 0);
    QList<int> widths;
    for (auto w : widthList) {
        widths << (1.0 * w / sum * allWidth);
    }
    const int newSum  = std::accumulate(widths.begin(), widths.end(), 0);
    /// 补精度差
    widths.back()    += allWidth - newSum;
    for (int i = 0; i < table->columnCount(); i += 1) {
        table->setColumnWidth(i, widths[i]);
    }
}

void init_table_colWidth(QTableWidget *table, const QList<QString> &titles) {
    QList<int> widthList;
    for (auto s : titles) {
        widthList << s.size();
    }
    init_table_colWidth(table, widthList);
}

void init_table_turnForm_init(QTableWidget *table) {
    /// table header
    {
        auto ver = table->verticalHeader();
        auto hor = table->horizontalHeader();

        /// 左侧列属性 false
        /// 顶部行属性 true
        ver->setVisible(false);
        hor->setVisible(true);

        /// 横纵属性不可获取
        ver->setEnabled(false);
        hor->setEnabled(false);

        /// 属性不可编辑
        ver->setSectionsClickable(false);
        hor->setSectionsClickable(false);

        /// 行列表头字体居中
        ver->setDefaultAlignment(Qt::AlignCenter);
        hor->setDefaultAlignment(Qt::AlignCenter);
    }  // header

    /// 横纵滚动条关闭
    table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    /// 设置不能编辑
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    /// 无焦点
    table->setFocusPolicy(Qt::NoFocus);
    /// 单选
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    /// 选择行
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    /// 不显示网格
    table->setShowGrid(false);
    /// 设置隔行变色
    table->setAlternatingRowColors(true);
}

void init_table_as_list(QTableWidget *table) {
    assert(table);

    table->setColumnCount(1);
    QScroller::grabGesture(table, QScroller::TouchGesture);

    /// table header
    {
        auto ver = table->verticalHeader();
        auto hor = table->horizontalHeader();

        /// 左侧列属性 false
        /// 顶部行属性 true
        ver->setVisible(false);
        hor->setVisible(false);
    }  // header

    /// 横纵滚动条关闭
    table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    /// 设置不能编辑
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    /// 无焦点
    table->setFocusPolicy(Qt::NoFocus);
    /// 单选
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    /// 选择行
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    /// 不显示网格
    table->setShowGrid(false);
    /// 设置隔行变色
    table->setAlternatingRowColors(true);
}

void init_list_vertical(QListWidget *list) {
    QScroller::grabGesture(list, QScroller::LeftMouseButtonGesture);
    list->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    list->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void init_table_select(QTableWidget *table) {
    assert(table);
    /// table header
    {
        auto ver = table->verticalHeader();
        auto hor = table->horizontalHeader();

        /// 是否可见
        ver->setVisible(false);
        hor->setVisible(false);

        /// 自动拉伸
        ver->setSectionResizeMode(QHeaderView::Stretch);
        hor->setSectionResizeMode(QHeaderView::Stretch);
    }  // header

    /// 横纵滚动条关闭
    table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    /// 设置不能编辑
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    /// 无焦点
    table->setFocusPolicy(Qt::NoFocus);
    /// 不选
    table->setSelectionMode(QAbstractItemView::NoSelection);
    /// 不显示网格
    table->setShowGrid(false);
}

void init_table_extend(QTableWidget *table) {
    UTILS::init_table_clear(table);

    QScroller::grabGesture(table, QScroller::LeftMouseButtonGesture);
    /// table header
    {
        auto ver = table->verticalHeader();
        auto hor = table->horizontalHeader();

        /// 左侧列属性 false
        /// 顶部行属性 true
        /// todo
        ver->setVisible(false);
        hor->setVisible(true);

        /// 横纵属性不可获取
        ver->setEnabled(false);
        hor->setEnabled(false);

        /// 属性不可编辑
        ver->setSectionsClickable(false);
        hor->setSectionsClickable(false);

        /// 行列表头字体居中
        ver->setDefaultAlignment(Qt::AlignCenter);
        hor->setDefaultAlignment(Qt::AlignCenter);

        /// 设定行属性的高和列属性的宽的最小值 width||height
        hor->setMinimumHeight(70);

        /// 自动拉伸满整个框 （此处全部手动）
        hor->setSectionResizeMode(QHeaderView::Stretch);

    }  // header

    /// 横纵滚动条关闭
    table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    /// 滚动模式
    table->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    // table->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    /// 设置不能编辑
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    /// 无焦点
    table->setFocusPolicy(Qt::NoFocus);
    /// 选择模式
    table->setSelectionMode(QAbstractItemView::NoSelection);
    /// 选择行
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    /// 不显示网格
    table->setShowGrid(false);
    /// 设置隔行变色
    table->setAlternatingRowColors(true);
}

void set_tableWidgetItem_enable(QTableWidgetItem *item, bool flag) {
    if (flag) {
        item->setFlags(item->flags() | Qt::ItemIsEnabled);
    } else {
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
    }
}

}  // namespace WIDGET::UTILS
