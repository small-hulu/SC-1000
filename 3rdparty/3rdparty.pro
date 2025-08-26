DESTDIR = $$PWD/../1stlib

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++17

include($$PWD/frm/frm.pri)
include($$PWD/qcustomplot/qcustomplot.pri)
