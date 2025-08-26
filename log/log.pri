###########################
# 基于 Qt 原生的 `qDebug()`
# 所封装的简单 log 模块
###########################
QT += widgets

DEFINES += LOG_VERSION_MAJOR=1
DEFINES += LOG_VERSION_MINOR=0
DEFINES += LOG_VERSION_PATCH=0

INCLUDEPATH += $$PWD/..

CONFIG(release, debug|release) {
    # 取消 release 优化
    # 取消优化这些信息:文件名、函数名、行数
    DEFINES += QT_MESSAGELOGCONTEXT
    # 消除debug输出
#    DEFINES += QT_NO_DEBUG_OUTPUT
}

HEADERS += \
    $$PWD/LOG.h \
    $$PWD/LOG_Config.hpp \
    $$PWD/LOG_SLog.h \
    $$PWD/LOG_Version.h \
    $$PWD/LOG_WidgetDebug.h

SOURCES += \
    $$PWD/LOG.cpp \
    $$PWD/LOG_SLog.cpp \
    $$PWD/LOG_Version.cpp \
    $$PWD/LOG_WidgetDebug.cpp
