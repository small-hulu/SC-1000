################################
# bootloader 相关操作
################################
QT += core
QT += gui
QT += widgets
QT += serialport
QT += serialbus
QT += concurrent

VERSION = 1.0.0.0
TEMPLATE = app
DESTDIR = $$PWD/../../../bin
RC_ICONS = $$PWD/static/logo.ico

CONFIG += c++17
CONFIG += console

CONFIG(debug, debug | release) {
    TARGET = SC-1000_TestBootloader_DV$$VERSION
}
CONFIG(release, debug | release) {
    TARGET = SC-1000_TestBootloader_RV$$VERSION
    DEFINES += QT_MESSAGELOGCONTEXT
}

QMAKE_CXXFLAGS +=                       \
                -Wall                   \
                -Wextra                 \
                -Wstrict-overflow       \
                -Wnull-dereference      \
                -Wduplicated-branches   \
                -Wduplicated-cond

lib_path +=                                \
    $$PWD/../../1stlib/liblog.a            \
    $$PWD/../../1stlib/libcommunication.a

LIBS += $$lib_path
PRE_TARGETDEPS += $$lib_path

INCLUDEPATH += $$PWD/../..

HEADERS += \
    code/MCUUpgrade.h

SOURCES += \
    main.cpp            \
    code/MCUUpgrade.cpp

FORMS += \
    code/MCUUpgrade.ui

