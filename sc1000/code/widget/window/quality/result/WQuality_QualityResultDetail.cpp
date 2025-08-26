#include "WQuality_QualityResultDetail.h"

#include "ui_WQuality_QualityResultDetail.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::QUALITY {
QualityResultDetail::QualityResultDetail(QWidget *parent)
    : QWidget(parent), ui(new Ui::QualityResultDetail) {
    ui->setupUi(this);

    ///
    { UTILS::resize_layout_stretch(ui->verticalLayout, {1, 1, 8}); }

    /// table
    {
        auto table = ui->table;
        UTILS::init_table_mode(table);
        QList<QString> titles;
        titles << QString("项目");
        titles << QString("浓度");
        titles << QString("发光值");
        titles << QString("结束时间");
        titles << QString("试剂批号");
        titles << QString("试剂瓶号");
        titles << QString("底物批号");
        titles << QString("底物瓶号");
        titles << QString("校准品批号");
        titles << QString("校准时间");

        table->setColumnCount(titles.size());
        table->setHorizontalHeaderLabels(titles);

        /// TODO
        /// todo and test
        UTILS::init_table_rowCount(table, 8);
        for (int i = 0; i < table->rowCount(); i += 1) {
            for (int j = 0; j < table->columnCount(); j += 1) {
                QString s = QString("[%1, %2]").arg(i).arg(j);

                auto item = table->item(i, j);
                item->setText(s);
            }
        }
    }
}

QualityResultDetail::~QualityResultDetail() {
    delete ui;
}

void QualityResultDetail::showEvent(QShowEvent *event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}
}  // namespace WIDGET::QUALITY
