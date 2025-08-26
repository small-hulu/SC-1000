TEMPLATE = subdirs

SUBDIRS += 3rdparty/3rdparty.pro
SUBDIRS += log/log.pro
SUBDIRS += communication/communication.pro

SUBDIRS += sc1000/app.pro

CONFIG  += ordered
CONFIG += qmltypes no_keywords
