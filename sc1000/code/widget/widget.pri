QT += core
QT += widgets network

INCLUDEPATH += $$PWD/..

include($$PWD/dialog/dialog.pri)
include($$PWD/utils/utils.pri)
include($$PWD/factory/factory.pri)
include($$PWD/window/window.pri)
include($$PWD/modules/modules.pri)

HEADERS += \
    $$PWD/Background.h

SOURCES += \
    $$PWD/Background.cpp
