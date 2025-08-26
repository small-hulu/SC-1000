################################
# 机械整机测试方案
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
    TARGET = SC-1000_TestPlan_DV$$VERSION
}
CONFIG(release, debug | release) {
    TARGET = SC-1000_TestPlan_RV$$VERSION
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

SOURCES +=                          \
    2/magneticseparation_2.cpp \
    3/pipetteNeedle_precision.cpp \
    3/pipetteneedle_3_7.cpp \
    4/gripper_5.cpp \
    6/mixing_2.cpp \
    9/photoelectric_9.cpp \
    main.cpp                        \
    mainwindow.cpp \
    util/mark_reagentBin.cpp        \
    util/project_help.cpp \
    util/widget_combo.cpp           \
    util/widget_table.cpp

HEADERS += \
    2/magneticseparation_2.h \
    3/pipetteNeedle_precision.h \
    3/pipetteneedle_3_7.h \
    4/gripper_5.h \
    6/mixing_2.h \
    9/photoelectric_9.h \
    mainwindow.h \
    util/Interface_widget.hpp \
    util/UTILS.hpp              \
    util/mark_reagentBin.h      \
    util/project_help.h \
    util/widget_combo.h         \
    util/widget_table.h

FORMS += \
    2/magneticseparation_2.ui \
    3/pipetteNeedle_precision.ui \
    3/pipetteneedle_3_7.ui \
    4/gripper_5.ui \
    6/mixing_2.ui \
    9/photoelectric_9.ui \
    mainwindow.ui
