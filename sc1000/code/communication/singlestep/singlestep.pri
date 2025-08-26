QT += widgets

INCLUDEPATH += $$PWD/..
INCLUDEPATH += $$PWD/../..

FORMS += \
    $$PWD/SingleStepGripper.ui \
    $$PWD/SingleStepMagneticSeparation.ui \
    $$PWD/SingleStepMixing.ui \
    $$PWD/SingleStepPhotoelectric.ui \
    $$PWD/SingleStepPipetteNeedle.ui \
    $$PWD/SingleStepReagentBin.ui \
    $$PWD/SingleStepThermostat.ui \
    $$PWD/WMultStpeTrigger.ui \
    $$PWD/WSingleStepMain.ui

HEADERS += \
    $$PWD/SingleStepGripper.h \
    $$PWD/SingleStepMagneticSeparation.h \
    $$PWD/SingleStepMixing.h \
    $$PWD/SingleStepPhotoelectric.h \
    $$PWD/SingleStepPipetteNeedle.h \
    $$PWD/SingleStepReagentBin.h \
    $$PWD/SingleStepThermostat.h \
    $$PWD/WMultStpeTrigger.h \
    $$PWD/WSSFactory.h \
    $$PWD/WSingleStepMain.h

SOURCES += \
    $$PWD/SingleStepGripper.cpp \
    $$PWD/SingleStepMagneticSeparation.cpp \
    $$PWD/SingleStepMixing.cpp \
    $$PWD/SingleStepPhotoelectric.cpp \
    $$PWD/SingleStepPipetteNeedle.cpp \
    $$PWD/SingleStepReagentBin.cpp \
    $$PWD/SingleStepThermostat.cpp \
    $$PWD/WMultStpeTrigger.cpp \
    $$PWD/WSSFactory.cpp \
    $$PWD/WSingleStepMain.cpp
