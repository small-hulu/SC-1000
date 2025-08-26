QT += sql

INCLUDEPATH += $$PWD/..

HEADERS += \
    $$PWD/DB.hpp \
    $$PWD/DB_Init.h \
    $$PWD/extend/DB_SqlExe.h \
    $$PWD/extend/DB_TableCheck.h \
    $$PWD/extend/DB_TableEntity_calibration.h \
    $$PWD/extend/DB_TableEntity_consume.h \
    $$PWD/extend/DB_TableEntity_expcalibration.h \
    $$PWD/extend/DB_TableEntity_expquality.h \
    $$PWD/extend/DB_TableEntity_expsample.h \
    $$PWD/extend/DB_TableEntity_log.h \
    $$PWD/extend/DB_TableEntity_quality.h \
    $$PWD/extend/DB_TableEntity_reagent.h \
    $$PWD/extend/DB_TableEntity_sample.h \
    $$PWD/extend/DB_TableEntity_sample_apply.h \
    $$PWD/extend/DB_TableEntity_user.h \
    $$PWD/impl/DB_DBEntity.h \
    $$PWD/impl/DB_TableEntityBase.hpp \
    $$PWD/impl/DB_TableFactory.h \
    $$PWD/impl/DB_TableFactory_AttributeCol.h \
    $$PWD/impl/DB_TableFactory_Autoincrement.h \
    $$PWD/impl/DB_TableFactory_KeyValue.h

SOURCES += \
    $$PWD/DB_Init.cpp \
    $$PWD/extend/DB_SqlExe.cpp \
    $$PWD/extend/DB_TableCheck.cpp \
    $$PWD/extend/DB_TableEntity_calibration.cpp \
    $$PWD/extend/DB_TableEntity_consume.cpp \
    $$PWD/extend/DB_TableEntity_expcalibration.cpp \
    $$PWD/extend/DB_TableEntity_expquality.cpp \
    $$PWD/extend/DB_TableEntity_expsample.cpp \
    $$PWD/extend/DB_TableEntity_log.cpp \
    $$PWD/extend/DB_TableEntity_quality.cpp \
    $$PWD/extend/DB_TableEntity_reagent.cpp \
    $$PWD/extend/DB_TableEntity_sample.cpp \
    $$PWD/extend/DB_TableEntity_sample_apply.cpp \
    $$PWD/extend/DB_TableEntity_user.cpp \
    $$PWD/impl/DB_DBEntity.cpp \
    $$PWD/impl/DB_TableFactory.cpp \
    $$PWD/impl/DB_TableFactory_AttributeCol.cpp \
    $$PWD/impl/DB_TableFactory_Autoincrement.cpp \
    $$PWD/impl/DB_TableFactory_KeyValue.cpp
