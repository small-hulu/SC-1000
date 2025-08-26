QT += serialport

INCLUDEPATH += $$PWD/..

HEADERS += \
    $$PWD/Lis.h \
    $$PWD/Lis_SC1000.h \
    $$PWD/SerialBase.h


SOURCES += \
    $$PWD/Lis.cpp \
    $$PWD/Lis_SC1000.cpp \
    $$PWD/SerialBase.cpp
