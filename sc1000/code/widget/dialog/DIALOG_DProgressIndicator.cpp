#include "DIALOG_DProgressIndicator.h"

#include <QDir>

#include "ui_DIALOG_DProgressIndicator.h"

namespace WIDGET::DIALOG {

DProgressIndicator::DProgressIndicator(QWidget *parent)
    : QWidget(parent), ui(new Ui::DProgressIndicator{}) {
    ui->setupUi(this);
    /// init
    {
        setAttribute(Qt::WA_DeleteOnClose);
        setAttribute(Qt::WA_StyledBackground);
    }

    /// connect
    {
        connect(&m_timer, &QTimer::timeout, this, [this]() {
            static int cnt = -1;
            cnt            = (cnt + 1) % 4;
            QString words;
            for (int i = 0; i < cnt; i += 1) {
                words += " ·";
            }
            ui->lab_words->setText(words);
        });
    }
}

void DProgressIndicator::Set_words(const QString &msg) {
    ui->lab_show->setText(msg);
}

void DProgressIndicator::Set_percentHint(const QString &msg) {
    ui->lab_words->hide();
    ui->lab_show->setText(msg);
}

void DProgressIndicator::start_Carousel(const int time) {
    if (false == m_timer.isActive()) {
        m_timer.start(time);
    }
}

void DProgressIndicator::close_Carousel() {
    if (m_timer.isActive()) {
        m_timer.stop();
    }
    this->close();
}

void DProgressIndicator::showEvent(QShowEvent *event) {
    auto parent = this->parentWidget();
    if (parent) {
        this->setGeometry(parent->rect());
    }
    start_Carousel();
    QWidget::showEvent(event);
    emit signal_showed();
}

void DProgressIndicator::closeEvent(QCloseEvent *event) {
    close_Carousel();
    QWidget::closeEvent(event);
}

}  // namespace WIDGET::DIALOG
