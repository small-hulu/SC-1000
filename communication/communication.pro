DESTDIR = $$PWD/../1stlib

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++17
include($$PWD/communication.pri)
