#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <QFileInfo>
#include <QTextCodec>
#include <QThread>

#include "communication/controller/Init.h"
#include "log/LOG.h"
#include "test.h"

bool unit_connect() {
    qWarning() << __PRETTY_FUNCTION__;
    if (QSerialPortInfo::availablePorts().size() == 0) {
        qWarning() << "未检测到串口";
        return false;
    }

    for (auto& info : QSerialPortInfo::availablePorts()) {
        qDebug().noquote().nospace() << COM::to_string(info);
        const bool flag = COM::Init(info.portName());
        if (flag) {
            qDebug() << "连接 ok";
            return true;
        } else {
            qDebug() << "连接 no ok";
            return false;
        }
    }

    return false;
}

void main_test() {
    srand(time(0));

    qInfo() << "Main Thread:" << QThread::currentThread();
    COM::Init_instance();

    if (false == COM::Init()) {
        qDebug() << "《 连接异常，无法进行测试，请关闭后重新打开重试！ 》";
        system("pause");
        return;
    }

    test();
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    qInstallMessageHandler(LOG::simple_messageHandler);

    QString filePath =
        "log/slog/" + QDateTime::currentDateTime().toString("yyyyMMddhhmmss") + ".log";
    LOG::SLog::Init_file(filePath);
    LOG::SLog::Init_console(true);

    main_test();

    int execRet = app.exec();
    qDebug() << "App-exec = " << execRet;
    return execRet;
}
