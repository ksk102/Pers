#-------------------------------------------------
#
# Project created by QtCreator 2017-07-30T13:12:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Pers
TEMPLATE = app

# set application icon
RC_FILE = Pers.rc

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    category_mst.cpp \
    add_cat.cpp \
    warnline_mst.cpp \
    add_warn.cpp \
    general.cpp \
    edit_cat.cpp \
    edit_warn.cpp \
    common_warn.cpp

HEADERS += \
        mainwindow.h \
    category_mst.h \
    add_cat.h \
    warnline_mst.h \
    add_warn.h \
    general.h \
    edit_cat.h \
    edit_warn.h \
    common_warn.h

FORMS += \
        mainwindow.ui \
    category_mst.ui \
    add_cat.ui \
    warnline_mst.ui \
    add_warn.ui \
    edit_cat.ui \
    edit_warn.ui
