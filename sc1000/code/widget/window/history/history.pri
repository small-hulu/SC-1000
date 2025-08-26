include($$PWD/historySample/historySample.pri)

FORMS += \
    $$PWD/WIDGET_HistorySample.ui \
    $$PWD/WSample_DSampleHistoryFilter.ui \
    $$PWD/WSample_DSampleHistoryModify.ui \
    $$PWD/WSample_SampleHistoryDetail.ui \
    $$PWD/widget_printviewsample.ui

HEADERS += \
    $$PWD/InfoSample.h \
    $$PWD/WIDGET_HistorySample.h \
    $$PWD/WIDGET_PrintViewSample.h \
    $$PWD/WSample_DSampleHistoryFilter.h \
    $$PWD/WSample_DSampleHistoryModify.h \
    $$PWD/WSample_SampleHistoryDetail.h

SOURCES += \
    $$PWD/InfoSample.cpp \
    $$PWD/WIDGET_HistorySample.cpp \
    $$PWD/WIDGET_PrintViewSample.cpp \
    $$PWD/WSample_DSampleHistoryFilter.cpp \
    $$PWD/WSample_DSampleHistoryModify.cpp \
    $$PWD/WSample_SampleHistoryDetail.cpp
