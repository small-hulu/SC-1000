#include "Frm.h"

#include <QApplication>
#include <QFile>
#include <QVariant>

#include "frminput.h"
#include "frmnum.h"

/**
 * +字符集
 * +字体格式(可以不在意)
 * +字体db(根据部署策略检查)
 */
void Frm::init() {
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    // QT5版本以上默认就是采用UTF-8编码
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#endif

#ifdef __WIN32
    qApp->setFont(QFont("Microsoft YaHei", 9));
#endif

    //! 可以检查是否存在有效的数据库
}

void Frm::exec(Exec_Type type) {
    switch (type) {
    /// 打开中文输入法
    case Input: {
        frmInput::Instance()->Init("control", "silvery", 10, 10);
    } break;
    /// 打开数字键盘
    case Num: {
        frmNum::Instance()->Init("black", 10);
    } break;
    default: {
        qWarning() << "Enum Error";
    }
    }
}

void Frm::set_noinput(QObject *obj) {
    obj->setProperty("noinput", true);
}

QSharedPointer<frmMain> Frm::get_exampleDemoWidget() {
    frmMain *widget = new frmMain{};
    widget->show();
    return QSharedPointer<frmMain>(widget);
}
