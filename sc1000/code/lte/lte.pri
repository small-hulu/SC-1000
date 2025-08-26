QT += serialport

INCLUDEPATH += $$PWD/..

HEADERS += \
    $$PWD/config/CKeyJson.hpp \
    $$PWD/controller/ActionControl.h \
    $$PWD/controller/HashCalculator.h \
    $$PWD/controller/Lte_Scheduler.h \
    $$PWD/controller/Parse.h \
    $$PWD/interface/ICOMThread.h \
    $$PWD/interface/ISingleton.hpp \
    $$PWD/lte.h \
    $$PWD/lte_entity/Task_Node.h \
    $$PWD/lte_lbs.h \
    $$PWD/lte_sc.h


SOURCES += \
    $$PWD/controller/ActionControl.cpp \
    $$PWD/controller/HashCalculator.cpp \
    $$PWD/controller/Lte_Scheduler.cpp \
    $$PWD/controller/Parse.cpp \
    $$PWD/interface/ICOMThread.cpp \
    $$PWD/lte.cpp \
    $$PWD/lte_lbs.cpp \
    $$PWD/lte_sc.cpp

DISTFILES +=
