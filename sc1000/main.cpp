#include <QApplication>
#include <QDebug>
#include <QFileInfo>
#include <QMessageBox>
#include <QTextCodec>

#include "3rdparty/frm/Frm.h"
#include "SC1000_StartUp.h"
#include "controller/AppInit.h"
#include "log/LOG.h"
#include "widget/Background.h"

#ifdef OPEN_KEYBOARD
#undef OPEN_KEYBOARD
#endif
#ifdef QT_NO_DEBUG
#define OPEN_KEYBOARD 1
#else
#define OPEN_KEYBOARD 1
#endif

#include "code/SC1000_test.h"

namespace {
void test_unit() {
#ifdef QT_DEBUG
    TEST::test();
#endif
}
}  // namespace

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    qRegisterMetaType<QVector<int>>("QVector<int>");

    // LOG::QLog_Init();
    qInstallMessageHandler(LOG::simple_messageHandler);

#if OPEN_KEYBOARD
    Frm::init();
    Frm::exec();
#endif

    if (CONTROLLER::AppInit::App_Init() == false) {
        QMessageBox::warning(nullptr, "Soft Init", "Soft Init Error");
        return EXIT_FAILURE;
    }

    /// 预备背景ui
    QWidget widgetBack;
    WIDGET::Background::Set(&widgetBack);
    widgetBack.setWindowTitle("SC1000");
    widgetBack.setWindowFlag(Qt::FramelessWindowHint);
    widgetBack.setGeometry(0, 0, 1920, 1080);
    widgetBack.show();

    ::StartUp startUp{};

    test_unit();

    int exeRet = app.exec();
    qCritical() << "$$$Main-Out Application-execRet = " << exeRet;
    return exeRet;
}
