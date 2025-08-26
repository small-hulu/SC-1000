QT += sql

SOURCES += \
        $$PWD/Frm.cpp \
        $$PWD/frminput.cpp \
        $$PWD/frmmain.cpp \
        $$PWD/frmnum.cpp

HEADERS += \
        $$PWD/Frm.h \
        $$PWD/frminput.h \
        $$PWD/frmmain.h \
        $$PWD/frmnum.h

FORMS += \
        $$PWD/frminput.ui \
        $$PWD/frmmain.ui \
        $$PWD/frmnum.ui

DISTFILES += \
    $$PWD/readme.md

# 注意，这是个非官方的方式
CONFIG += file_copies
examples.files = $$PWD/py.db
examples.path = $$DESTDIR
COPIES += examples
