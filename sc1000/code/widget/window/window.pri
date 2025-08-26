include($$PWD/main/main.pri)

include($$PWD/homepage/homepage.pri)
include($$PWD/sample/sample.pri)
include($$PWD/reagent/reagent.pri)
include($$PWD/help/help.pri)
include($$PWD/setting/setting.pri)
include($$PWD/maintain/maintain.pri)
include($$PWD/calibration/calibration.pri)
include($$PWD/quality/quality.pri)
include($$PWD/alarm/alarm.pri)
include($$PWD/history/history.pri)

FORMS += \
    $$PWD/testui.ui

HEADERS += \
    $$PWD/testui.h

SOURCES += \
    $$PWD/testui.cpp
