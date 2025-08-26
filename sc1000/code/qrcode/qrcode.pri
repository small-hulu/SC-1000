QT += serialport

INCLUDEPATH += $$PWD/..

DEFINES += QRCODE_VERSION_MAJOR=0
DEFINES += QRCODE_VERSION_MINOR=1
DEFINES += QRCODE_VERSION_PATCH=1

HEADERS += \
    $$PWD/QRCODE.hpp \
    $$PWD/Qrcode_Type.hpp \
    $$PWD/entity/Qrcode_EBaseAnalysis.h \
    $$PWD/entity/Qrcode_ECalibration.h \
    $$PWD/entity/Qrcode_EConsumable.h \
    $$PWD/entity/Qrcode_EQuality.h \
    $$PWD/entity/Qrcode_EReagent.h \
    $$PWD/serial/Qrcode_SerialBase.h \
    $$PWD/serial/Qrcode_SerialQrcode.h

SOURCES += \
    $$PWD/entity/Qrcode_EBaseAnalysis.cpp \
    $$PWD/entity/Qrcode_ECalibration.cpp \
    $$PWD/entity/Qrcode_EConsumable.cpp \
    $$PWD/entity/Qrcode_EQuality.cpp \
    $$PWD/entity/Qrcode_EReagent.cpp \
    $$PWD/serial/Qrcode_SerialBase.cpp \
    $$PWD/serial/Qrcode_SerialQrcode.cpp
