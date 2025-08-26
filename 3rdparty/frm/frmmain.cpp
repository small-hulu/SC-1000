#include "frmmain.h"

#include <QDebug>
#include <QDialog>
#include <QFileDialog>
#include <QInputDialog>

#include "ui_frmmain.h"

frmMain::frmMain(QWidget *parent) : QWidget(parent), ui(new Ui::frmMain) {
    ui->setupUi(this);
    ui->txt->setProperty("noinput", true);
}

frmMain::~frmMain() {
    delete ui;
}

void frmMain::on_btnDialog_clicked() {
    QDialog   *d   = new QDialog(this);
    QLineEdit *txt = new QLineEdit(d);
    txt->setVisible(true);
    d->show();
}

void frmMain::on_btnInputDialog_clicked() {
    QInputDialog *d = new QInputDialog(this);
    d->show();
}

void frmMain::on_btnFileDialog_clicked() {
    QFileDialog *d = new QFileDialog(this);
    connect(d, SIGNAL(fileSelected(QString)), this, SLOT(fileSelected(QString)));
    d->show();
}

void frmMain::fileSelected(QString fileName) {
    QFile file(fileName);
    file.open(QFile::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << ui->plainTextEdit->toPlainText();
    file.close();
}
