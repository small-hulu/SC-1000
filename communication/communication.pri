QT += serialport
QT += serialbus
QT += core
CONFIG += c++17

DEFINES += COM_VERSION_MAJOR=1
DEFINES += COM_VERSION_MINOR=0
DEFINES += COM_VERSION_PATCH=0

INCLUDEPATH += $$PWD/..

CONFIG(release, debug|release) {
    DEFINES += QT_MESSAGELOGCONTEXT
}

include($$PWD/bootloader/bootloader.pri)

HEADERS += \
    $$PWD/COM.hpp \
    $$PWD/COM_Device.hpp \
    $$PWD/COM_pch.hpp \
    $$PWD/Version.h \
    $$PWD/config/CKeyJson.hpp \
    $$PWD/config/CLteErrorCode.hpp \
    $$PWD/config/CParams.hpp \
    $$PWD/config/CPath.hpp \
    $$PWD/config/CRetCode.h \
    $$PWD/config/SharedMemory.h \
    $$PWD/controller/ActionMajor.h \
    $$PWD/controller/ActionMinor.h \
    $$PWD/controller/ActionPatch.h \
    $$PWD/controller/DeviceOpAuthorizeCU.h \
    $$PWD/controller/DeviceStateMediator.h \
    $$PWD/controller/DispatchController.h \
    $$PWD/controller/Event.h \
    $$PWD/controller/Init.h \
    $$PWD/controller/Parse.h \
    $$PWD/controller/WatchDog.h \
    $$PWD/device/DeviceManager.h \
    $$PWD/device/IDeviceConfig.hpp \
    $$PWD/device/IDeviceFixture.hpp \
    $$PWD/device/IDeviceThread.h \
    $$PWD/device/IModbusProxy.h \
    $$PWD/device/ModbusImpl.h \
    $$PWD/device/gripper/AGripper.h \
    $$PWD/device/gripper/CGripper.h \
    $$PWD/device/gripper/DGripper.h \
    $$PWD/device/incubate/CUIncubate.h \
    $$PWD/device/incubate/DIncubate.h \
    $$PWD/device/magneticSeparation/AMagneticSeparation.h \
    $$PWD/device/magneticSeparation/CMagneticSeparation.h \
    $$PWD/device/magneticSeparation/CUMagneticSeparation.h \
    $$PWD/device/magneticSeparation/DMagneticSeparation.h \
    $$PWD/device/mixing/AMixing.h \
    $$PWD/device/mixing/CMixing.h \
    $$PWD/device/mixing/CUMixing.h \
    $$PWD/device/mixing/DMixing.h \
    $$PWD/device/photoelectric/APhotoelectric.h \
    $$PWD/device/photoelectric/CPhotoelectric.h \
    $$PWD/device/photoelectric/CUPhotoelectric.h \
    $$PWD/device/photoelectric/DPhotoelectric.h \
    $$PWD/device/pipettingNeedle/APipetteNeedle.h \
    $$PWD/device/pipettingNeedle/CPipetteNeedle.h \
    $$PWD/device/pipettingNeedle/DPipetteNeedle.h \
    $$PWD/device/reagentBin/AReagentBin.h \
    $$PWD/device/reagentBin/CReagentBin.h \
    $$PWD/device/reagentBin/DReagentBin.h \
    $$PWD/device/thermostat/AThermostat.h \
    $$PWD/device/thermostat/CThermostat.h \
    $$PWD/device/thermostat/DThermostat.h \
    $$PWD/entity/TaskNode.h \
    $$PWD/entity/TaskPackage.h \
    $$PWD/entity/Tube.h \
    $$PWD/interface/ICOMThread.h \
    $$PWD/interface/ISingleton.hpp \
    $$PWD/utility/Helper.h \
    $$PWD/utility/IniConfig.h \
    $$PWD/utility/JsonConfig.h \
    $$PWD/utility/PDT.h \
    $$PWD/utility/ToString.h \
    $$PWD/utility/UnitConv.h

SOURCES += \
    $$PWD/Version.cpp \
    $$PWD/config/CRetCode.cpp \
    $$PWD/config/SharedMemory.cpp \
    $$PWD/controller/ActionMajor.cpp \
    $$PWD/controller/ActionMinor.cpp \
    $$PWD/controller/ActionPatch.cpp \
    $$PWD/controller/DeviceOpAuthorizeCU.cpp \
    $$PWD/controller/DeviceStateMediator.cpp \
    $$PWD/controller/DispatchController.cpp \
    $$PWD/controller/Event.cpp \
    $$PWD/controller/Init.cpp \
    $$PWD/controller/Parse.cpp \
    $$PWD/controller/WatchDog.cpp \
    $$PWD/device/DeviceManager.cpp \
    $$PWD/device/IDeviceThread.cpp \
    $$PWD/device/IModbusProxy.cpp \
    $$PWD/device/ModbusImpl.cpp \
    $$PWD/device/gripper/AGripper.cpp \
    $$PWD/device/gripper/CGripper.cpp \
    $$PWD/device/gripper/DGripper.cpp \
    $$PWD/device/incubate/CUIncubate.cpp \
    $$PWD/device/incubate/DIncubate.cpp \
    $$PWD/device/magneticSeparation/AMagneticSeparation.cpp \
    $$PWD/device/magneticSeparation/CMagneticSeparation.cpp \
    $$PWD/device/magneticSeparation/CUMagneticSeparation.cpp \
    $$PWD/device/magneticSeparation/DMagneticSeparation.cpp \
    $$PWD/device/mixing/AMixing.cpp \
    $$PWD/device/mixing/CMixing.cpp \
    $$PWD/device/mixing/CUMixing.cpp \
    $$PWD/device/mixing/DMixing.cpp \
    $$PWD/device/photoelectric/APhotoelectric.cpp \
    $$PWD/device/photoelectric/CPhotoelectric.cpp \
    $$PWD/device/photoelectric/CUPhotoelectric.cpp \
    $$PWD/device/photoelectric/DPhotoelectric.cpp \
    $$PWD/device/pipettingNeedle/APipetteNeedle.cpp \
    $$PWD/device/pipettingNeedle/CPipetteNeedle.cpp \
    $$PWD/device/pipettingNeedle/DPipetteNeedle.cpp \
    $$PWD/device/reagentBin/AReagentBin.cpp \
    $$PWD/device/reagentBin/CReagentBin.cpp \
    $$PWD/device/reagentBin/DReagentBin.cpp \
    $$PWD/device/thermostat/AThermostat.cpp \
    $$PWD/device/thermostat/CThermostat.cpp \
    $$PWD/device/thermostat/DThermostat.cpp \
    $$PWD/entity/TaskNode.cpp \
    $$PWD/entity/TaskPackage.cpp \
    $$PWD/entity/Tube.cpp \
    $$PWD/interface/ICOMThread.cpp \
    $$PWD/utility/Helper.cpp \
    $$PWD/utility/IniConfig.cpp \
    $$PWD/utility/JsonConfig.cpp \
    $$PWD/utility/PDT.cpp \
    $$PWD/utility/ToString.cpp \
    $$PWD/utility/UnitConv.cpp
