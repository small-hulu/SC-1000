#include "DIALOG_DIsVerify.h"

#include <QListView>
#include <QScroller>

#include "ui_DIALOG_DIsVerify.h"

namespace WIDGET::DIALOG {

DIsVerify::DIsVerify(QWidget *parent) : QWidget(parent), ui(new Ui::DIsVerify) {
    /// data
    {
        ui->setupUi(this);
        setAttribute(Qt::WA_StyledBackground);
        setAttribute(Qt::WA_DeleteOnClose);
    }

    /// connection
    {
        connect(ui->btn_no, &QPushButton::clicked, this, [this]() { this->close(); });

        connect(ui->btn_yes, &QPushButton::clicked, this, [this]() {
            emit this->signal_yes();
            this->close();
        });
    }
}

DIsVerify::DIsVerify(const QString &msg, QWidget *parent) : DIsVerify(parent) {
    ui->lab_question->setText(msg);
}

void DIsVerify::showEvent(QShowEvent *event) {
    auto parent = this->parentWidget();
    if (parent) {
        this->setGeometry(parent->rect());
    }
    QWidget::showEvent(event);
}

}  // namespace WIDGET::DIALOG
