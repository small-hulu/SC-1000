#include "DIALOG_DInputLine.h"

#include "ui_DIALOG_DInputLine.h"

namespace WIDGET::DIALOG {

DInputLine::DInputLine(QWidget* parent) : QWidget(parent), ui(new Ui::DInputLine{}) {
    ui->setupUi(this);

    /// for widget & object
    {
        this->setAttribute(Qt::WA_StyledBackground);
        this->setAttribute(Qt::WA_DeleteOnClose);
        if (parent != nullptr) {
            setGeometry(parent->rect());
        }
    }

    /// connect
    {
        /// 重置
        connect(ui->btn_reset, &QPushButton::clicked, ui->lineEdit, &QLineEdit::clear);

        /// 取消
        connect(ui->btn_no, &QPushButton::clicked, this, [this]() { close(); });

        /// 确定
        connect(ui->btn_yes, &QPushButton::clicked, this, [this]() {
            emit signal_yes();
            close();
        });
    }
}

void DInputLine::Set_messge(const QString& msg) {
    ui->lineEdit->setText(msg);
}

QString DInputLine::Get_messge() const {
    return ui->lineEdit->text();
}

}  // namespace WIDGET::DIALOG
