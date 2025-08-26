include($$PWD/modules/modules.pri)
include($$PWD/projectManage/projectManage.pri)
include($$PWD/deviceConfig/deviceConfig.pri)
include($$PWD/hospitalManage/hospitalManage.pri)
include($$PWD/lisManage/lisManage.pri)
include($$PWD/logManage/logManage.pri)
include($$PWD/userManage/userManage.pri)
include($$PWD/printManage/printManage.pri)

FORMS += \
    $$PWD/WIDGET_SettingDeviceConfig.ui \
    $$PWD/WIDGET_SettingHospitalManage.ui \
    $$PWD/WIDGET_SettingLISManage.ui \
    $$PWD/WIDGET_SettingLogManage.ui \
    $$PWD/WIDGET_SettingPrintManage.ui \
    $$PWD/WIDGET_SettingProjectManage.ui \
    $$PWD/WIDGET_SettingUserManage.ui

HEADERS += \
    $$PWD/WIDGET_SettingDeviceConfig.h \
    $$PWD/WIDGET_SettingHospitalManage.h \
    $$PWD/WIDGET_SettingLISManage.h \
    $$PWD/WIDGET_SettingLogManage.h \
    $$PWD/WIDGET_SettingPrintManage.h \
    $$PWD/WIDGET_SettingProjectManage.h \
    $$PWD/WIDGET_SettingUserManage.h

SOURCES += \
    $$PWD/WIDGET_SettingDeviceConfig.cpp \
    $$PWD/WIDGET_SettingHospitalManage.cpp \
    $$PWD/WIDGET_SettingLISManage.cpp \
    $$PWD/WIDGET_SettingLogManage.cpp \
    $$PWD/WIDGET_SettingPrintManage.cpp \
    $$PWD/WIDGET_SettingProjectManage.cpp \
    $$PWD/WIDGET_SettingUserManage.cpp
