#include "DIALOG_DInputText.h"

#include "ui_DIALOG_DInputText.h"

namespace WIDGET::DIALOG {

DInputText::DInputText(QWidget* parent) : QWidget(parent), ui(new Ui::DInputText{}) {
    ui->setupUi(this);

    /// for widget & object
    {
        this->setAttribute(Qt::WA_StyledBackground);
        this->setAttribute(Qt::WA_DeleteOnClose);
        if (parent != nullptr) {
            setGeometry(parent->rect());
        }
    }

    /// ui init
    {
        ui->plainTextEdit->installEventFilter(this);
        ui->plainTextEdit->document()->setMaximumBlockCount(4);
    }

    /// connect
    {
        /// 重置
        connect(ui->btn_reset, &QPushButton::clicked, ui->plainTextEdit, &QPlainTextEdit::clear);

        /// 取消
        connect(ui->btn_no, &QPushButton::clicked, this, [this]() { close(); });

        /// 确定
        connect(ui->btn_yes, &QPushButton::clicked, this, [this]() {
            emit signal_yes();
            close();
        });
    }
}

void DInputText::Set_messgeList(const QList<QString>& msg) {
    auto edit = ui->plainTextEdit;
    edit->clear();
    for (auto&& s : msg) {
        edit->appendPlainText(s);
    }
}

QList<QString> DInputText::Get_messgeList() const {
    auto txt = ui->plainTextEdit->toPlainText();
    return txt.split("\n");
}

}  // namespace WIDGET::DIALOG
