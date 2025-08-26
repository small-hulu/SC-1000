#include "WMultStpeTrigger.h"

#include <QDebug>
#include <QFileDialog>
#include <QJsonObject>
#include <future>

#include "communication/controller/DispatchController.h"
#include "communication/controller/Parse.h"
#include "ui_WMultStpeTrigger.h"

namespace COM::SINGLEWSTEP {
MultStpeTrigger::MultStpeTrigger(QWidget *parent) : QWidget(parent), ui(new Ui::MultStpeTrigger) {
    ui->setupUi(this);
    if (parent) {
        this->setGeometry(parent->rect());
    }

    connect(ui->btn_test, &QPushButton::clicked, this, &MultStpeTrigger::fun_test);
    connect(ui->btn_mult, &QPushButton::clicked, this, &MultStpeTrigger::fun_mult);
}

MultStpeTrigger::~MultStpeTrigger() {
    delete ui;
    join_thread();
}

void MultStpeTrigger::fun_test() {
    QString path = QFileDialog::getOpenFileName(this);
    qDebug() << path;
    if (path.isEmpty()) {
        qDebug() << "文件选择失败";
        return;
    }

    QFile file(path);
    if (false == file.open(QFile::ReadOnly)) {
        qWarning() << "%文本配置解析为空%" << path;
        return;
    }
    auto json     = QJsonDocument::fromJson(file.readAll()).object();
    auto tastList = COM::Parse::parsing_process_node(json);

    join_thread();
    taskTh = std::thread(
        static_cast<bool (*)(COM::Parse::task_sequence, bool &)>(COM::Parse::simple_single_process),
        tastList, std::ref(keepThreadRunning));
}

void MultStpeTrigger::fun_mult() {
    QString path = QFileDialog::getExistingDirectory(this);
    qDebug() << path;
    if (path.isEmpty()) {
        qInfo() << QString("选择目录失败");
        return;
    }

    auto get_pathList = [](auto path) {
        QDir        dir(path);  // 创建QDir对象，指向指定文件夹
        QStringList filters;
        filters << "*.json";                             // 设置过滤器，只选择JSON文件
        QStringList jsonFiles = dir.entryList(filters);  // 获取所有JSON文件的列表

        QList<QString> pathList;
        foreach (const QString &fileName, jsonFiles) {
            pathList << dir.absoluteFilePath(fileName);  // 获取文件的完整
        }
        return pathList;
    };

    auto get_json = [](auto path) -> QJsonObject {
        QFile file(path);
        if (false == file.open(QFile::ReadOnly)) {
            qWarning() << "%文本配置解析为空%" << path;
            return {};
        }
        return QJsonDocument::fromJson(file.readAll()).object();
    };

    auto &con = COM::DispatchController::instance();
    qWarning() << "全任务启动前" << con.to_string();

    con.Pause_dispatch();
    auto       pathList  = get_pathList(path);
    static int uniqueKey = 0;
    for (auto &&path : pathList) {
        con.Append_taskPackage(uniqueKey++, get_json(path));
    }
    con.Start_dispatch();

    qWarning() << "全任务启动后" << con.to_string();
}

void MultStpeTrigger::join_thread() {
    keepThreadRunning = false;
    if (taskTh.joinable()) {
        taskTh.join();
    }
    keepThreadRunning = true;
}

void MultStpeTrigger::showEvent(QShowEvent *event) {
    Q_UNUSED(event);
    QWidget *parent = static_cast<QWidget *>(this->parent());
    if (parent) {
        this->setGeometry(parent->rect());
    }
}
}  // namespace COM::SINGLEWSTEP
