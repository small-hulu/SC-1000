#include "WHistory_DHSampleDetail.h"

#include "ui_WHistory_DHSampleDetail.h"
#include "widget/dialog/DIALOG.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::HISTORY {
DHSampleDetail::DHSampleDetail(QWidget *parent) : QWidget(parent), ui(new Ui::DHSampleDetail) {
    ui->setupUi(this);

    /// dialog
    {
        this->setAttribute(Qt::WA_StyledBackground);
        this->setAttribute(Qt::WA_DeleteOnClose);
    }

    /// init
    {
        ui->lab_product->setText("");
        ui->lab_batchNum->setText("");
        ui->lab_validity->setText("");
        ui->lab_bottleNum->setText("");

        QList<QString> titles;
        titles << QString::number(__LINE__);
        titles << QString::number(__LINE__);
        titles << QString::number(__LINE__);
        titles << QString::number(__LINE__);

        auto table = ui->tableWidget;
        UTILS::init_table_mode(table);
        UTILS::init_table_titles(table, titles);
    }

    /// connect
    {
        connect(ui->btn_no, &QPushButton::clicked, this, [this]() { this->close(); });

        connect(ui->btn_yes, &QPushButton::clicked, this, [this]() {
            emit signal_yes();
            this->close();
        });
    }
}

DHSampleDetail::~DHSampleDetail() {
    delete ui;
}

void DHSampleDetail::showEvent(QShowEvent *event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}
}  // namespace WIDGET::HISTORY
