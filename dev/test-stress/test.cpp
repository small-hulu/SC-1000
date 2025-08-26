#include "test.h"

#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QFileDialog>

#include "communication/COM_Device.hpp"
#include "communication/Version.h"
#include "communication/controller/ActionMajor.h"
#include "communication/controller/ActionMinor.h"
#include "communication/controller/ActionPatch.h"
#include "communication/controller/DispatchController.h"
#include "communication/controller/Init.h"
#include "communication/utility/ToString.h"

using namespace COM;
QHash<int, TaskPackage::smart_entity> taskSet;

void run_task(QJsonObject json) {
    static int uniqueKey = 0;
    auto      &con       = COM::DispatchController::instance();
    con.Reset();
    auto task = con.Append_taskPackage(uniqueKey++, json);
    con.Start_dispatch();
    QObject::connect(task.get(), &QThread::finished, [json]() {
        QTimer::singleShot(100, QThread::currentThread(), [json]() { run_task(json); });
    });
}

void once_test(QJsonObject json, int id) {
    auto task   = TaskPackage::Create_entity(json);
    taskSet[id] = task;

    QObject::connect(task.get(), &QThread::finished, [task]() {
        QTimer::singleShot(100, QThread::currentThread(), [task]() {
            task->Reset();
            task->Start();
        });
    });
    task->Start();
}

void test() {
    QString path = QFileDialog::getExistingDirectory();
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
        for (const QString &fileName : jsonFiles) {
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

    auto pathList = get_pathList(path);
    qDebug() << pathList;
    for (int i = 0; i < pathList.size(); i += 1) {
        QString path = pathList[i];
        once_test(get_json(path), i);
    }
}
