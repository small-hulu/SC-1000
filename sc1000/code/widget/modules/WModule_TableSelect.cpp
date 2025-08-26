#include "WModule_TableSelect.h"

#include <QDateTime>
#include <QPushButton>
#include <QTimer>

#include "ui_WModule_TableSelect.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET {
namespace {
const QString btn_qss = QString(R"(
QPushButton[state='0'] {
background: #edf0f5;
border-radius: 10px;
}

QPushButton[state='1'] {
background: #dcf3f6;
border-radius: 10px;

font-family: Microsoft YaHei;
font-weight: 400;
font-size: 23px;
color: #333333;
line-height: 34px;
}

QPushButton[state='3'] {
background: #dcf3f6;
border-radius: 10px;

font-family: Microsoft YaHei;
font-weight: 400;
font-size: 23px;
color: #d9d9d9;
line-height: 34px;
}

QPushButton[state='2'] {
background: #22a4b3;
border-radius: 10px;

font-family: Microsoft YaHei;
font-weight: 400;
font-size: 23px;
color: #333333;
line-height: 34px;
}
)");

const char *const btn_state = "state";
}  // namespace

TableSelect::TableSelect(QWidget *parent) : QWidget(parent), ui(new Ui::TableSelect) {
    ui->setupUi(this);

    table = ui->tableWidget;
    UTILS::init_table_select(table);
}

TableSelect::~TableSelect() {
    delete ui;
}

void TableSelect::Builder(int row, int col) {
    table->setRowCount(row);
    table->setColumnCount(col);

    for (int i = 0; i < table->rowCount(); i += 1) {
        for (int j = 0; j < table->columnCount(); j += 1) {
            auto item = new QTableWidgetItem{};
            table->setItem(i, j, item);
            auto btn = new QPushButton("todo");
            btn->setStyleSheet(btn_qss);
            table->setCellWidget(i, j, btn);

            connect(btn, &QPushButton::clicked, this, [this, btn, i, j]() {
                m_lastInfo.x      = i;
                m_lastInfo.y      = j;
                m_lastInfo.txt    = btn->text();
                const QString pro = btn->property(btn_state).toString();
                if (pro == QString::number(State::selected)) {
                    btn->setProperty(btn_state, QString::number(State::unselected));
                    btn->style()->polish(btn);
                } else if (pro == QString::number(State::unselected)) {
                    btn->setProperty(btn_state, QString::number(State::selected));
                    btn->style()->polish(btn);
                }
                emit this->signal_cellSelected(i, j, btn->text());
            });
        }
    }
    UTILS::resize_show(this);
}

int TableSelect::Get_tableRowCount() {
    return table->rowCount();
}

int TableSelect::Get_tableColCount() {
    return table->columnCount();
}

QList<QPair<int, int> > TableSelect::Get_tableCheckedIndex() {
    QList<QPair<int, int> > ret;
    for (int i = 0; i < table->rowCount(); i += 1) {
        for (int j = 0; j < table->columnCount(); j += 1) {
            if (Is_cellChecked(i, j)) {
                ret << QPair<int, int>{i, j};
            }
        }
    }
    return ret;
}

QList<QString> TableSelect::Get_tableCheckedString() {
    QList<QString> res;
    auto           piiList = Get_tableCheckedIndex();
    for (auto pii : piiList) {
        int x = pii.first;
        int y = pii.second;
        res << Get_cellString(x, y);
    }
    return res;
}

QString TableSelect::Get_cellString(int row, int col) {
    auto btn = get_cellButton(row, col);
    return btn->text();
}

void TableSelect::Set_cellString(int row, int col, const QString &txt) {
    auto btn = get_cellButton(row, col);
    btn->setText(txt);
}

bool TableSelect::Is_cellChecked(int row, int col) {
    auto btn = get_cellButton(row, col);
    if (btn->text().isEmpty()) {
        return false;
    }
    return btn->property(btn_state).toString() == QString::number(State::selected);
}

void TableSelect::Set_tableText(const QList<QString> &txtList) {
    int k = 0;
    for (int i = 0; i < table->rowCount(); i += 1) {
        for (int j = 0; j < table->columnCount(); j += 1) {
            auto btn = get_cellButton(i, j);
            if (k < txtList.size()) {
                btn->setText(txtList[k]);
                btn->setEnabled(true);
                btn->setProperty(btn_state, QString::number(State::unselected));
                k += 1;
            } else {
                btn->setText("");
                btn->setEnabled(false);
                btn->setProperty(btn_state, QString::number(State::unseted));
            }
            btn->style()->polish(btn);
        }
    }  // for
}

void TableSelect::Set_tableClickedList(const QList<QString> &txtList) {
    for (int i = 0; i < Get_tableRowCount(); i += 1) {
        for (int j = 0; j < Get_tableColCount(); j += 1) {
            auto btn  = get_cellButton(i, j);
            auto text = btn->text();
            if (text.isEmpty()) {
                continue;
            }
            if (txtList.contains(text)) {
                btn->setProperty(btn_state, QString::number(State::selected));
            } else {
                btn->setProperty(btn_state, QString::number(State::unselected));
            }
            btn->style()->polish(btn);
        }
    }
}

void TableSelect::Set_tableEnableList(const QList<QString> &txtList) {
    for (int i = 0; i < Get_tableRowCount(); i += 1) {
        for (int j = 0; j < Get_tableColCount(); j += 1) {
            auto btn  = get_cellButton(i, j);
            auto text = btn->text();
            if (text.isEmpty()) {
                continue;
            }
            if (txtList.contains(text)) {
                btn->setEnabled(true);
            } else {
                btn->setProperty(btn_state, QString::number(State::unEnabled));
                btn->setEnabled(false);
            }
            btn->style()->polish(btn);
        }
    }
}

void TableSelect::Clear_tableCellClicked(int x, int y) {
    auto btn = get_cellButton(x, y);
    btn->setProperty(btn_state, QString::number(State::unselected));
    btn->style()->polish(btn);
    if (x == m_lastInfo.x && y == m_lastInfo.y) {
        m_lastInfo = CellInfo{};
    }
}

QPushButton *TableSelect::get_cellButton(int row, int col) {
    auto widget = table->cellWidget(row, col);
    auto btn    = static_cast<QPushButton *>(widget);
    assert(btn);
    return btn;
}

void TableSelect::showEvent(QShowEvent *event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}
}  // namespace WIDGET
