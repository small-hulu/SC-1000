#include "DIALOG_DInfoBottom.h"

#include <QTimer>

#include "ui_DIALOG_DInfoBottom.h"

namespace WIDGET::DIALOG {

int               DInfoDialog::Gap_y = 5;
DInfoDialog::_Map DInfoDialog::Map_heap;

DInfoDialog::DInfoDialog(QWidget *parent, const QString &text)
    : QWidget(parent), ui(new Ui::DInfoBottom{}) {
    /// date
    {
        ui->setupUi(this);
        ui->label->setText(text);

        this->setAttribute(Qt::WA_StyledBackground);
        this->setAttribute(Qt::WA_DeleteOnClose);
        /// click can pass down
        this->setAttribute(Qt::WA_TransparentForMouseEvents);
    }

    /// timeout close
    {
        QTimer::singleShot(3000, this, [this]() { this->close(); });
    }

    this->show();
}

DInfoDialog::~DInfoDialog() {
    QWidget *par = this->parentWidget();
    if (par != nullptr) {
        if (Map_heap.find(par) != Map_heap.end()) {
            auto iter = Map_heap[par].find({y_pos, y_height});
            if (iter != Map_heap[par].end()) {
                Map_heap[par].erase(iter);
            }
        }
    }
}

void DInfoDialog::show() {
    QWidget *par = this->parentWidget();
    if (par != nullptr) {
        this->setGeometry(0, 0, par->width(), par->height());
    }

    QWidget::show();
    // different content has different height
    y_height = ui->label->height();
    y_pos    = Gap_y;

    /// adjust y-position
    if (par != nullptr) {
        if (Map_heap.find(par) != Map_heap.end()) {
            const auto &sett = Map_heap[par];
            if (sett.size() > 0 && y_pos + y_height + Gap_y > ::std::get<0>(*(sett.begin()))) {
                bool is_first = true;
                for (const auto &interval : sett) {
                    if (is_first) {
                        is_first = false;
                        y_pos    = ::std::get<0>(interval) + ::std::get<1>(interval) + Gap_y;
                    } else {
                        if (y_pos + y_height + Gap_y <= ::std::get<0>(interval)) {
                            break;
                        } else {
                            y_pos = ::std::get<0>(interval) + ::std::get<1>(interval) + Gap_y;
                        }
                    }
                }
            }
        }
        Map_heap[par].insert({y_pos, y_height});
    }

    ui->label->move(ui->label->x(), this->height() - y_pos - y_height);
}

bool DInfoDialog::close() {
    return QWidget::close();
}

}  // namespace WIDGET::DIALOG
