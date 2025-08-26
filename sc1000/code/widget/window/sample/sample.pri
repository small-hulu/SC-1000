include($$PWD/apply/apply.pri)
include($$PWD/state/state.pri)

FORMS += \
    $$PWD/WIDGET_SampleApply.ui \
    $$PWD/WIDGET_SampleApplyTable.ui \
    $$PWD/WIDGET_SampleState.ui

HEADERS += \
    $$PWD/WIDGET_SampleApply.h \
    $$PWD/WIDGET_SampleApplyTable.h \
    $$PWD/WIDGET_SampleState.h

SOURCES += \
    $$PWD/WIDGET_SampleApply.cpp \
    $$PWD/WIDGET_SampleApplyTable.cpp \
    $$PWD/WIDGET_SampleState.cpp
