QT += widgets

INCLUDEPATH += $$PWD/..

FORMS += \
    $$PWD/WMainFixture.ui \
    $$PWD/WOperationPanel.ui

HEADERS += \
    $$PWD/FIXTURE.hpp \
    $$PWD/WFactoryFix.h \
    $$PWD/WMainFixture.h \
    $$PWD/WOperationPanel.h \
    $$PWD/gripper/Gripper.h \
    $$PWD/gripper/GripperCup.h \
    $$PWD/gripper/GripperDilute.h \
    $$PWD/gripper/GripperIncubateA.h \
    $$PWD/gripper/GripperIncubateB.h \
    $$PWD/gripper/GripperIncubateC.h \
    $$PWD/gripper/GripperLoad.h \
    $$PWD/gripper/GripperMagneticSeparation.h \
    $$PWD/gripper/GripperMixing.h \
    $$PWD/gripper/GripperPhotoelectric.h \
    $$PWD/gripper/GripperRangeMax.h \
    $$PWD/gripper/GripperRangeMin.h \
    $$PWD/gripper/GripperThrow.h \
    $$PWD/magneticSeparation/MagneticSeparation.h \
    $$PWD/magneticSeparation/MagneticSeparationACapacity.h \
    $$PWD/magneticSeparation/MagneticSeparationTray.h \
    $$PWD/magneticSeparation/MagneticSeparationZDrainDeep.h \
    $$PWD/magneticSeparation/MagneticSeparationZMixHeight.h \
    $$PWD/mixing/FixCup.h \
    $$PWD/mixing/FixCupRangeMax.h \
    $$PWD/mixing/FixCupRangeMin.h \
    $$PWD/photoelectric/Photoelectric.h \
    $$PWD/photoelectric/PhotoelectricBCapacity.h \
    $$PWD/photoelectric/PhotoelectricRangeMin.h \
    $$PWD/pipetteNeedle/PipetteNeedle.h \
    $$PWD/pipetteNeedle/PipetteNeedleDilute.h \
    $$PWD/pipetteNeedle/PipetteNeedleLoad.h \
    $$PWD/pipetteNeedle/PipetteNeedleReagentBin.h \
    $$PWD/pipetteNeedle/PipetteNeedleReagentBinZ1.h \
    $$PWD/pipetteNeedle/PipetteNeedleWash.h \
    $$PWD/reagentBin/ReagentBin.h \
    $$PWD/reagentBin/ReagentBinBack.h \
    $$PWD/reagentBin/ReagentBinFront.h \
    $$PWD/thermostat/FixThermostat.h \
    $$PWD/thermostat/FixThermostatTempIncubate.h \
    $$PWD/thermostat/FixThermostatTempPhotoelectric.h \
    $$PWD/thermostat/FixThermostatTempReagentBinLid.h

SOURCES += \
    $$PWD/WFactoryFix.cpp \
    $$PWD/WMainFixture.cpp \
    $$PWD/WOperationPanel.cpp \
    $$PWD/gripper/Gripper.cpp \
    $$PWD/gripper/GripperCup.cpp \
    $$PWD/gripper/GripperDilute.cpp \
    $$PWD/gripper/GripperIncubateA.cpp \
    $$PWD/gripper/GripperIncubateB.cpp \
    $$PWD/gripper/GripperIncubateC.cpp \
    $$PWD/gripper/GripperLoad.cpp \
    $$PWD/gripper/GripperMagneticSeparation.cpp \
    $$PWD/gripper/GripperMixing.cpp \
    $$PWD/gripper/GripperPhotoelectric.cpp \
    $$PWD/gripper/GripperRangeMax.cpp \
    $$PWD/gripper/GripperRangeMin.cpp \
    $$PWD/gripper/GripperThrow.cpp \
    $$PWD/magneticSeparation/MagneticSeparation.cpp \
    $$PWD/magneticSeparation/MagneticSeparationACapacity.cpp \
    $$PWD/magneticSeparation/MagneticSeparationTray.cpp \
    $$PWD/magneticSeparation/MagneticSeparationZDrainDeep.cpp \
    $$PWD/magneticSeparation/MagneticSeparationZMixHeight.cpp \
    $$PWD/mixing/FixCup.cpp \
    $$PWD/mixing/FixCupRangeMax.cpp \
    $$PWD/mixing/FixCupRangeMin.cpp \
    $$PWD/photoelectric/Photoelectric.cpp \
    $$PWD/photoelectric/PhotoelectricBCapacity.cpp \
    $$PWD/photoelectric/PhotoelectricRangeMin.cpp \
    $$PWD/pipetteNeedle/PipetteNeedle.cpp \
    $$PWD/pipetteNeedle/PipetteNeedleDilute.cpp \
    $$PWD/pipetteNeedle/PipetteNeedleLoad.cpp \
    $$PWD/pipetteNeedle/PipetteNeedleReagentBin.cpp \
    $$PWD/pipetteNeedle/PipetteNeedleReagentBinZ1.cpp \
    $$PWD/pipetteNeedle/PipetteNeedleWash.cpp \
    $$PWD/reagentBin/ReagentBin.cpp \
    $$PWD/reagentBin/ReagentBinBack.cpp \
    $$PWD/reagentBin/ReagentBinFront.cpp \
    $$PWD/thermostat/FixThermostat.cpp \
    $$PWD/thermostat/FixThermostatTempIncubate.cpp \
    $$PWD/thermostat/FixThermostatTempPhotoelectric.cpp \
    $$PWD/thermostat/FixThermostatTempReagentBinLid.cpp
