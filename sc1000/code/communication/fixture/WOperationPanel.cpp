#include "WOperationPanel.h"

#include <QDebug>

#include "ui_WOperationPanel.h"

namespace COM::WFIXTURE {
OperationPanel::OperationPanel(QWidget *parent) : QWidget(parent), ui(new Ui::OperationPanel) {
    ui->setupUi(this);

    /// ui
    {
        ui->edit_input->setEnabled(true);
        QList<QPushButton *> btnList;
        btnList << ui->btn_xadd;
        btnList << ui->btn_xsub;
        btnList << ui->btn_yadd;
        btnList << ui->btn_ysub;
        btnList << ui->btn_zadd;
        btnList << ui->btn_zsub;
        btnList << ui->btn_clockwise;
        btnList << ui->btn_anticlockwise;
        for (auto btn : btnList) {
            btn->setEnabled(false);
        }
    }

    /// connect
    {
        connect(ui->btn_xadd, &QPushButton::clicked, this, &OperationPanel::op_xadd);
        connect(ui->btn_xsub, &QPushButton::clicked, this, &OperationPanel::op_xsub);
        connect(ui->btn_yadd, &QPushButton::clicked, this, &OperationPanel::op_yadd);
        connect(ui->btn_ysub, &QPushButton::clicked, this, &OperationPanel::op_ysub);
        connect(ui->btn_zadd, &QPushButton::clicked, this, &OperationPanel::op_zadd);
        connect(ui->btn_zsub, &QPushButton::clicked, this, &OperationPanel::op_zsub);
        connect(ui->btn_clockwise, &QPushButton::clicked, this, &OperationPanel::op_clockwise);
        connect(ui->btn_anticlockwise, &QPushButton::clicked, this,
                &OperationPanel::op_anticlockwise);
    }
}

OperationPanel::~OperationPanel() {
    delete ui;
}

void OperationPanel::Set_Sudoku(const OperationPanel::Sudoku &matrx) {
    QList<QList<QPushButton *>> btnMatrx;
    btnMatrx << (QList<QPushButton *>() << ui->btn_clockwise << ui->btn_yadd << ui->btn_zadd);
    btnMatrx << (QList<QPushButton *>() << ui->btn_xadd << nullptr << ui->btn_xsub);
    btnMatrx << (QList<QPushButton *>() << ui->btn_zsub << ui->btn_ysub << ui->btn_anticlockwise);

    assert((int)matrx.size() == (int)btnMatrx.size());
    for (int i = 0; i < btnMatrx.size(); i += 1) {
        assert((int)matrx[i].size() == btnMatrx[i].size());
        for (int j = 0; j < btnMatrx[i].size(); j += 1) {
            auto btn = btnMatrx[i][j];
            if (btn) {
                btn->setEnabled(matrx[i][j]);
                if (btn->isEnabled()) {
                    btn->show();
                } else {
                    btn->hide();
                }
            }
        }
    }  // for
}

double OperationPanel::get_inputNum() {
    return ui->edit_input->text().toDouble();
}

void OperationPanel::set_inputNum(double num) {
    return ui->edit_input->setText(QString::number(num));
}

void OperationPanel::showEvent(QShowEvent *event) {
    auto parent = this->parentWidget();
    if (parent) {
        setGeometry(parent->rect());
    }
    QWidget::showEvent(event);
}
}  // namespace COM::WFIXTURE
