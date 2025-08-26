#include "WModule_TableSelectSingle.h"

#include <QPushButton>
#include <QStyle>

#include "ui_WModule_TableSelectSingle.h"
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

TableSelectSingle::TableSelectSingle(QWidget *parent)
    : QWidget(parent), ui(new Ui::TableSelectSingle) {
    ui->setupUi(this);
    table = ui->tableWidget;
    UTILS::init_table_select(table);
}

TableSelectSingle::~TableSelectSingle() {
    delete ui;
}

void TableSelectSingle::Builder(int row, int col) {
    table->setRowCount(row);
    table->setColumnCount(col);

    for (int i = 0; i < table->rowCount(); i += 1) {
        for (int j = 0; j < table->columnCount(); j += 1) {
            auto item = new QTableWidgetItem{};
            table->setItem(i, j, item);
            auto btn = new QPushButton("todo");
            btn->setStyleSheet(btn_qss);
            table->setCellWidget(i, j, btn);

            connect(btn, &QPushButton::clicked, this, [this, btn]() {
                for (int i = 0; i < table->rowCount(); i += 1) {
                    for (int j = 0; j < table->columnCount(); j += 1) {
                        auto btn = get_cellButton(i, j);
                        if (btn->text().isEmpty() == false) {
                            btn->setProperty(btn_state, QString::number(State::unselected));
                            btn->style()->polish(btn);
                        }
                    }
                }
                btn->setProperty(btn_state, QString::number(State::selected));
                btn->style()->polish(btn);

                emit this->signal_cellSelected(btn->text());
            });
        }
    }

    UTILS::resize_show(this);
}

void TableSelectSingle::Set_tableText(const QList<QString> &txtList) {
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

QString TableSelectSingle::Get_curChecked() {
    for (int i = 0; i < table->rowCount(); i += 1) {
        for (int j = 0; j < table->columnCount(); j += 1) {
            auto btn = get_cellButton(i, j);
            if (btn->property(btn_state).toString() == QString::number(State::selected)) {
                return btn->text();
            }
        }
    }

    return "";
}

QPushButton *TableSelectSingle::get_cellButton(int row, int col) {
    auto widget = table->cellWidget(row, col);
    auto btn    = static_cast<QPushButton *>(widget);
    assert(btn);
    return btn;
}

void TableSelectSingle::showEvent(QShowEvent *event) {
    UTILS::resize_widget_fromParent(this);
    return QWidget::showEvent(event);
}

}  // namespace WIDGET
