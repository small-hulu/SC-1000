#include "WQuality_QualityManageRule.h"

#include "controller/CONTROLLER_Project.h"
#include "ui_WQuality_QualityManageRule.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::QUALITY {
QualityManageRule::QualityManageRule(QWidget *parent)
    : QWidget(parent), ui(new Ui::QualityManageRule) {
    ui->setupUi(this);
    /// ui-init
    {
        auto table = ui->tableWidget;
        UTILS::init_table_mode(table);
        UTILS::init_table_titles(table, {QString("项目")});
        auto projectList = CONTROLLER::Create_ProjectsFolderController().Get_ProjectList();
        QList<QList<QString>> mat;
        for (auto s : projectList) {
            mat << QList<QString>{s};
        }
        UTILS::init_table_fix_content(table, mat);
    }

    /// connect
    {
        auto table = ui->tableWidget;
        connect(table, &QTableWidget::cellPressed, this, [this, table]() {
            /// todo
            auto cell = table->currentItem();
            if (cell == nullptr) {
                return;
            }
            QString txt = cell->text();
            qDebug() << "todo" << txt;
        });
    }
}

QualityManageRule::~QualityManageRule() {
    delete ui;
}

void QualityManageRule::showEvent(QShowEvent *event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}
}  // namespace WIDGET::QUALITY
