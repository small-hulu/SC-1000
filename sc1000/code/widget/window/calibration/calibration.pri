# 校准模块

include($$PWD/apply/apply.pri)
include($$PWD/manage/manage.pri)
include($$PWD/result/result.pri)
include($$PWD/curve/curve.pri)

FORMS += \
    $$PWD/WIDGET_CalibrationApply.ui \
    $$PWD/WIDGET_CalibrationCurve.ui \
    $$PWD/WIDGET_CalibrationManage.ui \
    $$PWD/WIDGET_CalibrationResult.ui

HEADERS += \
    $$PWD/WIDGET_CalibrationApply.h \
    $$PWD/WIDGET_CalibrationCurve.h \
    $$PWD/WIDGET_CalibrationManage.h \
    $$PWD/WIDGET_CalibrationResult.h

SOURCES += \
    $$PWD/WIDGET_CalibrationApply.cpp \
    $$PWD/WIDGET_CalibrationCurve.cpp \
    $$PWD/WIDGET_CalibrationManage.cpp \
    $$PWD/WIDGET_CalibrationResult.cpp
