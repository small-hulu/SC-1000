#include "mainwindow.h"

#include <QDebug>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}
void MainWindow::on_print_clicked() {
    print_A4_A5_custom = new print_preview_inferace();
    print_A4_A5_custom->setAttribute(Qt::WA_DeleteOnClose);  // 内存的释放回收标志
    print_A4_A5_custom->show();
    QString date;
    QString date1;
    print_A4_A5_custom->print_preview1(date, date1);  // 打印预览
}
