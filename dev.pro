TEMPLATE = subdirs

#SUBDIRS += 3rdparty/3rdparty.pro
SUBDIRS += log/log.pro
SUBDIRS += communication/communication.pro

SUBDIRS += dev/dev.pro
SUBDIRS += dev/test-plan/test-plan.pro
SUBDIRS += dev/test-stress/test-stress.pro
SUBDIRS += dev/test-bootloader/test-bootloader.pro

CONFIG  += ordered
