#-------------------------------------------------
#
# Project created by QtCreator 2016-04-10T08:55:36
#
#-------------------------------------------------
#if QT_VERSION >= 0x050000
QT       += core gui widgets
#else
QT       += core gui
#endif

QT       += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SailerCom
TEMPLATE = app


SOURCES += main.cpp\
        com.cpp \
    crc32.cpp \
    qextserial/qextserialport.cpp \

win32 {
     SOURCES += qextserial/qextserialport_win.cpp
}

unix {
     SOURCES += qextserial/qextserialport_unix.cpp
}

HEADERS  += com.h \
    crc32.h \
    qextserial/qextserialport.h \
    qextserial/qextserialport_global.h

FORMS    += com.ui

RC_ICONS = logo.ico

RC_FILE = myapp.rc

DISTFILES += \
    myapp.rc
