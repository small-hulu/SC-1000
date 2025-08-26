QT += core
QT += widgets
QT += concurrent
QT += serialport
QT += serialbus
QT += printsupport
QT += texttospeech

VERSION = 1.0.0.0
DESTDIR = $$PWD/../../bin
RC_ICONS = $$PWD/static/icon/logo.ico
QMAKE_TARGET_COPYRIGHT = "Copyright 2025 KeySystem. All rights reserved."

CONFIG += c++17

CONFIG(debug, debug | release) {
    TARGET = SC-1000_DV$$VERSION
    CONFIG += console
}
CONFIG(release, debug | release) {
    TARGET = SC-1000_RV$$VERSION
    DEFINES += QT_MESSAGELOGCONTEXT
}

DEFINES += QT_DEPRECATED_WARNINGS
QMAKE_CXXFLAGS +=                       \
                -Wall                   \
                -Wextra                 \
                -Wstrict-overflow       \
                -Wnull-dereference      \
                -Wduplicated-branches   \
                -Wduplicated-cond

lib_path +=                               \
    $$PWD/../1stlib/lib3rdparty.a         \
    $$PWD/../1stlib/liblog.a              \
    $$PWD/../1stlib/libcommunication.a

# 链接库
# 每次强制链接静态库
LIBS += $$lib_path
PRE_TARGETDEPS += $$lib_path

include($$PWD/../3rdparty/3rdparty_include.pri)
include($$PWD/../algorithm/algorithm/algorithm.pri)

include($$PWD/code/communication/communication.pri)
include($$PWD/code/controller/controller.pri)
include($$PWD/code/database/database.pri)
include($$PWD/code/entity/entity.pri)
include($$PWD/code/global/global.pri)
include($$PWD/code/lis/lis.pri)
include($$PWD/code/lte/lte.pri)
include($$PWD/code/printer/printer.pri)
include($$PWD/code/qrcode/qrcode.pri)
include($$PWD/code/utility/utility.pri)
include($$PWD/code/widget/widget.pri)
include($$PWD/code/xlsx/xlsx.pri)

INCLUDEPATH += $$PWD/..
INCLUDEPATH += code

CONFIG += precompile_header
PRECOMPILED_HEADER += pch.h

SOURCES +=                  \
    main.cpp                \
    code/SC1000_StartUp.cpp \
    code/SC1000_test.cpp

HEADERS += \
    code/SC1000_StartUp.h \
    code/SC1000_test.h

RESOURCES +=                \
    res.qrc                 \
    static/res_beijing.qrc  \
    static/res_suzhou.qrc
