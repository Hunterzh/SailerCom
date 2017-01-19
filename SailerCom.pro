#-------------------------------------------------
#
# Project created by QtCreator 2016-04-10T08:55:36
#
#-------------------------------------------------

QT       += core gui
QT       += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SailerCom
TEMPLATE = app


SOURCES += main.cpp\
        com.cpp \
    crc32.cpp \
    qextserial/qextserialport.cpp \
    qextserial/qextserialport_win.cpp

HEADERS  += com.h \
    crc32.h \
    qextserial/qextserialport.h \
    qextserial/qextserialport_global.h

FORMS    += com.ui

RC_ICONS = logo.ico

RC_FILE = myapp.rc

DISTFILES += \
    myapp.rc
