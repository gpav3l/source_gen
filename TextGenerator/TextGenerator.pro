QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    interfaces/PluginInterface.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
target.path = $$OUT_PATH/
INSTALLS += target

Release:DESTDIR = ../release
Release:OBJECTS_DIR = $${DESTDIR}/.obj
Release:MOC_DIR = $${DESTDIR}/.moc
Release:RCC_DIR = $${DESTDIR}/.rcc
Release:UI_DIR = $${DESTDIR}/.ui

Debug:DESTDIR = ../debug
Debug:OBJECTS_DIR = $${DESTDIR}/.obj
Debug:MOC_DIR = $${DESTDIR}/.moc
Debug:RCC_DIR = $${DESTDIR}/.rcc
Debug:UI_DIR = $${DESTDIR}/.ui
