QT += core
QT += gui
QT += widgets
QT += printsupport

INCLUDEPATH += $$PWD/..

HEADERS += \
    $$PWD/PHelper.h \
    $$PWD/PRINTER.hpp \
    $$PWD/PrinterCalibration.h \
    $$PWD/PrinterQuality.h \
    $$PWD/PrinterReagent.h \
    $$PWD/PrinterSample.h

SOURCES += \
    $$PWD/PHelper.cpp \
    $$PWD/PrinterCalibration.cpp \
    $$PWD/PrinterQuality.cpp \
    $$PWD/PrinterReagent.cpp \
    $$PWD/PrinterSample.cpp

FORMS +=
