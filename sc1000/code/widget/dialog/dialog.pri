#######################################################
# 不准依赖外部业务
# 允许直接将这个包，甚至包中的文件直接复制到别的项目中使用
#######################################################

QT += core
QT += widgets
QT += texttospeech

HEADERS += \
    $$PWD/DIALOG.h \
    $$PWD/DIALOG_DInfoBottom.h \
    $$PWD/DIALOG_DInputLine.h \
    $$PWD/DIALOG_DInputText.h \
    $$PWD/DIALOG_DIsVerify.h \
    $$PWD/DIALOG_DProgressIndicator.h \
    $$PWD/DIALOG_DWarning.h

SOURCES += \
    $$PWD/DIALOG.cpp \
    $$PWD/DIALOG_DInfoBottom.cpp \
    $$PWD/DIALOG_DInputLine.cpp \
    $$PWD/DIALOG_DInputText.cpp \
    $$PWD/DIALOG_DIsVerify.cpp \
    $$PWD/DIALOG_DProgressIndicator.cpp \
    $$PWD/DIALOG_DWarning.cpp

FORMS += \
    $$PWD/DIALOG_DInfoBottom.ui \
    $$PWD/DIALOG_DInputLine.ui \
    $$PWD/DIALOG_DInputText.ui \
    $$PWD/DIALOG_DIsVerify.ui \
    $$PWD/DIALOG_DProgressIndicator.ui \
    $$PWD/DIALOG_DWarning.ui
