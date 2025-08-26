INCLUDEPATH += $$PWD/..

include($$PWD/device/device.pri)

HEADERS += \
    $$PWD/AppEvent.h \
    $$PWD/AppInit.h \
    $$PWD/CInterface_DBEntity.hpp \
    $$PWD/CONTROLLER_Calibration.h \
    $$PWD/CONTROLLER_Config.h \
    $$PWD/CONTROLLER_Consumable.h \
    $$PWD/CONTROLLER_ExpCalibration.h \
    $$PWD/CONTROLLER_ExpQuality.h \
    $$PWD/CONTROLLER_ExpSample.h \
    $$PWD/CONTROLLER_Log.h \
    $$PWD/CONTROLLER_LoginUser.h \
    $$PWD/CONTROLLER_Project.h \
    $$PWD/CONTROLLER_Quality.h \
    $$PWD/CONTROLLER_Reagent.h \
    $$PWD/CONTROLLER_ReagentBinState.h \
    $$PWD/CONTROLLER_Sample.h \
    $$PWD/CONTROLLER_SampleApply.h \
    $$PWD/CONTROLLER_UpToLower.h \
    $$PWD/IniSC1000.h

SOURCES += \
    $$PWD/AppEvent.cpp \
    $$PWD/AppInit.cpp \
    $$PWD/CONTROLLER_Calibration.cpp \
    $$PWD/CONTROLLER_Config.cpp \
    $$PWD/CONTROLLER_Consumable.cpp \
    $$PWD/CONTROLLER_ExpCalibration.cpp \
    $$PWD/CONTROLLER_ExpQuality.cpp \
    $$PWD/CONTROLLER_ExpSample.cpp \
    $$PWD/CONTROLLER_Log.cpp \
    $$PWD/CONTROLLER_LoginUser.cpp \
    $$PWD/CONTROLLER_Project.cpp \
    $$PWD/CONTROLLER_Quality.cpp \
    $$PWD/CONTROLLER_Reagent.cpp \
    $$PWD/CONTROLLER_ReagentBinState.cpp \
    $$PWD/CONTROLLER_Sample.cpp \
    $$PWD/CONTROLLER_SampleApply.cpp \
    $$PWD/CONTROLLER_UpToLower.cpp \
    $$PWD/IniSC1000.cpp
