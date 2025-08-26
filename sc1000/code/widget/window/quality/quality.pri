# 质控模块

include($$PWD/apply/apply.pri)
include($$PWD/result/result.pri)
include($$PWD/analyse/analyse.pri)
include($$PWD/manage/manage.pri)

FORMS += \
    $$PWD/WIDGET_QualityAnalyse.ui \
    $$PWD/WIDGET_QualityApply.ui \
    $$PWD/WIDGET_QualityCurve.ui \
    $$PWD/WIDGET_QualityManage.ui \
    $$PWD/WIDGET_QualityResult.ui

HEADERS += \
    $$PWD/WIDGET_QualityAnalyse.h \
    $$PWD/WIDGET_QualityApply.h \
    $$PWD/WIDGET_QualityCurve.h \
    $$PWD/WIDGET_QualityManage.h \
    $$PWD/WIDGET_QualityResult.h

SOURCES += \
    $$PWD/WIDGET_QualityAnalyse.cpp \
    $$PWD/WIDGET_QualityApply.cpp \
    $$PWD/WIDGET_QualityCurve.cpp \
    $$PWD/WIDGET_QualityManage.cpp \
    $$PWD/WIDGET_QualityResult.cpp
