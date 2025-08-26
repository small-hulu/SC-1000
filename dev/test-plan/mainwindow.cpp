#include "mainwindow.h"

#include <QMessageBox>
#include <QSerialPortInfo>

#include "2/magneticseparation_2.h"
#include "3/pipetteNeedle_precision.h"
#include "3/pipetteneedle_3_7.h"
#include "4/gripper_5.h"
#include "6/mixing_2.h"
#include "9/photoelectric_9.h"
#include "communication/COM.hpp"
#include "ui_mainwindow.h"
#include "util/project_help.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->btn_pipetteNeedle, &QPushButton::clicked, this, []() {
        auto w = new PipetteNeedle_3_7();
        w->setAttribute(Qt::WA_DeleteOnClose);
        w->show();

        auto d = new PipetteNeedlePrecision();
        d->setAttribute(Qt::WA_DeleteOnClose);
        d->show();
    });

    connect(ui->btn_gripper, &QPushButton::clicked, this, []() {
        auto w = new Gripper_5();
        w->setAttribute(Qt::WA_DeleteOnClose);
        w->show();
    });

    connect(ui->btn_magneticSeparation, &QPushButton::clicked, this, []() {
        auto w = new MagneticSeparation_2();
        w->setAttribute(Qt::WA_DeleteOnClose);
        w->show();
    });

    connect(ui->btn_photoelectric, &QPushButton::clicked, this, []() {
        auto w = new Photoelectric_9();
        w->setAttribute(Qt::WA_DeleteOnClose);
        w->show();
    });

    connect(ui->btn_mixing, &QPushButton::clicked, this, []() {
        auto w = new Mixing_2();
        w->setAttribute(Qt::WA_DeleteOnClose);
        w->show();
    });

    connect(ui->btn_link, &QPushButton::clicked, this, &MainWindow::open_modbus);

    refresh_port();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::refresh_port() {
    ui->combo_port->clear();
    for (auto &info : QSerialPortInfo::availablePorts()) {
        ui->combo_port->addItem(info.portName());
    }
}

void MainWindow::open_modbus() {
    const QString port = ui->combo_port->currentText();
    bool          flag = COM::Init(port);
    if (flag == false) {
        auto message = new QMessageBox(QMessageBox::Warning, "异常报警", "通信连接失败");
        message->show();
    } else {
        help_stop_reagentBin_loop();
        qDebug() << "通信连接正常";
    }
}
