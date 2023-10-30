#QT       += core gui

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#CONFIG += c++11

## You can make your code fail to compile if it uses deprecated APIs.
## In order to do so, uncomment the following line.
##DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#SOURCES += \
#    main.cpp \
#    mainwindow.cpp

#HEADERS += \
#    mainwindow.h

#FORMS += \
#    mainwindow.ui

## Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

QT	+= gui
greaterThan(QT_MAJOR_VERSION, 4)
{
  QT +=  widgets
}

VERSION=1.0
DESTDIR=.
TARGET  = netobj
TEMPLATE	= app
QMAKE_LFLAGS += -Wl,-rpath-link,$(GISDESIGNER_ROOT)/usr/lib
QMAKE_CXXFLAGS += -finput-charset=cp1251
CONFIG	+= qt warn_on release

unix:INCLUDEPATH	+= $(GISDESIGNER_ROOT)/usr/include/gisdesigner
unix:LIBS	+= -L$(GISDESIGNER_ROOT)/usr/lib -lqdmapacces -lmapcomponents -ldl
LANGUAGE	= C++

SOURCES	+= main.cpp \
    UI/mainwindow.cpp \
    pathfinder.cpp

FORMS += \
    UI/mainwindow.ui

HEADERS += \
    UI/mainwindow.h \
    pathfinder.h
