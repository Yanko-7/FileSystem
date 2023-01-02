#-------------------------------------------------
#
# Project created by QtCreator 2020-10-16T23:32:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FileSystem
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    model_fat.cpp \
    model_fileoperation.cpp \
    model_readdisc.cpp \
    fileoperation.cpp \
    model_filedic.cpp \
    model_openedfiletable.cpp \
    model_stringoperation.cpp \
    control_main.cpp \
    textviewer.cpp \
    attributeviewer.cpp \
    newfiledic.cpp

HEADERS += \
        mainwindow.h \
    model_fat.h \
    model_fileoperation.h \
    model_readdisc.h \
    model_filedic.h \
    model_openedfiletable.h \
    model_stringoperation.h \
    control_main.h \
    textviewer.h \
    attributeviewer.h \
    newfiledic.h

FORMS += \
        mainwindow.ui \
    textviewer.ui \
    attributeviewer.ui \
    newfiledic.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    folder.qrc \
    qs.qrc
